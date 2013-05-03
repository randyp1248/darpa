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
import correlator_cc_swig as correlator_cc

class qa_correlator_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):

        src_data        = (
			   # Random samples
			   (+1+1j),(+1-1j),(-1-1j),(-1+1j),(-1+1j),(+1-1j),(+1+1j),(-1-1j),(+1-1j),(+1+1j),
			   # PN Sequence
                           (+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(-1+0j),(-1+0j),(-1+0j),
                           (+1+0j),(+1+0j),(-1+0j),(+1+0j),(+1+0j),(+1+0j),(-1+0j),(+1+0j),
                           (-1+0j),(+1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(+1+0j),(-1+0j),
                           (-1+0j),(+1+0j),(-1+0j),(+1+0j),(+1+0j),(-1+0j),(-1+0j),
			   # First frame data
			   (+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),
			   # Random samples
			   (+1+1j),(+1-1j),(-1-1j),(-1+1j),(-1+1j),(+1-1j),(+1+1j),(-1-1j),(+1-1j),(+1+1j),
			   # PN Sequence
                           (+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(-1+0j),(-1+0j),(-1+0j),
                           (+1+0j),(+1+0j),(-1+0j),(+1+0j),(+1+0j),(+1+0j),(-1+0j),(+1+0j),
                           (-1+0j),(+1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(+1+0j),(-1+0j),
                           (-1+0j),(+1+0j),(-1+0j),(+1+0j),(+1+0j),(-1+0j),(-1+0j),
			   # Second frame data
			   (-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j))
        expected_data   = ((+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),
			   (-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j))
        source = gr.vector_source_c(src_data)
	dut = correlator_cc.correlator_cc()
        sink = gr.vector_sink_c()
        self.tb.connect(source, dut)
        self.tb.connect(dut, sink)
        self.tb.run()
        result_data = sink.data()
        #print expected_data
        #print result_data
        self.assertEqual(expected_data, result_data)


if __name__ == '__main__':
    gr_unittest.run(qa_correlator_cc, "qa_correlator_cc.xml")
