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
import print_cc_swig as print_cc

class qa_print_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg

        data = ((0.5+0.5j),(-0.5-0.5j),(1+1j),(-1-1j))
        self.source = gr.vector_source_c(data) 
        self.printer = print_cc.print_cc() 
        self.sink = gr.null_sink(gr.sizeof_gr_complex)
       
        self.tb.connect(self.source, self.printer)
        self.tb.connect(self.printer, self.sink)
        
        self.tb.run ()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_print_cc, "qa_print_cc.xml")
