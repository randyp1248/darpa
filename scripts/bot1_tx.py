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

import time, struct, sys, socket, random

#import os 
#print os.getpid()
#raw_input('Attach and press enter')

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
                      default='psk',
                      help="Select modulation from: %s [default=%%default]"
                            % (', '.join(mods.keys()),))
    parser.add_option("-b", "--burst-length", type="int", default=50, 
                      help="Number of packets in each burst")
    parser.add_option("-t", "--sleep-time", type="int", default=100, 
                      help="sleep time (msec) between the bursts")
    parser.add_option("-s", "--server", type="string", default='idb2',
                      help="server hosting the packet server/sink")
    parser.add_option("-o", "--port", type="int", default='5123',
                      help="tcp port on the packet server/sink serving packets")
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
           

    # build the graph
    tb = my_top_block(mods[options.modulation], options)

    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: failed to enable realtime scheduling"

    tb.start()                       # start flow graph
        
    # Fetch packets from server

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(10)
    TCP_IP=options.server
    TCP_PORT=options.port
    try:
       s.connect((TCP_IP, TCP_PORT))
    except socket.timeout: 
       print"Connection timed out, try again later"
       return
    except socket.error:
       print"Connection error"
       return

   
    pktno = 1
    pkt_size = int(1442)
    MESSAGE = struct.pack('!l',pkt_size-2)
    random.seed()
    #tmp_pktno = random.randint(10,30)

    while 1: #n < nbytes:
        #if options.from_file is None:
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
          #First 4 bytes are checksum followed by the 4 byte sequence number
          crc,sn = struct.unpack('!LL',data[:8])
          print "Seq #:", sn, " with CRC [", hex(crc), "]"
                

        payload = struct.pack('!H', pktno & 0xffff) + data
        send_pkt(payload)
        sys.stderr.write('.')
        if pktno % (options.burst_length) == 0:
            time.sleep(options.sleep_time/1000)
        pktno += 1
        
    s.close()
    time.sleep(5)
    send_pkt(eof=True)

    tb.wait()                       # wait for it to finish

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
