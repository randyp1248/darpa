#!/usr/bin/env python

from gnuradio import gr
from gnuradio import eng_notation
from gnuradio.eng_option import eng_option
from optparse import OptionParser

# From gr-digital
from gnuradio import digital
from correlator_cc import correlator_cc_swig as correlator_cc
from spreader import spreader_swig as spreader
from rscoding_bb import rscoding_bb_swig as rscoding_bb
from crc import crc_swig as crc

# from current dir
from uhd_interface import uhd_transmitter
from uhd_interface import uhd_receiver

import time, struct, sys

#import os 
#print os.getpid()
#raw_input('Attach and press enter')

class my_top_block(gr.top_block):
    def __init__(self, options):
        gr.top_block.__init__(self)

        # Source block
        #frame = ((+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j))
       
        zeros = (0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0)
        frame_pad = zeros + zeros
        frame_pad = frame_pad + frame_pad 
        frame_pad = frame_pad + frame_pad 
        frame_pad = frame_pad + frame_pad 
        # 160
        frame_pad = frame_pad + zeros + zeros + zeros + zeros + zeros + zeros 
        frame_pad = frame_pad + (0x0, 0x0, 0x0)

        # 12
        data = (0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf, 0xd, 0x0, 0x0, 0xd)
        # 40
        data = data + data + data + (0xd, 0xe, 0xa, 0xd)

        ten = (0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf, 0xd, 0x0)
 
        # 223
	data = data + data + data + data + data + ten + ten + (0xd, 0xe, 0xa)

        src = frame_pad + data + data + data + data + frame_pad

        print len(src)
  
	#data = data + data + data + data
        
	#data = data + data + data + data + data + data + data + data + data + data + data + data + data + data
        #data = (0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf, 0xd, 0x0)
        self.source = gr.vector_source_b(src)

        # CRC TX block
        self.crctx = crc.crctx()

        # RS encoding
        self.encoder = rscoding_bb.encode_bb()

        # Spreader
        self.spreader = spreader.spreader_bc()

	# Preamble insert block
        self.inserter = correlator_cc.preamble_insert_cc()

	# Sink block
        symbol_rate = 500000 # Work-around to get the modulation's bits_per_symbol
        self.sink = uhd_transmitter(options.args, symbol_rate,
                                    2,
                                    options.tx_freq, 30,
                                    options.spec, "TX/RX",
                                    options.verbose)
        options.samples_per_symbol = self.sink._sps


	# Connect the blocks
        #self.connect(self.source, self.inserter)
        #self.connect(self.inserter, self.sink)
        #self.connect(self.source, self.spreader)
        self.connect(self.source, self.crctx)
        self.connect(self.crctx, self.encoder)
        self.connect(self.encoder, self.spreader)
        #self.connect(self.source, self.spreader)
        self.connect(self.spreader, self.inserter)
        self.connect(self.inserter, self.sink)
  
# /////////////////////////////////////////////////////////////////////////////
#                                   main
# /////////////////////////////////////////////////////////////////////////////

def main():

    parser = OptionParser(option_class=eng_option, conflict_handler="resolve")
    expert_grp = parser.add_option_group("Expert")

    uhd_transmitter.add_options(parser)
    uhd_receiver.add_options(parser)

    (options, args) = parser.parse_args ()

    if len(args) != 0:
        parser.print_help()
        sys.exit(1)
           
    # build the graph
    tb = my_top_block(options)

    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: failed to enable realtime scheduling"

    tb.run()
    #tb.start()                       # start flow graph
    #tb.wait()                       # wait for it to finish

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
