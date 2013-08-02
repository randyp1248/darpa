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
from uhd_interface import uhd_receiver

# from custom blocks
from correlator_cc import correlator_cc_swig as correlator_cc
from spreader import spreader_swig as spreader
from print_bb import print_bb_swig as print_bb
from rscoding_bb import rscoding_bb_swig as rscoding_bb
from crc import crc_swig as crc
from packetizer import packetizer_swig as packetizer

import struct
import sys, socket

#import os
#print os.getpid()
#raw_input('Attach and press enter: ')

class my_top_block(gr.top_block):
    def __init__(self, options):
        gr.top_block.__init__(self)

        if(options.rx_freq is not None):
	    symbol_rate = 500000
	    self.source = uhd_receiver(options.args, symbol_rate,
				       2,
				       options.rx_freq, 30,
				       #options.spec, "RX2",
				       options.spec, "TX/RX",
				       1) #options.verbose)
	    options.samples_per_symbol = self.source._sps

        elif(options.from_file is not None):
            sys.stderr.write(("Reading samples from '%s'.\n\n" % (options.from_file)))
            self.source = gr.file_source(gr.sizeof_gr_complex, options.from_file)
        else:
            sys.stderr.write("No source defined, pulling samples from null source.\n\n")
            self.source = gr.null_source(gr.sizeof_gr_complex)

        # Set up receive path
	# Correlator block
        self.correlator = correlator_cc.correlator_cc()

	# Despreader block
	self.despreader = spreader.despreader_cb()
  
        # RS Decoder block
	self.decoder = rscoding_bb.decode_bb()

        # CRC RX Block
        self.crcrx = crc.crcrx()

	# Packetizer receiver
	self.packetrx = packetizer.packet_sink()

	# Print block
	self.printer = print_bb.print_bb()

	# NULL sink block
        self.nullsink = gr.null_sink(1)
 
        # RRC filter
        #nfilts = 32 
        #ntaps = nfilts * 11 * int(options.samples_per_symbol)
        #self.rrc_taps = gr.firdes.root_raised_cosine(
            #1.0, # gain
            #1000000, # sampling rate based on 32 fliters in resampler
            #symbol_rate,    # symbol rate
            #1.0,    # excess bandwidth or roll-off factor
            #ntaps)
        #self.rrc_filter = gr.pfb_arb_resampler_ccf(options.samples_per_symbol, 
        #                                           self.rrc_taps)


        # Design filter to get actual channel we want
        sw_decim = 1
        chan_coeffs = gr.firdes.low_pass (1.0,                  # gain
                                          #sw_decim * self.samples_per_symbol(), # sampling rate
                                          #1000000, # sampling rate
                                          2,
                                          #self._chbw_factor,    # midpoint of trans. band
                                          1,    # midpoint of trans. band
                                          0.5,                  # width of trans. band
                                          gr.firdes.WIN_HANN)   # filter type
        self.channel_filter = gr.fft_filter_ccc(sw_decim, chan_coeffs)


	# Connect the blocks
        #self.connect(self.source, self.correlator)
        #self.connect(self.source, self.rrc_filter)
        #self.connect(self.rrc_filter, self.correlator)

        # connect block input to channel filter
	self.connect(self.source, self.channel_filter)
        self.connect(self.channel_filter, self.correlator)


        #self.connect(self.source, self.correlator)
        self.connect(self.correlator, self.despreader)
        self.connect(self.despreader, self.decoder)
        self.connect(self.decoder, self.crcrx)
        #self.connect(self.decoder, self.printer)
        #self.connect(self.printer, self.crcrx)
        #self.connect(self.despreader, self.printer)
        #self.connect(self.printer, self.nullsink)
        #self.connect(self.printer, self.filesink)
        #self.connect(self.crcrx, self.filesink)
        self.connect(self.crcrx, self.packetrx) 


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

    # Create Options Parser:
    parser = OptionParser (option_class=eng_option, conflict_handler="resolve")
    expert_grp = parser.add_option_group("Expert")

    parser.add_option("-s", "--server", type="string", default='idb2',
                      help="server hosting the packet server/sink")
    parser.add_option("-o", "--port", type="int", default='5125',
                      help="packet sink tcp port")
    parser.add_option("-i", "--timeout", type="int", default='300',help='receive timeout(sec)')
    parser.add_option("","--from-file", default=None,
                      help="input file of samples to demod")

    uhd_receiver.add_options(parser)

    (options, args) = parser.parse_args ()

    if len(args) != 0:
        parser.print_help(sys.stderr)
        sys.exit(1)

    if options.from_file is None:
        if options.rx_freq is None:
            sys.stderr.write("You must specify -f FREQ or --freq FREQ\n")
            parser.print_help(sys.stderr)
            sys.exit(1)

    
    # build the graph
    tb = my_top_block(options)

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

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
