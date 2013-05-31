#!/usr/bin/env python

from gnuradio import gr, gru
from gnuradio import eng_notation
from gnuradio.eng_option import eng_option
from optparse import OptionParser

# From gr-digital
from gnuradio import digital

# from current dir
from uhd_interface import uhd_receiver
from uhd_interface import uhd_transmitter

from correlator_cc import correlator_cc_swig as correlator_cc
from spreader import spreader_swig as spreader
from print_bb import print_bb_swig as print_bb
from rscoding_bb import rscoding_bb_swig as rscoding_bb
from crc import crc_swig as crc

import struct
import sys
import time

#import os
#print os.getpid()
#raw_input('Attach and press enter: ')

class my_top_block(gr.top_block):
    def __init__(self, options):
        gr.top_block.__init__(self)

        # Source block
        symbol_rate = 500000
        self.source = uhd_receiver(options.args, symbol_rate,
                                   2,
                                   options.rx_freq, 30,
                                   #options.spec, "RX2",
                                   options.spec, "TX/RX",
                                   options.verbose)
        options.samples_per_symbol = self.source._sps

        self.filesink = gr.file_sink(1, "output_file")

	# Correlator block
        self.correlator = correlator_cc.correlator_cc()

	# Despreader block
	self.despreader = spreader.despreader_cb()
  
        # RS Decoder block
	self.decoder = rscoding_bb.decode_bb()

        # CRC RX Block
        self.crcrx = crc.crcrx()

	# Print block
	self.printer = print_bb.print_bb()

	# NULL sink block
        #self.nullsink = gr.null_sink(1)
 
        # RRC filter
        #nfilts = 32 
        #ntaps = nfilts * 11 * int(options.samples_per_symbol)
        #self.rrc_taps = gr.firdes.root_raised_cosine(
            #nfilts, # gain
            #nfilts, # sampling rate based on 32 fliters in resampler
            #1.0,    # symbol rate
            #1.0,    # excess bandwidth or roll-off factor
            #ntaps)
        #self.rrc_filter = gr.pfb_arb_resampler_ccf(options.samples_per_symbol, 
                                                   #self.rrc_taps)

	# Connect the blocks
        #self.connect(self.source, self.correlator)
        #self.connect(self.source, self.rrc_filter)
        #self.connect(self.rrc_filter, self.correlator)
        self.connect(self.source, self.correlator)
        self.connect(self.correlator, self.despreader)
        self.connect(self.despreader, self.decoder)
        self.connect(self.decoder, self.crcrx)
        #self.connect(self.crcrx, self.printer)
        #self.connect(self.despreader, self.printer)
        #self.connect(self.printer, self.nullsink)
        #self.connect(self.printer, self.filesink)
        self.connect(self.crcrx, self.filesink)
       

# /////////////////////////////////////////////////////////////////////////////
#                                   main
# /////////////////////////////////////////////////////////////////////////////

def main():

    # Create Options Parser:
    parser = OptionParser (option_class=eng_option, conflict_handler="resolve")
    expert_grp = parser.add_option_group("Expert")

    uhd_receiver.add_options(parser)
    uhd_transmitter.add_options(parser)

    (options, args) = parser.parse_args ()

    if len(args) != 0:
        parser.print_help(sys.stderr)
        sys.exit(1)

    # build the graph
    tb = my_top_block(options)

    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: Failed to enable realtime scheduling."

    tb.run()
    #tb.start()        # start flow graph
    #tb.wait()         # wait for it to finish

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
