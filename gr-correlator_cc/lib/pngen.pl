#!/usr/bin/perl

####################################################################################
####################################################################################
#####                            PN Generation {                               #####
####################################################################################
####################################################################################

#initialize the taps
$numStagesI = 0;
$numStagesQ = 0;
for($tap=0; $tap<100; ++$tap)
{
   $tapsI[$tap] = 0;
   $tapsQ[$tap] = 0;
}

#read the taps from the command line
while ($tap = shift)
{
   if (($tap eq "-i") || ($tap eq "-q"))
   {
      $tapswitch = $tap;
      next;
   }
   if ($tapswitch eq "-i")
   {
      $tapsI[$tap] = 1;
      if ($tap > $numStagesI)
      {
	 $numStagesI = $tap;
      }
   }
   elsif ($tapswitch eq "-q")
   {
      $tapsQ[$tap] = 1;
      if ($tap > $numStagesQ)
      {
	 $numStagesQ = $tap;
      }
   }
   else
   {
      print "ERROR: tap without -i or -q qualifier.\n";
      exit(-1);
   }
}
if ($numStagesQ != $numStagesI)
{
   print "ERROR: number of stages in I and Q are imbalanced.\n";
   exit(-1);
}
$codeLength = (1<<$numStagesI)-1;

if ($numStagesI == 0)
{
   &usage;
   exit(-1);
}

#initialize the stage delays to all ones
for($stageNum=1; $stageNum<=$numStagesI; ++$stageNum)
{
   $stageI[$stageNum] = 1;
   $stageQ[$stageNum] = 1;
}

$tapStringI = "";
$tapStringQ = "";
for($tap=$numStagesI; $tap>=1; --$tap)
{
   if ($tapsI[$tap] == 1) {$tapStringI .= "$tap ";};
   if ($tapsQ[$tap] == 1) {$tapStringQ .= "$tap ";};
}

$pnSequenceI = "";
$pnSequenceQ = "";
$pnSequenceIQ = "";
$correlateRealCode = "";
$correlateImagCode = "";
$correlateI = "";
$correlateQ = "";
$lastI = "";
$lastQ = "";
$skipIteration = 1; 
#iternate over the pn sequence
for($iter=0; $iter<$codeLength; ++$iter)
{
   $newline = "";

#print "$stageI[$numStagesI] $stageQ[$numStagesQ] => ";


   if ((!$stageI[$numStagesI]) && (!$stageQ[$numStagesQ])) # 00 => 1+0j
   { $outputI = "+1"; $outputQ = "+0"; }
   if ((!$stageI[$numStagesI]) && ($stageQ[$numStagesQ])) # 01 => 0+1j
   { $outputI = "+0"; $outputQ = "+1"; }
   if (($stageI[$numStagesI]) && (!$stageQ[$numStagesQ])) # 10 =>-1+0j
   { $outputI = "-1"; $outputQ = "+0"; }
   if (($stageI[$numStagesI]) && ($stageQ[$numStagesQ])) # 11 => 0-1j
   { $outputI = "+0"; $outputQ = "-1"; }

#print "$outputI $outputQ j ";

   # phaseShift = current / last = (e + fj)/(a + bj)
   $e = $outputI;
   $f = $outputQ;
   $a = $lastI;
   $b = $lastQ;
   if (!$skipIteration)
   {
      $phaseShiftI = ($a * $e + $f * $b) / ($a * $a + $b * $b);
      $phaseShiftQ = ($a * $f - $e * $b) / ($a * $a + $b * $b);
      #print "($e $f j)($phaseShiftI $phaseShiftQ j) = ($a $b j)\n";
   }
#   print "phase shift ($phaseShiftI $phaseShiftQ j)\n";

   $lastI = $outputI;
   $lastQ = $outputQ;

   $index = $codeLength-$iter-1;

   if ($skipIteration)
   {
      $skipIteration = 0;
   }
   elsif (($phaseShiftI == 1) && ($phaseShiftQ == 0)) # 00 => 1+0j
   {
      $newlineI = <<END;
   *(realPtr + $index) += real;
END
      $newlineQ = <<END;
   *(imagPtr + $index) += imag;
END

   }
   elsif (($phaseShiftI == 0) && ($phaseShiftQ == 1)) # 01 => 0+1j
   {
      $newlineI = <<END;
   *(realPtr + $index) += imag;
END
      $newlineQ = <<END;
   *(imagPtr + $index) -= real;
END
   }
   elsif (($phaseShiftI == -1) && ($phaseShiftQ == 0)) # 10 =>-1+0j
   {
      $newlineI = <<END;
   *(realPtr + $index) -= real;
END
      $newlineQ = <<END;
   *(imagPtr + $index) -= imag;
END
   }
   elsif (($phaseShiftI == 0) && ($phaseShiftQ == -1)) # 11 => 0-1j
   {
      $newlineI = <<END;
   *(realPtr + $index) -= imag;
END
      $newlineQ = <<END;
   *(imagPtr + $index) += real;
END
   }

   $correlateI = $newlineI . $correlateI;
   $correlateQ = $newlineQ . $correlateQ;



   #print "tapsI  = ";
   for($stageNum=$numStagesI; $stageNum>=1; --$stageNum)
   {
      #print $tapsI[$stageNum] . " ";
   }
   #print "\n";
   #print "stageI = ";
   for($stageNum=$numStagesI; $stageNum>=1; --$stageNum)
   {
      #print $stageI[$stageNum] . " ";
   }
   #print "\n\n";


   $newshiftI = 0;
   $newshiftQ = 0;
   for($stageNum=$numStagesI; $stageNum>1; --$stageNum)
   {
      $newshiftI ^= ($tapsI[$stageNum] * $stageI[$stageNum]);
      $stageI[$stageNum] = $stageI[$stageNum-1];
      $newshiftQ ^= ($tapsQ[$stageNum] * $stageQ[$stageNum]);
      $stageQ[$stageNum] = $stageQ[$stageNum-1];
   }
   $stageI[1] = $newshiftI ^ ($tapsI[1] * $stageI[1]);
   $stageQ[1] = $newshiftQ ^ ($tapsQ[1] * $stageQ[1]);
   if ($iter%16 == 0) {$pnSequenceI .= "   ";$pnSequenceQ .= "   ";}
   if ($iter%8 == 0) {$pnSequenceIQ .= "                           ";}
   $pnSequenceI .= $outputI;
   $pnSequenceQ .= $outputQ;
   $pnSequenceIQ .= "\($outputI" . $outputQ . "j\)";
   if ($iter < $codeLength-1) {$pnSequenceI .= ", ";$pnSequenceQ .= ", ";}
   $pnSequenceIQ .= ",";
   if ($iter%16 == 15) {$pnSequenceI .= "\n";$pnSequenceQ .= "\n";};
   if (($iter < $codeLength-1) &&($iter%8 == 7)) {$pnSequenceIQ .= "\n";};
}

