/* -*- c++ -*- */
/* 
 * Convolve the input against a PN sequence to find the frame boundary.
 * 
 * <<< This file is autogenerated from pngen.pl.  Do not edit directly>>>
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

#if (15 != PREAMBLE_SYMBOL_LENGTH)
//#error Mismatch preamble length (15 != PREAMBLE_SYMBOL_LENGTH)
#endif

namespace gr {
namespace correlator_cc {

//Taps: [ 4 3 ]
const int correlator_cc_impl::_sequence[15] = {
   +1, +1, +1, +1, -1, -1, -1, +1, -1, -1, +1, +1, -1, +1, -1
};

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
   int index = _accIndex-1;
   double mag = sqrt(real*real + imag*imag);
   double scale = 65536 / mag;  // Normalize magnitude to 2^16

   // Normalization prevents weighting based on power.  While this may normally be desirable,
   // here it is not because it may weight the transmitter feedback incorrectly.
   // The result is more of a phase correlator.

   real = (sampleType)(real*scale);
   imag = (sampleType)(imag*scale);

   // Zero out the accumulator that is getting its first sample
   _accReal[(index+CODE_LENGTH)&ACCUMULATOR_LENGTH_MASK] = 0;
   _accImag[(index+CODE_LENGTH)&ACCUMULATOR_LENGTH_MASK] = 0;

   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] -= real;
   _accImag[index] -= imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] += real;
   _accImag[index] += imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] -= real;
   _accImag[index] -= imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] += real;
   _accImag[index] += imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] += real;
   _accImag[index] += imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] -= real;
   _accImag[index] -= imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] -= real;
   _accImag[index] -= imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] += real;
   _accImag[index] += imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] -= real;
   _accImag[index] -= imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] -= real;
   _accImag[index] -= imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] -= real;
   _accImag[index] -= imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] += real;
   _accImag[index] += imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] += real;
   _accImag[index] += imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] += real;
   _accImag[index] += imag;
   _accReal[++index, index&=ACCUMULATOR_LENGTH_MASK] += real;
   _accImag[index] += imag;


   ++_sampleNum;

   // Threshold the correlation
   double accReal = (double)_accReal[_accIndex&ACCUMULATOR_LENGTH_MASK];
   double accImag = (double)_accImag[_accIndex&ACCUMULATOR_LENGTH_MASK];
   if (sqrt(accReal*accReal + accImag*accImag) > 65536.0/2*CODE_LENGTH)
   {
      //printf("Peak on sample %ld\n", _sampleNum);
      _capsuleLen = CAPSULE_SYMBOL_LENGTH;

      double mag = sqrt(accReal*accReal + accImag*accImag);
      rotator = gr_complex(accReal/mag,-accImag/mag);
   }

   ++_accIndex;
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

	 out[samplesOutput++] = in[samplesRead] * rotator;
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
