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

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


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
   set_min_noutput_items((CODE_LENGTH+CAPSULE_SYMBOL_LENGTH)*2);
   set_max_noutput_items((CODE_LENGTH+CAPSULE_SYMBOL_LENGTH)*2);

   for(int i=0; i<CODE_LENGTH; ++i)
   {
      _sequenceIQ[i] = gr_complex(_sequenceI[i], _sequenceQ[i]);
   }
}

preamble_insert_cc_impl::~preamble_insert_cc_impl()
{
}

void
preamble_insert_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
   ninput_items_required[0] = CAPSULE_SYMBOL_LENGTH;
}

int
preamble_insert_cc_impl::general_work (int noutput_items,
                    gr_vector_int &ninput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items)
{
   const gr_complex* in = reinterpret_cast<const gr_complex*>(input_items[0]);
   gr_complex *out = reinterpret_cast<gr_complex*>(output_items[0]);
   int samplesOutput = 0;
   int samplesRead = ninput_items[0];
   gr_complex prevSample = _sequenceIQ[CODE_LENGTH-1];

   if (samplesRead > CAPSULE_SYMBOL_LENGTH)
   {
      samplesRead = CAPSULE_SYMBOL_LENGTH;
   }

   for(int i=0; i<CODE_LENGTH; ++i)
   {
      out[samplesOutput++] = _sequenceIQ[i];
      out[samplesOutput++] = _sequenceIQ[i];
   }

   for(int i=0; i<samplesRead; ++i)
   {
      //out[samplesOutput++] = out[samplesOutput++] = (prevSample *= in[i]);
      gr_complex outputVal = prevSample *= in[i];
      out[samplesOutput++] = outputVal;
      out[samplesOutput++] = outputVal;
   }

   // Sleep for 100ms.
   struct timeval timeout = {0, 100000};
   select(0,0,0,0,&timeout);


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

   sampleType _accRealOdd[ACCUMULATOR_LENGTH];
   sampleType _accImagOdd[ACCUMULATOR_LENGTH];
   sampleType _accRealEven[ACCUMULATOR_LENGTH];
   sampleType _accImagEven[ACCUMULATOR_LENGTH];
   int _accIndexOdd, _accIndexEven;  // Indexes the accumulator that is recieving its last contribution
   unsigned long _sampleNum;
   unsigned long _capsuleLen;

   int _oddSample; // True if current sample belongs to odd clock
   int _oddData;   // True if odd clocked data should be output

   double _correlationMagnitude; // Magnitude of a correlation peak.  Maintained for one sample to see if next one is stronger.
   double _movingSumAddends[2*CODE_LENGTH];
   double _movingSum;
   int _movingSumIndex;
   int _primed;

   gr_complex _prevSample;

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
   for (int i=0; i<ACCUMULATOR_LENGTH; ++i)
   {
      _accRealOdd[i] = 0;
      _accImagOdd[i] = 0;

      _accRealEven[i] = 0;
      _accImagEven[i] = 0;
   }
   _accIndexOdd = 0;
   _accIndexEven = 0;

   _sampleNum = 0;
   _capsuleLen = 0;

   _oddSample = 0;
   _oddData = 0;

   _correlationMagnitude = 0.0;

   _primed = 0;
   _movingSum = 0.0;
   _movingSumIndex = 0;
   for (int i=0; i<2*CODE_LENGTH; ++i)
      _movingSumAddends[i] = 0.0;
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
   static sampleType lastRealOdd = 0;
   static sampleType lastImagOdd = 0;
   static sampleType lastRealEven = 0;
   static sampleType lastImagEven = 0;

   sampleType& lastReal = (_oddSample ? lastRealOdd : lastRealEven);
   sampleType& lastImag = (_oddSample ? lastImagOdd : lastImagEven);

   int& accIndex = (_oddSample ? _accIndexOdd : _accIndexEven);

   sampleType* accRealArr = (_oddSample ? _accRealOdd : _accRealEven);
   sampleType* accImagArr = (_oddSample ? _accImagOdd : _accImagEven);

   sampleType* realPtr = accRealArr + accIndex;
   sampleType* imagPtr = accImagArr + accIndex;

   sampleType a = lastReal;
   sampleType b = lastImag;
   sampleType e = real;
   sampleType f = imag;
   lastReal = real;
   lastImag = imag;

   real = (a*e + f*b) / (a*a + b*b);
   imag = (a*f - e*b) / (a*a + b*b);
   if (isnan(real) || isnan(imag))
   {
      real = imag = 0.0;
   }


   // Zero out the accumulator that is getting its first sample
   accRealArr[accIndex+CODE_LENGTH] = 0;
   accImagArr[accIndex+CODE_LENGTH] = 0;

$correlateI
$correlateQ

   // Threshold the correlation
   double accReal = (double)accRealArr[accIndex];
   double accImag = (double)accImagArr[accIndex];
   double mag = sqrt(accReal*accReal + accImag*accImag);
   if (_correlationMagnitude > 0.0)
   {
      // Previous sample was peak.  Check if this one is larger.
      _oddData = _oddSample ^ ((mag > _correlationMagnitude) ? 0 : 1);
      // Start passing capsule data out of this block.
      _capsuleLen = CAPSULE_SYMBOL_LENGTH;
      // Reset the correlation magnitude to start looking for next peak.
      _correlationMagnitude = 0.0;
      _movingSumIndex = 0;
      //_primed = 0;
   }
   else if (_primed && (mag > 4*_movingSum/CODE_LENGTH)) // 8 times the average
   {
      printf("Peak on sample %ld\\n", _sampleNum);
      _correlationMagnitude = mag;
   }

   // Update the moving sum of magnitudes for threshold comparisons
   _movingSum -= _movingSumAddends[_movingSumIndex];
   _movingSumAddends[_movingSumIndex] = mag;
   _movingSum += mag;
   _movingSumIndex = (_movingSumIndex+1)%(2*CODE_LENGTH);
   if (!_movingSumIndex)
   {
      _primed = 1;
   }

   ++accIndex;

   // Prevent need for modulo arithematic for circular buffer.  Instead copy back to start of array.
   if (accIndex + CODE_LENGTH == ACCUMULATOR_LENGTH)
   {
      for(int i=0; i<CODE_LENGTH; ++i)
      {
         accRealArr[i] = accRealArr[accIndex+i];
         accImagArr[i] = accImagArr[accIndex+i];
      }
      accIndex = 0;
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
         // Peak has been detected, output this sample (only correct clock)

         if (_oddSample == _oddData)
         {
            out[samplesOutput++] = in[samplesRead]/_prevSample;
            _prevSample = in[samplesRead];
            --_capsuleLen;
         }
         ++_sampleNum;
         ++samplesRead;
         --samplesRemaining;
         _oddSample ^= 1;
      }

      while (samplesRemaining && !_capsuleLen)
      {
         detect_peak(in[samplesRead].real(), in[samplesRead].imag());
         ++_sampleNum;
         ++samplesRead;
         --samplesRemaining;
         _oddSample ^= 1;
         if ((_capsuleLen && (_oddSample != _oddData)) || //    peak detected and next sample is not output
            (!_capsuleLen))                               // or peak not detected
         {
            _prevSample = in[samplesRead-1];
         }
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
import random
import math
import cmath

class qa_correlator_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        random.seed(None)
	self.randomSamples[:] = []
        for x in range(1, $codeLength * 4):
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
    #firstFrame = ((+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j),(+1+0j))
    firstFrame =  ((+0-1j),(+0-1j),(+0-1j),(+0-1j),(+0-1j),(+0-1j),(+0-1j),(+0-1j),(+0-1j),(+0-1j))
    recvFirstFrame = []
    pnSequence = ($pnSequenceIQ)
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
        src_data =  \\
	    tuple(self.randomSamples) +  \\
	    self.pnSequence +  \\
	    tuple(self.recvFirstFrame) +  \\
	    tuple(self.randomSamples) +  \\
	    self.pnSequence +  \\
	    tuple(self.recvSecondFrame)
        src_data = tuple([val for pair in zip(src_data,src_data) for val in pair])
        expected_data =  \\
	    self.firstFrame +  \\
	    self.secondFrame
        source = gr.vector_source_c(src_data)
        dut = correlator_cc.correlator_cc()
        sink = gr.vector_sink_c()
        self.tb.connect(source, dut)
        self.tb.connect(dut, sink)
        self.tb.run()
        result_data = sink.data()
        #print "Expected\\n"
        #print expected_data
        #print "Results\\n"
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
        src_data =  \\
	    tuple(self.randomSamples) +  \\
	    self.pnSequence +  \\
	    tuple(self.recvFirstFrame) +  \\
	    tuple(self.randomSamples) +  \\
	    self.pnSequence +  \\
	    tuple(self.recvSecondFrame)
        src_data = tuple([val for pair in zip(src_data,src_data) for val in pair])
        expected_data =  \\
	    self.firstFrame +  \\
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
        #print "Expected\\n"
        #print expected_data
        #print "Results\\n"
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
        src_data =  \\
	    tuple(self.randomSamples) +  \\
	    self.pnSequence +  \\
	    tuple(self.recvFirstFrame) +  \\
	    tuple(self.randomSamples) +  \\
	    self.pnSequence +  \\
	    tuple(self.recvSecondFrame)
        src_data = tuple([val for pair in zip(src_data,src_data) for val in pair])
        expected_data =  \\
	    self.firstFrame +  \\
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
        #print "Expected\\n"
        #print expected_data
        #print "Results\\n"
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
    pnSequence = ($pnSequenceIQ)
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