open(PRE_HEADER, '>preamble_insert_cc_impl.h');
open(PRE_CODE, '>preamble_insert_cc_impl.cc');
open(CORR_HEADER, '>correlator_cc_impl.h');
open(CORR_CODE, '>correlator_cc_impl.cc');
open(TESTCODE, '>../python/qa_correlator_cc.py');
open(PRE_TESTCODE, '>../python/qa_preamble_insert_cc.py');

####################################################################################
####################################################################################
#####                 } Preamble Insertion Header Generation {                 #####
####################################################################################
####################################################################################

print PRE_HEADER <<END;
/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 *
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 * <<< This file is autogenerated from pngen.pl.  Do not edit directly >>>
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_CORRELATOR_CC_PREAMBLE_INSERT_CC_IMPL_H
#define INCLUDED_CORRELATOR_CC_PREAMBLE_INSERT_CC_IMPL_H

#include <correlator_cc/preamble_insert_cc.h>
#include "TRITONS.h"

#define CODE_LENGTH ($codeLength)

namespace gr {
namespace correlator_cc {

class preamble_insert_cc_impl : public preamble_insert_cc
{
private:

   static const int _sequenceI[CODE_LENGTH];
   static const int _sequenceQ[CODE_LENGTH];

   gr_complex _sequenceIQ[CODE_LENGTH];
   int _sampleIndex;
   gr_complex _prevSample;
   int _pnIndex;

public:
   preamble_insert_cc_impl();
   ~preamble_insert_cc_impl();

   void forecast (int noutput_items, gr_vector_int &ninput_items_required);

   int general_work(int noutput_items,
		    gr_vector_int &ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items);
};

} // namespace correlator_cc
} // namespace gr

#endif /* INCLUDED_CORRELATOR_CC_PREAMBLE_INSERT_CC_IMPL_H */

END

####################################################################################
####################################################################################
#####                 } Preamble Insertion Code Generation {                   #####
####################################################################################
####################################################################################

print PRE_CODE <<END;
/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 *
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 * <<< This file is autogenerated from pngen.pl.  Do not edit directly >>>
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include "preamble_insert_cc_impl.h"
#include <stdio.h>
#include "TRITONS.h"

