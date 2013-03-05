#!/usr/bin/env python
#
# Copyright 2010,2011 Free Software Foundation, Inc.
# 
# This file is part of GNU Radio
# 
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr
from gnuradio import eng_notation
from gnuradio.eng_option import eng_option
from optparse import OptionParser

# From gr-digital
from gnuradio import digital

# from current dir
from transmit_path import transmit_path
from uhd_interface import uhd_transmitter
from uhd_interface import uhd_receiver

import time, struct, sys, socket
import filter_swig as filter
import copy

#import os 
#print os.getpid()
#raw_input('Attach and press enter')

class filter_top_block(gr.top_block):
    def __init__(self, options):
        gr.top_block.__init__(self)

        options = copy.copy(options)
        symbol_rate = 2500000

 
 
        print "%f" % (options.tx_freq-1.25e6)
        #print "==========================================\n"
        #print "Samp per sym = %d\n" % options.samples_per_symbol
        #print "==========================================\n"

        self.source = uhd_receiver(options.args, symbol_rate, 
                                   1, #options.samples_per_symbol,
                                   options.tx_freq-1.25e6, 30,
                                   options.spec, options.antenna,
                                   options.verbose)

        low_pass_taps = [
	    -0.0401,
	    0.0663,
	    0.0468,
	    -0.0235,
	    -0.0222,
	    0.0572,
	    0.0299,
	    -0.1001,
	    -0.0294,
	    0.3166,
	    0.5302,
	    0.3166,
	    -0.0294,
	    -0.1001,
	    0.0299,
	    0.0572,
	    -0.0222,
	    -0.0235,
	    0.0468,
	    0.0663,
	    -0.0401]

	high_pass_taps = [
	    -0.0389,
	    -0.0026,
	    0.0302,
	    0.0181,
	    -0.0357,
	    -0.0394,
	    0.0450,
	    0.0923,
	    -0.0472,
	    -0.3119,
	    0.5512,
	    -0.3119,
	    -0.0472,
	    0.0923,
	    0.0450,
	    -0.0394,
	    -0.0357,
	    0.0181,
	    0.0302,
	    -0.0026,
	    -0.0389]

        # Carrier Sensing Blocks
        alpha = 0.5
        thresh = 30   # in dB, will have to adjust
        self.probe_lp = gr.probe_avg_mag_sqrd_c(thresh,alpha)
        self.probe_hp = gr.probe_avg_mag_sqrd_c(thresh,alpha)

        self.lp = gr.fft_filter_ccc(32, low_pass_taps)
        self.hp = gr.fft_filter_ccc(32, high_pass_taps)

        self.connect(self.source, self.lp)
        self.connect(self.lp, self.probe_lp)

        self.connect(self.source, self.hp)
        self.connect(self.hp, self.probe_hp)
        

class my_top_block(gr.top_block):
    def __init__(self, modulator, options):
        gr.top_block.__init__(self)

        if(options.tx_freq is not None):
            # Work-around to get the modulation's bits_per_symbol
            args = modulator.extract_kwargs_from_options(options)
            symbol_rate = options.bitrate / modulator(**args).bits_per_symbol()

            self.sink = uhd_transmitter(options.args, symbol_rate,
                                        options.samples_per_symbol,
                                        options.tx_freq, options.tx_gain,
                                        options.spec, options.antenna,
                                        options.verbose)
            options.samples_per_symbol = self.sink._sps
            
        elif(options.to_file is not None):
            sys.stderr.write(("Saving samples to '%s'.\n\n" % (options.to_file)))
            self.sink = gr.file_sink(gr.sizeof_gr_complex, options.to_file)
        else:
            sys.stderr.write("No sink defined, dumping samples to null sink.\n\n")
            self.sink = gr.null_sink(gr.sizeof_gr_complex)

        # do this after for any adjustments to the options that may
        # occur in the sinks (specifically the UHD sink)
        self.txpath = transmit_path(modulator, options)

        self.connect(self.txpath, self.sink)

# /////////////////////////////////////////////////////////////////////////////
#                                   main
# /////////////////////////////////////////////////////////////////////////////

