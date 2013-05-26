#!/usr/bin/env python
# 
#  Copyright 2013 TRITONS
#
#  <<<=================================================================>>>
#  <<<=================================================================>>>
#  <<< This file is autogenerated from pngen.pl.  Do not edit directly >>>
#  <<<=================================================================>>>
#  <<<=================================================================>>>
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
import correlator_cc_swig as correlator_cc

class qa_preamble_insert_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.tmitFirstFrame[:] = []
        self.tmitSecondFrame[:] = []
        prev = self.pnSequence[len(self.pnSequence)-1]
        for x in range(len(self.firstFrame)):
	    prev = prev * self.firstFrame[x]
	    self.tmitFirstFrame.append(prev)
        prev = self.pnSequence[len(self.pnSequence)-1]
        for x in range(len(self.secondFrame)):
	    prev = prev * self.secondFrame[x]
	    self.tmitSecondFrame.append(prev)

    def tearDown (self):
        self.tb = None

    firstFrame = ((+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j))
    tmitFirstFrame = []
    pnSequence = (                           (+0-1j),(+0-1j),(+0-1j),(+0-1j),(+0-1j),(+0-1j),(+0-1j),(+0-1j),
                           (+1+0j),(+1+0j),(+1+0j),(+0+1j),(+0-1j),(+1+0j),(-1+0j),(-1+0j),
                           (+0-1j),(-1+0j),(+0+1j),(+1+0j),(+0+1j),(-1+0j),(+0-1j),(+1+0j),
                           (-1+0j),(+1+0j),(+1+0j),(+0+1j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),
                           (+0-1j),(+0+1j),(+1+0j),(+1+0j),(+0-1j),(+0-1j),(-1+0j),(+1+0j),
                           (+1+0j),(+1+0j),(-1+0j),(+0+1j),(+0+1j),(+0-1j),(+0+1j),(+0-1j),
                           (-1+0j),(+0-1j),(+0+1j),(+0+1j),(+1+0j),(+0+1j),(+0+1j),(+0+1j),
                           (+0-1j),(+0-1j),(+0+1j),(+1+0j),(+0-1j),(+1+0j),(+0+1j),(-1+0j),
                           (+1+0j),(+0+1j),(-1+0j),(+0-1j),(+0+1j),(-1+0j),(+0-1j),(-1+0j),
                           (+0+1j),(+1+0j),(+0-1j),(+0+1j),(+1+0j),(+1+0j),(+0+1j),(+0+1j),
                           (+0-1j),(+1+0j),(-1+0j),(+0+1j),(-1+0j),(-1+0j),(+0+1j),(+0-1j),
                           (-1+0j),(+1+0j),(+0-1j),(+1+0j),(-1+0j),(-1+0j),(+0+1j),(+1+0j),
                           (+0-1j),(+0+1j),(+0-1j),(-1+0j),(+1+0j),(+1+0j),(+0+1j),(+1+0j),
                           (-1+0j),(-1+0j),(+0-1j),(+0-1j),(+0-1j),(+0+1j),(-1+0j),(-1+0j),
                           (+0+1j),(-1+0j),(+0-1j),(+0-1j),(+0-1j),(+0+1j),(-1+0j),(+1+0j),
                           (-1+0j),(-1+0j),(+0-1j),(+1+0j),(+0-1j),(+1+0j),(+1+0j),(+1+0j),
                           (+0-1j),(+0+1j),(+1+0j),(+0+1j),(+1+0j),(+0-1j),(+0-1j),(+0+1j),
                           (+0-1j),(+0-1j),(+1+0j),(+1+0j),(+0+1j),(+0-1j),(+0-1j),(+0-1j),
                           (-1+0j),(+0+1j),(+0+1j),(-1+0j),(-1+0j),(+0-1j),(+1+0j),(+0+1j),
                           (-1+0j),(+0-1j),(+0+1j),(+0+1j),(+1+0j),(+0-1j),(+1+0j),(+0-1j),
                           (+0-1j),(+1+0j),(+0-1j),(+0+1j),(+0+1j),(-1+0j),(+1+0j),(+0+1j),
                           (+0+1j),(-1+0j),(+0+1j),(-1+0j),(+1+0j),(+0+1j),(+0-1j),(+1+0j),
                           (+0-1j),(+0+1j),(-1+0j),(+0+1j),(+1+0j),(-1+0j),(-1+0j),(+0-1j),
                           (+1+0j),(-1+0j),(+0-1j),(+0-1j),(+0+1j),(-1+0j),(+0-1j),(+0+1j),
                           (+1+0j),(+0-1j),(+0-1j),(-1+0j),(-1+0j),(+1+0j),(-1+0j),(+0-1j),
                           (+0-1j),(+0-1j),(-1+0j),(+0-1j),(+1+0j),(-1+0j),(+0+1j),(+1+0j),
                           (-1+0j),(+0-1j),(+1+0j),(+1+0j),(-1+0j),(-1+0j),(+1+0j),(+0-1j),
                           (+1+0j),(+0-1j),(+0+1j),(+1+0j),(+1+0j),(-1+0j),(+0-1j),(+0+1j),
                           (+0+1j),(+1+0j),(+1+0j),(+1+0j),(-1+0j),(-1+0j),(-1+0j),(+0+1j),
                           (+0-1j),(+1+0j),(+0-1j),(+0+1j),(+0-1j),(+0+1j),(-1+0j),(+0-1j),
                           (-1+0j),(-1+0j),(-1+0j),(+1+0j),(+1+0j),(-1+0j),(+1+0j),(+0-1j),
                           (+1+0j),(+1+0j),(+0+1j),(+1+0j),(+0-1j),(+1+0j),(+1+0j),)
    secondFrame = ((-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j))
    tmitSecondFrame = []

    def test_001_t (self):
        src_data = self.firstFrame + self.secondFrame
        expected_data = self.pnSequence + tuple(self.tmitFirstFrame) + self.pnSequence + tuple(self.tmitSecondFrame)
        expected_data = tuple([val for pair in zip(expected_data,expected_data) for val in pair])
        source = gr.vector_source_c(src_data)
        dut = correlator_cc.preamble_insert_cc()
        sink = gr.vector_sink_c()
        self.tb.connect(source, dut)
        self.tb.connect(dut, sink)
        self.tb.run()
        result_data = sink.data()
        #print "Expected:\n"
        #print expected_data
        #print "Result:\n"
        #print result_data
        self.assertEqual(expected_data, result_data)

if __name__ == '__main__':
    gr_unittest.run(qa_preamble_insert_cc, "qa_preamble_insert_cc.xml")
