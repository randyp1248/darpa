#!/usr/bin/env python

from gnuradio import gr, gru
from gnuradio import eng_notation
from gnuradio.eng_option import eng_option
from optparse import OptionParser

# From gr-digital
from gnuradio import digital

# from current dir
from uhd_interface import uhd_receiver

from correlator_cc import correlator_cc_swig as correlator_cc

import struct
import sys
import time

#import os
#print os.getpid()
#raw_input('Attach and press enter: ')

class my_top_block(gr.top_block):
    def __init__(self, options):
        gr.top_block.__init__(self)

        symbol_rate = 500000

        self.source = uhd_receiver(options.args, symbol_rate,
                                   2,
                                   options.rx_freq, 30,
                                   options.spec, options.antenna,
                                   options.verbose)
        options.samples_per_symbol = self.source._sps

        # Set up receive path
        # do this after for any adjustments to the options that may
        # occur in the sinks (specifically the UHD sink)
        
        self.correlator = correlator_cc.correlator_cc()

        self.sink = gr.vector_sink_c()
        self.file_sink = gr.file_sink(gr.sizeof_gr_complex, "out")

        self.connect(self.source, self.correlator)
        self.connect(self.correlator, self.file_sink)
        #self.connect(self.source, self.sink)
   
        #self.connect(self.source, self.file_sink)

# /////////////////////////////////////////////////////////////////////////////
#                                   main
# /////////////////////////////////////////////////////////////////////////////

def main():

    # Create Options Parser:
    parser = OptionParser (option_class=eng_option, conflict_handler="resolve")
    expert_grp = parser.add_option_group("Expert")

    uhd_receiver.add_options(parser)

    (options, args) = parser.parse_args ()

    if len(args) != 0:
        parser.print_help(sys.stderr)
        sys.exit(1)

    # build the graph
    tb = my_top_block(options)

    r = gr.enable_realtime_scheduling()
    if r != gr.RT_OK:
        print "Warning: Failed to enable realtime scheduling."

    tb.start()        # start flow graph
    tb.wait()         # wait for it to finish

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
