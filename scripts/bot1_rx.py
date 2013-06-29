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

from gnuradio import gr, gru
from gnuradio import eng_notation
from gnuradio.eng_option import eng_option
from optparse import OptionParser
import time

# From gr-digital
from gnuradio import digital

# from current dir
from receive_path import receive_path
from uhd_interface import uhd_receiver

import struct
import sys, socket

#import os
#print os.getpid()
#raw_input('Attach and press enter: ')

class my_top_block(gr.top_block):
    def __init__(self, demodulator, rx_callback, options):
        gr.top_block.__init__(self)

        if(options.rx_freq is not None):
            # Work-around to get the modulation's bits_per_symbol
            args = demodulator.extract_kwargs_from_options(options)
            symbol_rate = options.bitrate / demodulator(**args).bits_per_symbol()

            self.source = uhd_receiver(options.args, symbol_rate,
                                       options.samples_per_symbol,
                                       options.rx_freq, options.rx_gain,
                                       options.spec, options.antenna,1)
                                       #options.verbose)
            options.samples_per_symbol = self.source._sps

        elif(options.from_file is not None):
            sys.stderr.write(("Reading samples from '%s'.\n\n" % (options.from_file)))
            self.source = gr.file_source(gr.sizeof_gr_complex, options.from_file)
        else:
            sys.stderr.write("No source defined, pulling samples from null source.\n\n")
            self.source = gr.null_source(gr.sizeof_gr_complex)

        # Set up receive path
        # do this after for any adjustments to the options that may
        # occur in the sinks (specifically the UHD sink)
        self.rxpath = receive_path(demodulator, rx_callback, options) 

        self.connect(self.source, self.rxpath)


# /////////////////////////////////////////////////////////////////////////////
#                                   main
# /////////////////////////////////////////////////////////////////////////////

global n_rcvd, n_right

def main():
    global n_rcvd, n_right, start_time, stop_rcv
    
    TIMEOUT = 60 # 60sec for hurdle 2
    n_rcvd = 0
    n_right = 0
    #start_time = 0
    mstr_cnt = 0
    #stop_rcv = 0   

    def rx_callback(ok, payload):
        global n_rcvd, n_right, start_time, stop_rcv
        n_rcvd += 1
        if len(payload) < 10:
           return
        (pktno,crc,sn) = struct.unpack('!HLL', payload[0:10])
        if ok:
            n_right += 1
            try:            
               data = s.recv(4) # if a ready packet is received
               s.send(payload[2:])
            except socket.error as e:
               print "Socket error: %s" %e.strerror
               #stop_rcv = 1
               return
            if data.__len__() == 0:
               print "Connection closed"
               #stop_rcv = 1
               return
            #if n_right == 1:
              # start_time = time.time()
            #if n_right == 2000:
             #  t = time.time() - start_time              
              # print"Mod : %5s, Rate : %8d, Time for 2000 pkts : %f sec\n" %(options.modulation, options.bitrate, t)
               #stop_rcv = 1;
            
        if options.verbose:
              print "ok = %5s  pktno = %4d  n_rcvd = %4d  n_right = %4d" %(
              ok, pktno, n_rcvd, n_right)
       

    demods = digital.modulation_utils.type_1_demods()

    # Create Options Parser:
    parser = OptionParser (option_class=eng_option, conflict_handler="resolve")
    expert_grp = parser.add_option_group("Expert")

    parser.add_option("-m", "--modulation", type="choice", choices=demods.keys(), 
                      default='psk',
                      help="Select modulation from: %s [default=%%default]"
                            % (', '.join(demods.keys()),))      
    parser.add_option("-s", "--server", type="string", default='idb2',
                      help="server hosting the packet server/sink")
    parser.add_option("-o", "--port", type="int", default='5125',
                      help="packet sink tcp port")
    parser.add_option("-i", "--timeout", type="int", default='300',help='receive timeout(sec)')
    parser.add_option("","--from-file", default=None,
                      help="input file of samples to demod")

    receive_path.add_options(parser, expert_grp)
    uhd_receiver.add_options(parser)

    for mod in demods.values():
        mod.add_options(expert_grp)

    (options, args) = parser.parse_args ()

    if len(args) != 0:
        parser.print_help(sys.stderr)
        sys.exit(1)

    if options.from_file is None:
        if options.rx_freq is None:
            sys.stderr.write("You must specify -f FREQ or --freq FREQ\n")
            parser.print_help(sys.stderr)
            sys.exit(1)

    
    # connect to the packet sink
    TCP_IP=options.server
    TCP_PORT=options.port
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try: 
       s.connect((TCP_IP, TCP_PORT))
    except socket.error as e:
       print "Error connecting to the packet sink: %s" %e.strerror
       return

    # build the graph
    tb = my_top_block(demods[options.modulation], rx_callback, options)

    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: Failed to enable realtime scheduling."

    tb.start()
    tb.wait()         # wait for it to finish
    
    #while mstr_cnt < options.timeout*1000:
     #  if stop_rcv == 1:
      #    break;
     #  mstr_cnt = mstr_cnt + 1
    #   time.sleep(0.001)

    #if stop_rcv == 0:
    #   print "Receiver timed out, received %d packets successfully in %d sec" %(n_right, TIMEOUT)

    s.close()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
