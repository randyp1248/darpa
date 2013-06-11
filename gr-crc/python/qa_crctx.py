#!/usr/bin/env python
# 
# Copyright 2013 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 
# ********************************************************************
# This code was generated using gr-modtool and modified by 
# Dave Kier to incorporate a FRAME SIZE and CRC algorithm
# June 7, 2013
#*********************************************************************
from gnuradio import gr, gr_unittest
import crc_swig as crc
import os

class qa_crctx (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t_crctx(self):
	# *** FILES USED FOR TESTING *** 	
	TX_input_file = "TX_Input_Test"
	_TX_input_file = 'TX_Input_Test'

	TX_output_file = "TX_output_test_file"
	_TX_output_file = 'TX_output_test_file'

	RX_input_file = "TX_output_test_file"
	_RX_input_file = 'TX_output_test_file'

	RX_output_file = "RX_output_test_file"
	_RX_output_file = 'RX_output_test_file'
        
	# *** Start Python Implementation ***
	print "Starting Python test 001"
	TX_source_data = gr.file_source(gr.sizeof_char, TX_input_file) 		#located in ../build/python
	#RX_source_data = gr.file_source(gr.sizeof_char, RX_input_file) 		#located in ../build/python

	#TX_file_sink = gr.file_sink(gr.sizeof_char, TX_output_file)
	TX_Input_filesize  = os.stat(_TX_input_file).st_size
	#TX_output_filesize = os.stat(_TX_output_file).st_size
	
	RX_file_sink = gr.file_sink(gr.sizeof_char, RX_output_file)
        #RX_Input_filesize  = os.stat(_RX_input_file).st_size
	RX_output_filesize = os.stat(_RX_output_file).st_size
	
	
	# *** Call C++ Blocks ***
	append_crc   = crc.crctx(TX_Input_filesize)
	remove_crc = crc.crcrx()

	# *** Connect the Blocks ***
	self.tb.connect(TX_source_data, append_crc)
	self.tb.connect(append_crc, remove_crc)
	self.tb.connect(remove_crc, RX_file_sink)
	self.tb.run()	

if __name__ == '__main__':
    gr_unittest.run(qa_crctx, "qa_crctx.xml")
