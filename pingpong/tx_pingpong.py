#!/usr/bin/env python

from gnuradio import gr
from gnuradio import eng_notation
from gnuradio.eng_option import eng_option
from optparse import OptionParser

# From gr-digital
from gnuradio import digital
from gr-correlator_cc import preamble_insert_cc, correlator_cc

# from current dir
from uhd_interface import uhd_transmitter

import time, struct, sys

#import os 
#print os.getpid()
#raw_input('Attach and press enter')

class my_top_block(gr.top_block):
    def __init__(self, options):
        gr.top_block.__init__(self)

        # some arbitrary data
        data_capsule = ( (+1+1j), (+1+1j), (-1-1j), (-1-1j),
                         (-1+1j), (-1+1j), (+1-1j), (+1-1j) )

        # Work-around to get the modulation's bits_per_symbol
        symbol_rate = options.bitrate

        self.sink = uhd_transmitter(options.args, symbol_rate,
                                    options.samples_per_symbol,
                                    options.tx_freq, options.tx_gain,
                                    options.spec, options.antenna,
                                    options.verbose)

        options.samples_per_symbol = self.sink._sps

        self.source = gr.vector_source_c(data_capsule)
        self.inserter = correlator_cc.preamble_insert_cc()
        self.connect(self.source, self.inserter)
        self.connect(self.inserter, self.sink)

# /////////////////////////////////////////////////////////////////////////////
#                                   main
# /////////////////////////////////////////////////////////////////////////////

def main():

    parser = OptionParser(option_class=eng_option, conflict_handler="resolve")
    expert_grp = parser.add_option_group("Expert")

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
