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
from print_cc import print_cc_swig as print_cc

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
                                   options.spec, "RX2",
                                   options.verbose)
        options.samples_per_symbol = self.source._sps

	# Correlator block
        self.correlator = correlator_cc.correlator_cc()

	# Print block
	self.printer = print_cc.print_cc()

	# NULL sink block
        self.nullsink = gr.null_sink(gr.sizeof_gr_complex)
 
        # RRC filter
        nfilts = 32 
        ntaps = nfilts * 11 * int(options.samples_per_symbol)
        self.rrc_taps = gr.firdes.root_raised_cosine(
            nfilts, # gain
            nfilts, # sampling rate based on 32 fliters in resampler
            1.0,    # symbol rate
            1.0,    # excess bandwidth or roll-off factor
            ntaps)
        self.rrc_filter = gr.pfb_arb_resampler_ccf(options.samples_per_symbol, 
                                                   self.rrc_taps)

	# Connect the blocks
        #self.connect(self.source, self.correlator)
        self.connect(self.source, self.rrc_filter)
        self.connect(self.rrc_filter, self.correlator)
        self.connect(self.correlator, self.printer)
        self.connect(self.printer, self.nullsink)
       

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
