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
# May-24-2013
#*********************************************************************
from gnuradio import gr, gr_unittest
import crc_swig as crc

class qa_crctx (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None
    
    #result_data = ()

    def test_001_t_crctx(self):
	
        print "Starting Python test 001"
	
	source_data1 = gr.file_source(gr.sizeof_char, "input_test_file2") 	#located in ../build/python

	expected_result1 = 65, 32, 66, 32, 67, 32, 68, 32, 69, 32, 49, 50, 51, 52, 70, 32, 71, 32, 72, 32, 73, 32, 74, 32, 49, 50, 51, 52, 75, 32, 76, 32, 77, 32, 78, 32, 79, 32, 49, 50, 51, 52, 80, 32, 81, 32, 82, 32, 83, 32, 84, 32, 49, 50, 51, 52, 85, 32, 86, 32, 87, 32, 88, 32, 89, 32, 49, 50, 51, 52, 90, 32, 48, 32, 49, 32, 50, 32, 51, 32, 49, 50, 51, 52, 52, 32, 53, 32, 54, 32, 55, 32, 56, 32, 49, 50, 51, 52, 57, 32, 65, 32, 66, 32, 67, 32, 68, 10, 49, 50, 51, 52
	
	expected_result2 = 65, 32, 66, 32, 67, 32, 68, 32, 69, 32, 70, 32, 71, 32, 72, 32, 73, 32, 74, 32, 75, 32, 76, 32, 77, 32, 78, 32, 79, 32, 80, 32, 81, 32, 82, 32, 83, 32, 84, 32, 85, 32, 86, 32, 87, 32, 88, 32, 89, 32, 90, 32, 48, 32, 49, 32, 50, 32, 51, 32, 52, 32, 53, 32, 54, 32, 55, 32, 56, 32, 57, 32, 65, 32, 66, 32, 67, 32, 68, 10 

	calc_crc = crc.crctx()
	remove_crc = crc.crcrx()	
	#dst = gr.vector_sink_b()
	file_sink = gr.file_sink(gr.sizeof_char, "output_test_file2")
	self.tb.connect(source_data1, calc_crc)
	self.tb.connect(calc_crc, remove_crc)
	#self.tb.connect(remove_crc, dst)
	self.tb.connect(remove_crc, file_sink)

	self.tb.run()	
	#result_data = dst.data()

	#print "********** Result2 is = ", result_data, " *********"  
	#self.assertEqual(expected_result2, result_data)
	#print "Ending Python Test 001"

if __name__ == '__main__':
    gr_unittest.run(qa_crctx, "qa_crctx.xml")
