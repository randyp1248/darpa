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

from gnuradio import gr, gr_unittest
import rscoding_bb_swig as rscoding_bb
import random
import copy

class qa_encode_bb (gr_unittest.TestCase):

    def setUp (self):
        self.tb1 = gr.top_block ()
        self.tb2 = gr.top_block ()

    def tearDown (self):
        self.tb1 = None
        self.tb2 = None


    def test_001_t (self):
        NN = 255
        NROOTS = 32
        #NN = 63
        #NROOTS = 8
        expected_data = []
        for i in range((NN - NROOTS)):
            expected_data.append(random.randint(1,100))

        source1 = gr.vector_source_b(expected_data)
        encode = rscoding_bb.encode_bb()
        sink1 = gr.vector_sink_b()
        
        self.tb1.connect(source1, encode)
        self.tb1.connect(encode, sink1)

        # set up fg
        self.tb1.run ()

        source2 = gr.vector_source_b(sink1.data())
        decode = rscoding_bb.decode_bb()
        sink2 = gr.vector_sink_b()
        self.tb2.connect(source2, decode)
        self.tb2.connect(decode, sink2)
   
        # set up fg
        self.tb2.run ()

        result_data = sink2.data()

        print "Expected Data"
        print tuple(expected_data)
        print "Length of expected Data"
        print len(expected_data)

        print "Encoded Data"
        print (sink1.data())
        print "Length of Encoded Data"
        print len(sink1.data())

        print "Decoded Data"
        print (result_data)
        print "Length of Decoded Data"
        print len(result_data)

        self.assertEqual(tuple(expected_data), tuple(result_data))

        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_encode_bb, "qa_encode_bb.xml")