namespace gr {
namespace correlator_cc {

static const int MIN_IN = 1;  // mininum number of input streams
static const int MAX_IN = 1;  // maximum number of input streams
static const int MIN_OUT = 1; // minimum number of output streams
static const int MAX_OUT = 1; // maximum number of output streams

//Taps: [ $tapStringI]
const int preamble_insert_cc_impl::_sequenceI[$codeLength] = {
$pnSequenceI
};
//Taps: [ $tapStringQ]
const int preamble_insert_cc_impl::_sequenceQ[$codeLength] = {
$pnSequenceQ
};

preamble_insert_cc::sptr
preamble_insert_cc::make()
{
   return gnuradio::get_initial_sptr (new preamble_insert_cc_impl());
}

preamble_insert_cc_impl::preamble_insert_cc_impl()
  : gr_block("preamble_insert_cc",
		   gr_make_io_signature(MIN_IN, MAX_IN, sizeof (gr_complex)),
		   gr_make_io_signature(MIN_IN, MAX_IN, sizeof (gr_complex)))
{
   set_min_output_buffer(0, 2*CODE_LENGTH);

   for(int i=0; i<CODE_LENGTH; ++i)
   {
      _sequenceIQ[i] = gr_complex(_sequenceI[i], _sequenceQ[i]);
   }
   _sampleIndex = 0;
   _pnIndex = 0;
}

preamble_insert_cc_impl::~preamble_insert_cc_impl()
{
}

void
preamble_insert_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
    /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    ninput_items_required[0] = noutput_items;
}

int
preamble_insert_cc_impl::general_work (int noutput_items,
		    gr_vector_int &ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items)
{
   const gr_complex* in = reinterpret_cast<const gr_complex*>(input_items[0]);
   gr_complex *out = reinterpret_cast<gr_complex*>(output_items[0]);
   int samplesRemaining = ninput_items[0];
   int samplesOutput = 0;
   int samplesRead = 0;

   while ((samplesOutput < noutput_items) && samplesRemaining)
   {
      if ((!_sampleIndex) && (_pnIndex < CODE_LENGTH))
      {
	 // New frame.  Start with known previous sample differential encoding.
	 _prevSample = gr_complex(1.0, 0.0);

	 // Output the preamble
	 out[samplesOutput++] = _sequenceIQ[_pnIndex++];
      }
      else
      {
         // Output a sample from the input
	 out[samplesOutput++] = in[samplesRead++];
	 --samplesRemaining;
	 if (++_sampleIndex == CAPSULE_SYMBOL_LENGTH)
	 {
	    // Wrap around.  Next sample will be new frame boundary
	    _sampleIndex = 0;
	 }
         _pnIndex = 0;
      }
   }

   // Tell runtime system how many input items we consumed on each input stream.
   consume_each (samplesRead);

   // Tell runtime system how many output items we produced.
   return samplesOutput;
}

} /* namespace correlator_cc */
} /* namespace gr */

END

####################################################################################
####################################################################################
#####              } Correlation Insertion Header Generation {                 #####
####################################################################################
####################################################################################

print CORR_HEADER <<END;
/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 *
 * Convolve the input against a PN sequence to find the frame boundary.
 * 
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 * <<< This file is autogenerated from pngen.pl.  Do not edit directly >>>
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_CORRELATOR_CC_CORRELATOR_CC_IMPL_H
#define INCLUDED_CORRELATOR_CC_CORRELATOR_CC_IMPL_H

#include <correlator_cc/correlator_cc.h>

#define CODE_LENGTH ($codeLength)
#define ACCUMULATOR_LENGTH (1<<($numStagesI+1))
#define ACCUMULATOR_LENGTH_MASK (ACCUMULATOR_LENGTH-1)

typedef float sampleType;

namespace gr {
namespace correlator_cc {

class correlator_cc_impl : public correlator_cc
{

private:
   void detect_peak(sampleType real, sampleType imag);

   sampleType _accReal[ACCUMULATOR_LENGTH];
   sampleType _accImag[ACCUMULATOR_LENGTH];
   int _accIndex;  // Indexes the accumulator that is receiving its last contribution
   unsigned long _sampleNum;
   unsigned long _capsuleLen;

public:
   correlator_cc_impl();
   ~correlator_cc_impl();

   void forecast (int noutput_items, gr_vector_int &ninput_items_required);

   int general_work(int noutput_items,
		    gr_vector_int &ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items);
}; // correlator_cc_impl

} // namespace correlator_cc
} // namespace gr

