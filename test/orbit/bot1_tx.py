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

# from custom blocks
from correlator_cc import correlator_cc_swig as correlator_cc
from spreader import spreader_swig as spreader
from print_bb import print_bb_swig as print_bb
from rscoding_bb import rscoding_bb_swig as rscoding_bb
from crc import crc_swig as crc
from packetizer import packetizer_swig as packetizer

# from current dir
from uhd_interface import uhd_transmitter

import time, struct, sys, socket, random

#import os 
#print os.getpid()
#raw_input('Attach and press enter')

class my_top_block(gr.top_block):
    def __init__(self, options):
        gr.top_block.__init__(self)

        if(options.tx_freq is not None):
            symbol_rate = 500000
	    self.sink = uhd_transmitter(options.args, symbol_rate,
					2,
					options.tx_freq, 30,
					options.spec, "TX/RX",
					1)
	    options.samples_per_symbol = self.sink._sps
        elif(options.to_file is not None):
            sys.stderr.write(("Saving samples to '%s'.\n\n" % (options.to_file)))
            self.sink = gr.file_sink(gr.sizeof_gr_complex, options.to_file)
        else:
            sys.stderr.write("No sink defined, dumping samples to null sink.\n\n")
            self.sink = gr.null_sink(gr.sizeof_gr_complex)

        # Packetizer
	self.packettx = packetizer.packet_source()

        # CRC TX block
        self.crctx = crc.crctx(0)

        # RS encoding
        self.encoder = rscoding_bb.encode_bb()

        # Spreader
        self.spreader = spreader.spreader_bc()

	# Preamble insert block
        self.inserter = correlator_cc.preamble_insert_cc()

        # Printer block
        self.printer = print_bb.print_bb()

        ntaps = 11 * int(options.samples_per_symbol)
        self.rrc_taps = gr.firdes.root_raised_cosine( 
                      1.0,
                      1000000,
                      symbol_rate,
                      1.0,
                      ntaps)
        self.rrc_filter = gr.interp_fir_filter_ccf(1, self.rrc_taps)


	# Connect the blocks
        #self.connect(self.source, self.inserter)
        #self.connect(self.inserter, self.sink)
        #self.connect(self.source, self.spreader)
        #self.connect(TX_source_data, self.crctx)
        self.connect(self.packettx, self.crctx)
        #self.connect(self.crctx, self.printer)
        #self.connect(self.printer, self.encoder)
        self.connect(self.crctx, self.encoder)
        self.connect(self.encoder, self.spreader)
        #self.connect(self.source, self.spreader)
        self.connect(self.spreader, self.inserter)
        #self.connect(self.inserter, self.rrc_filter)
        #self.connect(self.rrc_filter, self.sink)
        self.connect(self.inserter, self.sink)
  


# /////////////////////////////////////////////////////////////////////////////
#                                   main
# /////////////////////////////////////////////////////////////////////////////

def main():

    parser = OptionParser(option_class=eng_option, conflict_handler="resolve")
    expert_grp = parser.add_option_group("Expert")

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

    uhd_transmitter.add_options(parser)

    (options, args) = parser.parse_args ()

    if len(args) != 0:
        parser.print_help()
        sys.exit(1)
           

    # build the graph
    tb = my_top_block(options)

    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: failed to enable realtime scheduling"

    tb.start()                       # start flow graph
        
    tb.wait()                       # wait for it to finish

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
