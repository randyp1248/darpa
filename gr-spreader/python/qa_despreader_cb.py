#!/usr/bin/env python
# 
# Copyright 2013 TRITONS
#
# <<<====================================================================>>>
# <<<====================================================================>>>
# <<< This file is autogenerated from walshgen.pl.  Do not edit directly >>>
# <<<====================================================================>>>
# <<<====================================================================>>>
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
import spreader_swig as spreader
import random

class qa_despreader_cb (gr_unittest.TestCase):

    randomBytes = []

    def setUp (self):
        self.tb = gr.top_block ()
        random.seed(None)
        self.randomBytes[:] = []
        for x in range(0, 5*3):
            self.randomBytes.append(random.randint(0,255));

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        src_data = self.randomBytes
        expected_data = self.randomBytes
        source = gr.vector_source_b(src_data)
        spread = spreader.spreader_bc()
        despread = spreader.despreader_cb()
        sink = gr.vector_sink_b()
        self.tb.connect(source, spread)
        self.tb.connect(spread, despread)
        self.tb.connect(despread, sink)
        self.tb.run()
        result_data = sink.data()
        #print "Expected\n"
        #print expected_data
        #print "Results\n"
        #print result_data
        self.assertEqual(tuple(expected_data), result_data)


if __name__ == '__main__':
    gr_unittest.run(qa_despreader_cb, "qa_despreader_cb.xml")
