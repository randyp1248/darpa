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
from print_bb import print_bb_swig as print_bb
from crc import crc_swig as crc

# from current dir
from uhd_interface import uhd_transmitter
from uhd_interface import uhd_receiver

import time, struct, sys
import os

#import os 
#print os.getpid()
#raw_input('Attach and press enter')

class my_top_block(gr.top_block):
    def __init__(self, options):
        gr.top_block.__init__(self)
 

        #Input Data File
        TX_input_file = "TX_Input_Data"
        _TX_input_file = 'TX_Input_Data'
        #TX_input_file = "Ding_Dong2.wma"
        #_TX_input_file = 'Ding_Dong2.wma'
        TX_source_data = gr.file_source(gr.sizeof_char, TX_input_file)
        TX_Input_filesize  = os.stat(_TX_input_file).st_size

        #RX_output_file = "RX_output_test_file"
	#_RX_output_file = 'RX_output_test_file'


        #RX_file_sink = gr.file_sink(gr.sizeof_char, RX_output_file)
	#RX_output_filesize = os.stat(_RX_output_file).st_size

        # Source block
        #frame = ((+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j))
       
        zeros10 = (0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0)
        zeros20 = zeros10 + zeros10
        zeros40 = zeros20 + zeros20

        frame_pad223 = zeros40+zeros40+zeros40+zeros40+zeros40+zeros20+(0x0,0x0,0x0)
        frame_pad219 = zeros40+zeros40+zeros40+zeros40+zeros40+zeros10+(0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0)
        frame_pad215 = zeros40+zeros40+zeros40+zeros40+zeros40+zeros10+(0x0,0x0,0x0,0x0,0x0)
        frame_pad183 = zeros40+zeros40+zeros40+zeros40+zeros20+(0x0,0x0,0x0)
        frame_pad179 = zeros40+zeros40+zeros40+zeros40+zeros10+(0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0)

        data12 = (0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf, 0xd, 0x0, 0x0, 0xd)
        data40 = data12 + data12 + data12 + (0xd, 0xe, 0xa, 0xd)
        data10 = (0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9)
 
        data223 = data40+data40+data40+data40+data40+data10+data10+(0xa,0xb,0xc)
        data219 = data40+data40+data40+data40+data40+data10+(0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8)
        data183 = data40+data40+data40+data40+data10+data10+(0x0,0x1,0x2)
 

        #src = frame_pad + data + data + data + data + frame_pad
        #src = frame_pad + frame_pad + data + data + data + data + data + data + data + data + data + data+ data + frame_pad + frame_pad
   
        #src = frame_pad215+data219+frame_pad219
       
        # 10 frames 
        #src = frame_pad215+frame_pad219+data219+data219+data219+data219+data219+data219+frame_pad219+frame_pad219
 
        # 20 frames
        #src = frame_pad215+frame_pad219+data219+data219+data219+data219+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+frame_pad219+frame_pad219
        
        # 8 frames
        #src = frame_pad215+frame_pad219+data219+data219+data219+data219+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+data219+data219
        #src = src+frame_pad219+frame_pad219
        #50 frames

        # 8 frames
        src = frame_pad179+frame_pad183+data183+data183+data183+data183+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        #src = src+data183+data183
        src = src+frame_pad183+frame_pad183
        #50 frames

        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src

        #print len(src)
        #print len(data)
  
	#data = data + data + data + data
        
	#data = data + data + data + data + data + data + data + data + data + data + data + data + data + data
        #data = (0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf, 0xd, 0x0)
        #self.source = gr.vector_source_b(src)
        #self.source = gr.vector_source_b(frame_pad)

        # CRC TX block
        #self.crctx = crc.crctx()

        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src

        #print len(src)
        #print len(data)
  
	#data = data + data + data + data
        
	#data = data + data + data + data + data + data + data + data + data + data + data + data + data + data
        #data = (0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf, 0xd, 0x0)
        #self.source = gr.vector_source_b(src)
        #self.source = gr.vector_source_b(frame_pad)

        # CRC TX block
        #self.crctx = crc.crctx()
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src
        #src = src + src

        #print len(src)
        #print len(data)
  
	#data = data + data + data + data
        
	#data = data + data + data + data + data + data + data + data + data + data + data + data + data + data
        #data = (0xd, 0xe, 0xa, 0xd, 0xb, 0xe, 0xe, 0xf, 0xd, 0x0)
        #self.source = gr.vector_source_b(src)
        #self.source = gr.vector_source_b(frame_pad)

        # CRC TX block
        self.crctx = crc.crctx(TX_Input_filesize)

        # RS encoding
        self.encoder = rscoding_bb.encode_bb()

        # Spreader
        self.spreader = spreader.spreader_bc()

	# Preamble insert block
        self.inserter = correlator_cc.preamble_insert_cc()

        # Printer block
        self.printer = print_bb.print_bb()

	# Sink block
        symbol_rate = 500000 # Work-around to get the modulation's bits_per_symbol
        self.sink = uhd_transmitter(options.args, symbol_rate,
                                    2,
                                    options.tx_freq, 30,
                                    options.spec, "TX/RX",
				    1)
                                    #options.verbose)
        options.samples_per_symbol = self.sink._sps


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
        self.connect(TX_source_data, self.crctx)
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