#endif /* INCLUDED_CORRELATOR_CC_CORRELATOR_CC_IMPL_H */
END

####################################################################################
####################################################################################
#####                   } Correlation Code Generation {                        #####
####################################################################################
####################################################################################

print CORR_CODE <<END;
/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 *
 * Convolve the input against a PN sequence to find the frame boundary.
 * 
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 * <<< This file is autogenerated from pngen.pl.  Do not edit directly >>>
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include <complex>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "correlator_cc_impl.h"
#include "TRITONS.h"

#if ($codeLength != PREAMBLE_SYMBOL_LENGTH)
//#error Mismatch preamble length ($codeLength != PREAMBLE_SYMBOL_LENGTH)
#endif

namespace gr {
namespace correlator_cc {

static const int MIN_IN = 1;  // mininum number of input streams
static const int MAX_IN = 1;  // maximum number of input streams
static const int MIN_OUT = 1; // minimum number of output streams
static const int MAX_OUT = 1; // maximum number of output streams

correlator_cc::sptr
correlator_cc::make()
{
   return gnuradio::get_initial_sptr (new correlator_cc_impl());
}

/*
 * The private constructor
 */
correlator_cc_impl::correlator_cc_impl()
   : gr_block(
        "correlator_cc",
        gr_make_io_signature(MIN_IN, MAX_IN, sizeof(gr_complex)),
        gr_make_io_signature(MIN_OUT, MAX_OUT, sizeof(gr_complex)))
	
{
   int i;

   for (i=0; i<ACCUMULATOR_LENGTH; ++i)
   {
      _accReal[i] = 0;
      _accImag[i] = 0;
      _accIndex = 0;
   }
   _sampleNum = 0;
   _capsuleLen = 0;
}

/*
 * Our virtual destructor.
 */
correlator_cc_impl::~correlator_cc_impl()
{
}

void
correlator_cc_impl::detect_peak(sampleType real, sampleType imag)
{
   static sampleType lastReal = 0;
   static sampleType lastImag = 0;

   sampleType* realPtr = _accReal + _accIndex;
   sampleType* imagPtr = _accImag + _accIndex;

   sampleType a = lastReal;
   sampleType b = lastImag;
   sampleType e = real;
   sampleType f = imag;
   lastReal = real;
   lastImag = imag;

   real = (a*e + f*b) / (a*a + b*b);
   imag = (a*f - e*b) / (a*a + b*b);

   // Zero out the accumulator that is getting its first sample
   _accReal[_accIndex+CODE_LENGTH] = 0;
   _accImag[_accIndex+CODE_LENGTH] = 0;

$correlateI
$correlateQ

   ++_sampleNum;

   // Threshold the correlation
   double accReal = (double)_accReal[_accIndex];
   double accImag = (double)_accImag[_accIndex];
//printf("%f\\n",sqrt(accReal*accReal + accImag*accImag));
   if (sqrt(accReal*accReal + accImag*accImag) > CODE_LENGTH/2.0)
   {
      printf("Peak on sample %ld\\n", _sampleNum);
      _capsuleLen = CAPSULE_SYMBOL_LENGTH;
   }

   ++_accIndex;

   if (_accIndex + CODE_LENGTH == ACCUMULATOR_LENGTH)
   {
      for(int i=0; i<CODE_LENGTH; ++i)
      {
         _accReal[i] = _accReal[_accIndex+i];
         _accImag[i] = _accImag[_accIndex+i];
      }
      _accIndex = 0;
   }
}


void
correlator_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
   ninput_items_required[0] = noutput_items;
}

int
correlator_cc_impl::general_work (
   int noutput_items,
   gr_vector_int &ninput_items,
   gr_vector_const_void_star &input_items,
   gr_vector_void_star &output_items
)
{
   const gr_complex* in = reinterpret_cast<const gr_complex*>(input_items[0]);
   gr_complex *out = reinterpret_cast<gr_complex*>(output_items[0]);
   int samplesRemaining = ninput_items[0];
   int samplesOutput = 0;
   int samplesRead = 0;

   while (samplesRemaining)
   {
      while (samplesRemaining && _capsuleLen)
      {
         // Peak has been detected, output this sample

	 out[samplesOutput++] = in[samplesRead];
	 ++samplesRead;
	 --samplesRemaining;
	 --_capsuleLen;
      }

      while (samplesRemaining && !_capsuleLen)
      {
	 detect_peak(in[samplesRead].real(), in[samplesRead].imag());
	 ++samplesRead;
	 --samplesRemaining;
      }
   }

   // Tell runtime system how many input items we consumed on each input stream.
   consume_each (samplesRead);

   // Tell runtime system how many output items we produced.
   return samplesOutput;
}

} /* namespace correlator_cc */
} /* namespace gr */
END