def main():

    def send_pkt(payload='', eof=False):
        return tb.txpath.send_pkt(payload, eof)

    mods = digital.modulation_utils.type_1_mods()

    parser = OptionParser(option_class=eng_option, conflict_handler="resolve")
    expert_grp = parser.add_option_group("Expert")

    parser.add_option("-m", "--modulation", type="choice", choices=mods.keys(),
                      default='dqpsk',
                      help="Select modulation from: %s [default=%%default]"
                            % (', '.join(mods.keys()),))

 #    parser.add_option("-s", "--size", type="eng_float", default=1442,
 #                     help="set packet size [default=%default]")
 #   parser.add_option("-M", "--megabytes", type="eng_float", default=1.0,
 #                     help="set megabytes to transmit [default=%default]")
    parser.add_option("","--discontinuous", action="store_true", default=False,
                      help="enable discontinous transmission (bursts of 5 packets)")
    parser.add_option("","--from-file", default=None,
                      help="use intput file for packet contents")
    parser.add_option("","--to-file", default=None,
                      help="Output file for modulated samples")

    transmit_path.add_options(parser, expert_grp)
    uhd_transmitter.add_options(parser)

    for mod in mods.values():
        mod.add_options(expert_grp)

    (options, args) = parser.parse_args ()

    if len(args) != 0:
        parser.print_help()
        sys.exit(1)
           
    if options.from_file is not None:
        source_file = open(options.from_file, 'r')

    # build the graph
    tb = my_top_block(mods[options.modulation], options)
    ftb = filter_top_block(options)

    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: failed to enable realtime scheduling"

    tb.start()                       # start flow graph
    ftb.start()
        
    # log parameter to OML
    cmd1 = "/root/OML/omlcli --out h2_benchmark --line \""
    cmd1 = cmd1 + " tx-freq=" + str(options.tx_freq)
    cmd1 = cmd1 + " modulation=" + str(options.modulation)
    cmd1 = cmd1 + " tx-gain=" + str(options.tx_gain)
    cmd1 = cmd1 + " bitrate=" + str(options.bitrate)
    cmd1 = cmd1 + " sps=" + str(options.samples_per_symbol)
    cmd1 = cmd1 + "\""

    from subprocess import os
    os.system(cmd1)


    # Fetch packets from server

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(10)
    TCP_IP='10.10.0.51'
    TCP_PORT=5123    
    try:
       s.connect((TCP_IP, TCP_PORT))
    except socket.timeout: 
       print"Connection timed out, try again later"
       return
    except socket.error:
       print"Connection error"
       return

    print "Setting frequency to %d\n" % (options.tx_freq+625000)
    tb.sink.set_freq(options.tx_freq+625000)
   
    n = 0
    pktno = 0
    pkt_size = int(1442)
    MESSAGE = struct.pack('!l',pkt_size-2)

    while 1: #n < nbytes:
        if options.from_file is None:
            try:
               s.send(MESSAGE)
               data=s.recv(pkt_size-2)
            except socket.timeout: 
               print"Connection timed out, try again later"
               return
            except socket.error:
               print"Connection closed"
               return
            if data.__len__() < 8:
               print "Connection timed out, try again later"
               break
            if options.verbose:
                # First 4 bytes are checksum followed by the 4 byte sequence number
                   crc,sn = struct.unpack('!LL',data[:8])
                   print "Seq #:", sn, " with CRC [", hex(crc), "]"
                
        else:
            data = source_file.read(pkt_size - 2)
            if data == '':
                break;

        payload = struct.pack('!H', pktno & 0xffff) + data
        send_pkt(payload)
        n += len(payload)
        sys.stderr.write('.')
        if options.discontinuous and pktno % 5 == 4:
            time.sleep(1)

        if pktno % 25 == 25-1:
            tb.sink.set_gain(-60)
            time.sleep(0.250)
            low_sum = ftb.probe_lp.level()
            high_sum = ftb.probe_hp.level()
                
            #while 1:
                #time.sleep(0.1)
                #low_sum = ftb.probe_lp.level()
                #high_sum = ftb.probe_hp.level()
                #print " low=%f\nhigh=%f\n" % (low_sum*100000, high_sum*100000)
 
            print "\n low=%f\nhigh=%f\n" % (low_sum*100000, high_sum*100000)

            if low_sum > high_sum:
                print "Setting frequency to %d\n" % (options.tx_freq+625000)
                tb.sink.set_freq(options.tx_freq+625000)
            else:
                print "Setting frequency to %d\n" % (options.tx_freq-625000)
                tb.sink.set_freq(options.tx_freq-625000)

            tb.sink.set_gain(30)

        #if pktno % 50 == 50-1:
        #    print "Setting frequency to %d\n" % (options.tx_freq+625000)
            #tb.sink.set_freq(options.tx_freq+625000)

        pktno += 1
        
    if options.from_file is None:
        s.close()
    time.sleep(5)
    send_pkt(eof=True)

    tb.wait()                       # wait for it to finish

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass

