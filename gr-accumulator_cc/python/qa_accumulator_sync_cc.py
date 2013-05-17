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
import accumulator_cc_swig as accumulator_cc

from gnuradio import digital

class qa_accumulator_sync_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_000_t (self):
        # set up fg
        pos_src_data = (
                    (+0+0j),(+1+1j),(+2+2j),(+3+3j))

        neg_src_data = (
                    (-0-0j),(-1-1j),(-2-2j),(-3-3j))

        # length 8
        expected_data = pos_src_data + neg_src_data

        #print "Length of expected data: ", len(expected_data)
        
        pos_1_16_src_data = (
                    (+0+0j),(+0.0625+0.0625j),(+0.125+0.125j),(+0.1875+0.1875j))

        neg_1_16_src_data = (
                    (-0-0j),(-0.0625-0.0625j),(-0.125-0.125j),(-0.1875-0.1875j))

        sample_data = pos_1_16_src_data + neg_1_16_src_data

        # 2 frames
        repeated_samples = sample_data + sample_data
        # 4 frames
        repeated_samples = repeated_samples + repeated_samples 
        # 8 frames
        repeated_samples = repeated_samples + repeated_samples 
        # 16 frames
        repeated_samples = repeated_samples + repeated_samples 

        #print "Length of 16 samples: ", len(repeated_samples)

        samples = gr.vector_source_c(repeated_samples)
        dut = accumulator_cc.accumulator_sync_cc()
        sink = gr.vector_sink_c()
        self.tb.connect(samples, dut)
        self.tb.connect(dut, sink)
        self.tb.run()
        # check data

        result_data = sink.data()
        #print len(result_data)
        result_data = result_data[len(result_data)-len(expected_data):len(result_data)]
        
        #print len(expected_data)
        #print len(result_data)
        #print expected_data
        #nprint result_data

        # not using this as a test because we've defined hardcoded the sample length
        #self.assertFloatTuplesAlmostEqual(expected_data, result_data)

    def test_001_t (self):
        # set up fg
        pos_src_data = (
                    (+0+0j),(+1+1j),(+2+2j),(+3+3j),
                    (+4+4j),(+5+5j),(+6+6j),(+7+7j),
                    (+8+8j),(+9+9j),(+10+10j),(+11+11j),
                    (+12+12j),(+13+13j),(+14+14j),(+15+15j))

        neg_src_data = (
                    (-0-0j),(-1-1j),(-2-2j),(-3-3j),
                    (-4-4j),(-5-5j),(-6-6j),(-7-7j),
                    (-8-8j),(-9-9j),(-10-10j),(-11-11j),
                    (-12-12j),(-13-13j),(-14-14j),(-15-15j))

        # length 32
        data_32 = pos_src_data + neg_src_data
        # length 64
        data_64 = data_32 + data_32
        # length 128
        data_128 = data_64 + data_64
        # length 256
        data_256 = data_128 + data_128
        # length 512
        expected_data = data_256 + data_256

        #print "Length of expected data: ", len(expected_data)
        
        pos_1_16_src_data = (
                    (+0+0j),(+0.0625+0.0625j),(+0.125+0.125j),(+0.1875+0.1875j),
                    (+0.25+0.25j),(+0.3125+0.3125j),(+0.375+0.375j),(+0.4375+0.4375j),
                    (+0.5+0.5j),(+0.5625+0.5625j),(+0.625+0.625j),(+0.6875+0.6875j),
                    (+0.75+0.75j),(+0.8125+0.8125j),(+0.875+0.875j),(+0.9375+0.9375j))

        neg_1_16_src_data = (
                    (-0-0j),(-0.0625-0.0625j),(-0.125-0.125j),(-0.1875-0.1875j),
                    (-0.25-0.25j),(-0.3125-0.3125j),(-0.375-0.375j),(-0.4375-0.4375j),
                    (-0.5-0.5j),(-0.5625-0.5625j),(-0.625-0.625j),(-0.6875-0.6875j),
                    (-0.75-0.75j),(-0.8125-0.8125j),(-0.875-0.875j),(-0.9375-0.9375j))

        data_1_16_32 = pos_1_16_src_data + neg_1_16_src_data
        data_1_16_64 = data_1_16_32 + data_1_16_32
        data_1_16_128 = data_1_16_64 + data_1_16_64
        data_1_16_256 = data_1_16_128 + data_1_16_128
        sample_data = data_1_16_256 + data_1_16_256

        # 2 frames
        repeated_samples = sample_data + sample_data
        # 4 frames
        repeated_samples = repeated_samples + repeated_samples 
        # 8 frames
        repeated_samples = repeated_samples + repeated_samples 
        # 16 frames
        repeated_samples = repeated_samples + repeated_samples 

        #repeated_expected_three = repeated_samples + repeated_samples + repeated_samples

        #print "Length of 16 samples: ", len(repeated_samples)

        samples = gr.vector_source_c(repeated_samples)
        #samples = gr.vector_source_c(repeated_expected_three)
        dut = accumulator_cc.accumulator_sync_cc()
        #expecter = accumulator_cc.expected_cc()
        #expecter.set_expected_cc(expected_data)
        #expecter.set_accumulator_cc(dut)
        sink = gr.vector_sink_c()
        self.tb.connect(samples, dut)
        #self.tb.connect(dut, expecter)
        #self.tb.connect(expecter, sink)
        self.tb.connect(dut, sink)
        self.tb.run()
        # check data

        result_data = sink.data()
        result_data = result_data[len(result_data)-len(expected_data):len(result_data)]
        
        #print len(expected_data)
        #print len(result_data)
        #print expected_data
        #print result_data

        self.assertFloatTuplesAlmostEqual(expected_data, result_data)

        self.tb.run()
        result_data = sink.data()
        result_data = result_data[len(result_data)-len(expected_data):len(result_data)]
        self.assertFloatTuplesAlmostEqual(expected_data, result_data)

if __name__ == '__main__':
    gr_unittest.run(qa_accumulator_sync_cc, "qa_accumulator_sync_cc.xml")
