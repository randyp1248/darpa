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
import random
import math
import cmath

class qa_correlator_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        random.seed(None)
	self.randomSamples[:] = []
        for x in range(1, 255 * 4):
            self.randomSamples.append(cmath.rect(1, random.uniform(0,3.141529)))
        self.recvFirstFrame[:] = []
        prev = self.pnSequence[len(self.pnSequence)-1]
        for x in range(len(self.firstFrame)):
	    prev = prev * self.firstFrame[x]
	    self.recvFirstFrame.append(prev)
        self.recvSecondFrame[:] = []
        prev = self.pnSequence[len(self.pnSequence)-1]
        for x in range(len(self.secondFrame)):
	    prev = prev * self.secondFrame[x]
	    self.recvSecondFrame.append(prev)

    def tearDown (self):
        self.tb = None

    randomSamples = []
    firstFrame = ((+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j))
    recvFirstFrame = []
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
    recvSecondFrame = []

    ####################################################################################
    #  test_001_t
    #
    #  Test the sequence:
    #     Random samples - should not correlate, and should all be dropped
    #     PN Sequence - should be detected, but not output
    #     First frame - should be output
    #     Random samples - should not correlate, and should all be dropped
    #     PN Sequence - should be detected, but not output
    #     First frame - should be output
    #
    #  Test passes if the two frames are passed to the output, but no other samples
    ####################################################################################

    def test_001_t (self):
        src_data =  \
	    tuple(self.randomSamples) +  \
	    self.pnSequence +  \
	    tuple(self.recvFirstFrame) +  \
	    tuple(self.randomSamples) +  \
	    self.pnSequence +  \
	    tuple(self.recvSecondFrame)
        src_data = tuple([val for pair in zip(src_data,src_data) for val in pair])
        expected_data =  \
	    self.firstFrame +  \
	    self.secondFrame
        source = gr.vector_source_c(src_data)
        dut = correlator_cc.correlator_cc()
        sink = gr.vector_sink_c()
        self.tb.connect(source, dut)
        self.tb.connect(dut, sink)
        self.tb.run()
        result_data = sink.data()
        #print "Expected\n"
        #print expected_data
        #print "Results\n"
        #print result_data
        self.assertEqual(expected_data, result_data)


    ####################################################################################
    #  test_002_t
    #
    #  This is the same as test_001_t except that the samples are now rotated by pi/2
    #  before passed into the correlator block.  The correlator needs to derotate these
    #  samples before outputing.
    #
    #  Test the sequence:
    #     Random samples - should not correlate, and should all be dropped
    #     PN Sequence - should be detected, but not output
    #     First frame - should be output
    #     Random samples - should not correlate, and should all be dropped
    #     PN Sequence - should be detected, but not output
    #     First frame - should be output
    #
    #  Test passes if the two frames are passed to the output, but no other samples.
    #  The samples will have to be rotated back for the test to pass.
    ####################################################################################
    def test_002_t (self):
        src_data =  \
	    tuple(self.randomSamples) +  \
	    self.pnSequence +  \
	    tuple(self.recvFirstFrame) +  \
	    tuple(self.randomSamples) +  \
	    self.pnSequence +  \
	    tuple(self.recvSecondFrame)
        src_data = tuple([val for pair in zip(src_data,src_data) for val in pair])
        expected_data =  \
	    self.firstFrame +  \
	    self.secondFrame
        rotatedSrcData = []
        for x in range(len(src_data)):
	    rotatedSrcData.append(src_data[x]*(0+1j))
        source = gr.vector_source_c(tuple(rotatedSrcData))
        dut = correlator_cc.correlator_cc()
        sink = gr.vector_sink_c()
        self.tb.connect(source, dut)
        self.tb.connect(dut, sink)
        self.tb.run()
        result_data = sink.data()
        #print "Expected\n"
        #print expected_data
        #print "Results\n"
        #print result_data
        self.assertEqual(expected_data, result_data)

    ####################################################################################
    #  test_003_t
    #
    #  This is the same as test_001_t except that the samples are now rotated by a frequency error
    #  before passed into the correlator block.  The correlator needs to derotate these
    #  samples before outputing.
    #
    #  Test the sequence:
    #     Random samples - should not correlate, and should all be dropped
    #     PN Sequence - should be detected, but not output
    #     First frame - should be output
    #     Random samples - should not correlate, and should all be dropped
    #     PN Sequence - should be detected, but not output
    #     First frame - should be output
    #
    #  Test passes if the two frames are passed to the output, but no other samples.
    #  The samples will have to be rotated back for the test to pass.
    ####################################################################################
    def test_003_t (self):
        src_data =  \
	    tuple(self.randomSamples) +  \
	    self.pnSequence +  \
	    tuple(self.recvFirstFrame) +  \
	    tuple(self.randomSamples) +  \
	    self.pnSequence +  \
	    tuple(self.recvSecondFrame)
        src_data = tuple([val for pair in zip(src_data,src_data) for val in pair])
        expected_data =  \
	    self.firstFrame +  \
	    self.secondFrame
        rotatedSrcData = []
	# Frequency error 1000 samples/cycle
	phaseChangePerSample = cmath.rect(1, 2*math.pi/1000)
	currentPhase = (1 + 0j)
        for x in range(len(src_data)):
	    rotatedSrcData.append(src_data[x]*currentPhase)
	    currentPhase = currentPhase * phaseChangePerSample
        source = gr.vector_source_c(tuple(rotatedSrcData))
        dut = correlator_cc.correlator_cc()
        sink = gr.vector_sink_c()
        self.tb.connect(source, dut)
        self.tb.connect(dut, sink)
        self.tb.run()
        result_data = sink.data()
        #print "Expected\n"
        #print expected_data
        #print "Results\n"
        #print result_data
	self.assertEqual(len(expected_data), len(result_data))
        for x in range(len(expected_data)):
	    angle = cmath.polar(expected_data[x]/result_data[x])[1]
	    #print "Angle = " + str(angle)
	    #print expected_data[x]
	    #print result_data[x]
	    if angle > math.pi:
	        angle = 2*math.pi - angle
	    self.assertLess(abs(angle), 2*math.pi/400)

if __name__ == '__main__':
    gr_unittest.run(qa_correlator_cc, "qa_correlator_cc.xml")