####################################################################################
####################################################################################
#####                     } Correlator Test Generation {                       #####
####################################################################################
####################################################################################

print TESTCODE <<END;
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

class qa_correlator_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    randomSamples = ((+2+2j),(+2-2j),(-2-2j),(-2+2j),(-2+2j),(+2-2j),(+2+2j),(-2-2j),(+2-2j),(+2+2j))
    firstFrame = ((+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j))
    pnSequence = ($pnSequenceIQ)
    secondFrame = ((-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j))

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

        src_data = self.randomSamples + self.pnSequence + self.firstFrame + self.randomSamples + self.pnSequence + self.secondFrame
        expected_data = self.firstFrame + self.secondFrame
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
    def xxst_002_t (self):

        src_data        = (
			   # Random samples
			   (+2+2j),(+2-2j),(-2-2j),(-2+2j),(-2+2j),(+2-2j),(+2+2j),(-2-2j),(+2-2j),(+2+2j),
			   # PN Sequence
$pnSequenceIQ
			   # First frame data
			   (+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),
			   # Random samples
			   (+2+2j),(+2-2j),(-2-2j),(-2+2j),(-2+2j),(+2-2j),(+2+2j),(-2-2j),(+2-2j),(+2+2j),
			   # PN Sequence
$pnSequenceIQ
			   # Second frame data
			   (-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j))
        expected_data   = ((+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),
			   (-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j))

        # Rotate the samples by pi/2
        source = gr.vector_source_c(tuple([z * (0+1j) for z in src_data]))
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
END


####################################################################################
####################################################################################
#####                 } Insert Preamble Test Generation {                      #####
####################################################################################
####################################################################################

print PRE_TESTCODE <<END;
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

    def tearDown (self):
        self.tb = None

    firstFrame = ((+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j))
    pnSequence = ($pnSequenceIQ)
    secondFrame = ((-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j),(-1+0j))

    def test_001_t (self):
        src_data = self.firstFrame + self.secondFrame
        expected_data = self.pnSequence + self.firstFrame + self.pnSequence + self.secondFrame
        source = gr.vector_source_c(src_data)
	dut = correlator_cc.preamble_insert_cc()
        sink = gr.vector_sink_c()
        self.tb.connect(source, dut)
        self.tb.connect(dut, sink)
        self.tb.run()
        result_data = sink.data()
	#print "Expected:\\n"
        #print expected_data
	#print "Result:\\n"
        #print result_data
        self.assertEqual(expected_data, result_data)

if __name__ == '__main__':
    gr_unittest.run(qa_preamble_insert_cc, "qa_preamble_insert_cc.xml")
END

####################################################################################
####################################################################################
#####                                } Usage                                   #####
####################################################################################
####################################################################################

close(PRE_TESTCODE);
close(TESTCODE);
close(PRE_CODE);
close(PRE_HEADER);
close(CORR_CODE);
close(CORR_HEADER);

sub usage
{
   print <<END;
Usage: ./pngen.pl -i <tap>... -q <tap>...
The number of taps must be non-zero.
Number of stages is derived from the largest tap.
The number of stages in the I and Q branches must be equal.

Maximal length taps are:
=========================
[ 2 1 ]
[ 3 2 ] [ 3 1 ]
[ 4 3 ] [ 4 1 ]
[ 5 3 ] [ 5 2 ]
[ 6 5 ] [ 6 1 ]
[ 7 6 ] [ 7 3 ] [ 7 1 ]
[ 8 6 5 4 ] [ 8 6 5 3 ]
[ 9 5 ] [ 9 6 4 3 ]
[ 10 7 ] [ 10 3 ]
[ 11 9 ] [ 11 8 5 2 ]
[ 12 6 4 1 ]
[ 13 4 3 1 ]
[ 14 5 3 1 ]
[ 15 14 ] [ 15 4 ]
[ 16 15 13 4 ]
[ 17 14 ] [ 17 3 ]
[ 18 11 ] [ 18 7 ]
[ 19 6 2 1 ]
[ 20 17 ] [ 20 3 ]
[ 21 19 ] [ 21 2 ]
[ 22 21 ] [ 22 1 ]
[ 23 18 ] [ 23 5 ]
[ 24 23 22 17 ]
[ 25 22 ] [ 25 3 ]
=========================
END
}
