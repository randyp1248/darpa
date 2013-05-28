/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 *
 * Despread input constellation stream and correlate with each row of the
 * walsh table to determine the index bits to output.
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
#include "despreader_cb_impl.h"

namespace gr {
namespace spreader {

static const int MIN_IN = 1;
static const int MAX_IN = 1;
static const int MIN_OUT = 1;
static const int MAX_OUT = 1;

despreader_cb::sptr
despreader_cb::make()
{
   return gnuradio::get_initial_sptr (new despreader_cb_impl());
}

despreader_cb_impl::despreader_cb_impl()
   : gr_block("despreader_cb",
		  gr_make_io_signature(MIN_IN, MAX_IN, sizeof (gr_complex)),
		  gr_make_io_signature(MIN_OUT, MAX_OUT, sizeof (unsigned char)))
{

   set_min_noutput_items(3);
   set_max_noutput_items(3);
}

despreader_cb_impl::~despreader_cb_impl()
{
}

void
despreader_cb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
   ninput_items_required[0] = (3 * 8 / 3 * 8);
}

int
despreader_cb_impl::general_work (int noutput_items,
		   gr_vector_int &ninput_items,
		   gr_vector_const_void_star &input_items,
		   gr_vector_void_star &output_items)
{
   const gr_complex* in = (gr_complex*) input_items[0];
   unsigned char* out = (unsigned char*) output_items[0];
   float currentCorrelationValue;
   float maxCorrelationValue;
   unsigned maxCorrelationIndex;
   unsigned maxCorrelationSign;
   unsigned temp;
   
   maxCorrelationValue = 0.0;
   currentCorrelationValue =
      - in[0+0*8/2].real() - in[0+0*8/2].imag()
      - in[1+0*8/2].real() - in[1+0*8/2].imag()
      - in[2+0*8/2].real() - in[2+0*8/2].imag()
      - in[3+0*8/2].real() - in[3+0*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 0;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+0*8/2].real() - in[0+0*8/2].imag()
      - in[1+0*8/2].real() - in[1+0*8/2].imag()
      - in[2+0*8/2].real() - in[2+0*8/2].imag()
      - in[3+0*8/2].real() - in[3+0*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 1;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+0*8/2].real() - in[0+0*8/2].imag()
      + in[1+0*8/2].real() + in[1+0*8/2].imag()
      - in[2+0*8/2].real() - in[2+0*8/2].imag()
      + in[3+0*8/2].real() + in[3+0*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 2;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+0*8/2].real() - in[0+0*8/2].imag()
      + in[1+0*8/2].real() + in[1+0*8/2].imag()
      - in[2+0*8/2].real() - in[2+0*8/2].imag()
      + in[3+0*8/2].real() + in[3+0*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 3;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+0*8/2].real() - in[0+0*8/2].imag()
      - in[1+0*8/2].real() - in[1+0*8/2].imag()
      + in[2+0*8/2].real() + in[2+0*8/2].imag()
      + in[3+0*8/2].real() + in[3+0*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 4;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+0*8/2].real() - in[0+0*8/2].imag()
      - in[1+0*8/2].real() - in[1+0*8/2].imag()
      + in[2+0*8/2].real() + in[2+0*8/2].imag()
      + in[3+0*8/2].real() + in[3+0*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 5;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+0*8/2].real() - in[0+0*8/2].imag()
      + in[1+0*8/2].real() + in[1+0*8/2].imag()
      + in[2+0*8/2].real() + in[2+0*8/2].imag()
      - in[3+0*8/2].real() - in[3+0*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 6;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+0*8/2].real() - in[0+0*8/2].imag()
      + in[1+0*8/2].real() + in[1+0*8/2].imag()
      + in[2+0*8/2].real() + in[2+0*8/2].imag()
      - in[3+0*8/2].real() - in[3+0*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 7;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   temp = (((unsigned)out[0])<<24) | 
	  (((unsigned)out[0+1])<<16) | 
	  (((unsigned)out[0+2])<<8) | 
	  (((unsigned)out[0+3]));
   temp &= ~(((1<<3)-1) << (32-0-3));
   temp |= maxCorrelationIndex << (32-0-3);
   out[0]   = (temp>>24) & 0xff;
   out[0+1] = (temp>>16) & 0xff;
   out[0+2] = (temp>>8 ) & 0xff;
   out[0+3] = (temp    ) & 0xff;


   maxCorrelationValue = 0.0;
   currentCorrelationValue =
      - in[0+1*8/2].real() - in[0+1*8/2].imag()
      - in[1+1*8/2].real() - in[1+1*8/2].imag()
      - in[2+1*8/2].real() - in[2+1*8/2].imag()
      - in[3+1*8/2].real() - in[3+1*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 0;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+1*8/2].real() - in[0+1*8/2].imag()
      - in[1+1*8/2].real() - in[1+1*8/2].imag()
      - in[2+1*8/2].real() - in[2+1*8/2].imag()
      - in[3+1*8/2].real() - in[3+1*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 1;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+1*8/2].real() - in[0+1*8/2].imag()
      + in[1+1*8/2].real() + in[1+1*8/2].imag()
      - in[2+1*8/2].real() - in[2+1*8/2].imag()
      + in[3+1*8/2].real() + in[3+1*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 2;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+1*8/2].real() - in[0+1*8/2].imag()
      + in[1+1*8/2].real() + in[1+1*8/2].imag()
      - in[2+1*8/2].real() - in[2+1*8/2].imag()
      + in[3+1*8/2].real() + in[3+1*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 3;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+1*8/2].real() - in[0+1*8/2].imag()
      - in[1+1*8/2].real() - in[1+1*8/2].imag()
      + in[2+1*8/2].real() + in[2+1*8/2].imag()
      + in[3+1*8/2].real() + in[3+1*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 4;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+1*8/2].real() - in[0+1*8/2].imag()
      - in[1+1*8/2].real() - in[1+1*8/2].imag()
      + in[2+1*8/2].real() + in[2+1*8/2].imag()
      + in[3+1*8/2].real() + in[3+1*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 5;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+1*8/2].real() - in[0+1*8/2].imag()
      + in[1+1*8/2].real() + in[1+1*8/2].imag()
      + in[2+1*8/2].real() + in[2+1*8/2].imag()
      - in[3+1*8/2].real() - in[3+1*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 6;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+1*8/2].real() - in[0+1*8/2].imag()
      + in[1+1*8/2].real() + in[1+1*8/2].imag()
      + in[2+1*8/2].real() + in[2+1*8/2].imag()
      - in[3+1*8/2].real() - in[3+1*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 7;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   temp = (((unsigned)out[0])<<24) | 
	  (((unsigned)out[0+1])<<16) | 
	  (((unsigned)out[0+2])<<8) | 
	  (((unsigned)out[0+3]));
   temp &= ~(((1<<3)-1) << (32-3-3));
   temp |= maxCorrelationIndex << (32-3-3);
   out[0]   = (temp>>24) & 0xff;
   out[0+1] = (temp>>16) & 0xff;
   out[0+2] = (temp>>8 ) & 0xff;
   out[0+3] = (temp    ) & 0xff;


   maxCorrelationValue = 0.0;
   currentCorrelationValue =
      - in[0+2*8/2].real() - in[0+2*8/2].imag()
      - in[1+2*8/2].real() - in[1+2*8/2].imag()
      - in[2+2*8/2].real() - in[2+2*8/2].imag()
      - in[3+2*8/2].real() - in[3+2*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 0;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+2*8/2].real() - in[0+2*8/2].imag()
      - in[1+2*8/2].real() - in[1+2*8/2].imag()
      - in[2+2*8/2].real() - in[2+2*8/2].imag()
      - in[3+2*8/2].real() - in[3+2*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 1;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+2*8/2].real() - in[0+2*8/2].imag()
      + in[1+2*8/2].real() + in[1+2*8/2].imag()
      - in[2+2*8/2].real() - in[2+2*8/2].imag()
      + in[3+2*8/2].real() + in[3+2*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 2;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+2*8/2].real() - in[0+2*8/2].imag()
      + in[1+2*8/2].real() + in[1+2*8/2].imag()
      - in[2+2*8/2].real() - in[2+2*8/2].imag()
      + in[3+2*8/2].real() + in[3+2*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 3;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+2*8/2].real() - in[0+2*8/2].imag()
      - in[1+2*8/2].real() - in[1+2*8/2].imag()
      + in[2+2*8/2].real() + in[2+2*8/2].imag()
      + in[3+2*8/2].real() + in[3+2*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 4;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+2*8/2].real() - in[0+2*8/2].imag()
      - in[1+2*8/2].real() - in[1+2*8/2].imag()
      + in[2+2*8/2].real() + in[2+2*8/2].imag()
      + in[3+2*8/2].real() + in[3+2*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 5;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+2*8/2].real() - in[0+2*8/2].imag()
      + in[1+2*8/2].real() + in[1+2*8/2].imag()
      + in[2+2*8/2].real() + in[2+2*8/2].imag()
      - in[3+2*8/2].real() - in[3+2*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 6;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+2*8/2].real() - in[0+2*8/2].imag()
      + in[1+2*8/2].real() + in[1+2*8/2].imag()
      + in[2+2*8/2].real() + in[2+2*8/2].imag()
      - in[3+2*8/2].real() - in[3+2*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 7;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   temp = (((unsigned)out[0])<<24) | 
	  (((unsigned)out[0+1])<<16) | 
	  (((unsigned)out[0+2])<<8) | 
	  (((unsigned)out[0+3]));
   temp &= ~(((1<<3)-1) << (32-6-3));
   temp |= maxCorrelationIndex << (32-6-3);
   out[0]   = (temp>>24) & 0xff;
   out[0+1] = (temp>>16) & 0xff;
   out[0+2] = (temp>>8 ) & 0xff;
   out[0+3] = (temp    ) & 0xff;


   maxCorrelationValue = 0.0;
   currentCorrelationValue =
      - in[0+3*8/2].real() - in[0+3*8/2].imag()
      - in[1+3*8/2].real() - in[1+3*8/2].imag()
      - in[2+3*8/2].real() - in[2+3*8/2].imag()
      - in[3+3*8/2].real() - in[3+3*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 0;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+3*8/2].real() - in[0+3*8/2].imag()
      - in[1+3*8/2].real() - in[1+3*8/2].imag()
      - in[2+3*8/2].real() - in[2+3*8/2].imag()
      - in[3+3*8/2].real() - in[3+3*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 1;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+3*8/2].real() - in[0+3*8/2].imag()
      + in[1+3*8/2].real() + in[1+3*8/2].imag()
      - in[2+3*8/2].real() - in[2+3*8/2].imag()
      + in[3+3*8/2].real() + in[3+3*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 2;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+3*8/2].real() - in[0+3*8/2].imag()
      + in[1+3*8/2].real() + in[1+3*8/2].imag()
      - in[2+3*8/2].real() - in[2+3*8/2].imag()
      + in[3+3*8/2].real() + in[3+3*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 3;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+3*8/2].real() - in[0+3*8/2].imag()
      - in[1+3*8/2].real() - in[1+3*8/2].imag()
      + in[2+3*8/2].real() + in[2+3*8/2].imag()
      + in[3+3*8/2].real() + in[3+3*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 4;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+3*8/2].real() - in[0+3*8/2].imag()
      - in[1+3*8/2].real() - in[1+3*8/2].imag()
      + in[2+3*8/2].real() + in[2+3*8/2].imag()
      + in[3+3*8/2].real() + in[3+3*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 5;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+3*8/2].real() - in[0+3*8/2].imag()
      + in[1+3*8/2].real() + in[1+3*8/2].imag()
      + in[2+3*8/2].real() + in[2+3*8/2].imag()
      - in[3+3*8/2].real() - in[3+3*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 6;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+3*8/2].real() - in[0+3*8/2].imag()
      + in[1+3*8/2].real() + in[1+3*8/2].imag()
      + in[2+3*8/2].real() + in[2+3*8/2].imag()
      - in[3+3*8/2].real() - in[3+3*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 7;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   temp = (((unsigned)out[1])<<24) | 
	  (((unsigned)out[1+1])<<16) | 
	  (((unsigned)out[1+2])<<8) | 
	  (((unsigned)out[1+3]));
   temp &= ~(((1<<3)-1) << (32-1-3));
   temp |= maxCorrelationIndex << (32-1-3);
   out[1]   = (temp>>24) & 0xff;
   out[1+1] = (temp>>16) & 0xff;
   out[1+2] = (temp>>8 ) & 0xff;
   out[1+3] = (temp    ) & 0xff;


   maxCorrelationValue = 0.0;
   currentCorrelationValue =
      - in[0+4*8/2].real() - in[0+4*8/2].imag()
      - in[1+4*8/2].real() - in[1+4*8/2].imag()
      - in[2+4*8/2].real() - in[2+4*8/2].imag()
      - in[3+4*8/2].real() - in[3+4*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 0;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+4*8/2].real() - in[0+4*8/2].imag()
      - in[1+4*8/2].real() - in[1+4*8/2].imag()
      - in[2+4*8/2].real() - in[2+4*8/2].imag()
      - in[3+4*8/2].real() - in[3+4*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 1;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+4*8/2].real() - in[0+4*8/2].imag()
      + in[1+4*8/2].real() + in[1+4*8/2].imag()
      - in[2+4*8/2].real() - in[2+4*8/2].imag()
      + in[3+4*8/2].real() + in[3+4*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 2;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+4*8/2].real() - in[0+4*8/2].imag()
      + in[1+4*8/2].real() + in[1+4*8/2].imag()
      - in[2+4*8/2].real() - in[2+4*8/2].imag()
      + in[3+4*8/2].real() + in[3+4*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 3;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+4*8/2].real() - in[0+4*8/2].imag()
      - in[1+4*8/2].real() - in[1+4*8/2].imag()
      + in[2+4*8/2].real() + in[2+4*8/2].imag()
      + in[3+4*8/2].real() + in[3+4*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 4;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+4*8/2].real() - in[0+4*8/2].imag()
      - in[1+4*8/2].real() - in[1+4*8/2].imag()
      + in[2+4*8/2].real() + in[2+4*8/2].imag()
      + in[3+4*8/2].real() + in[3+4*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 5;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+4*8/2].real() - in[0+4*8/2].imag()
      + in[1+4*8/2].real() + in[1+4*8/2].imag()
      + in[2+4*8/2].real() + in[2+4*8/2].imag()
      - in[3+4*8/2].real() - in[3+4*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 6;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+4*8/2].real() - in[0+4*8/2].imag()
      + in[1+4*8/2].real() + in[1+4*8/2].imag()
      + in[2+4*8/2].real() + in[2+4*8/2].imag()
      - in[3+4*8/2].real() - in[3+4*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 7;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   temp = (((unsigned)out[1])<<24) | 
	  (((unsigned)out[1+1])<<16) | 
	  (((unsigned)out[1+2])<<8) | 
	  (((unsigned)out[1+3]));
   temp &= ~(((1<<3)-1) << (32-4-3));
   temp |= maxCorrelationIndex << (32-4-3);
   out[1]   = (temp>>24) & 0xff;
   out[1+1] = (temp>>16) & 0xff;
   out[1+2] = (temp>>8 ) & 0xff;
   out[1+3] = (temp    ) & 0xff;


   maxCorrelationValue = 0.0;
   currentCorrelationValue =
      - in[0+5*8/2].real() - in[0+5*8/2].imag()
      - in[1+5*8/2].real() - in[1+5*8/2].imag()
      - in[2+5*8/2].real() - in[2+5*8/2].imag()
      - in[3+5*8/2].real() - in[3+5*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 0;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+5*8/2].real() - in[0+5*8/2].imag()
      - in[1+5*8/2].real() - in[1+5*8/2].imag()
      - in[2+5*8/2].real() - in[2+5*8/2].imag()
      - in[3+5*8/2].real() - in[3+5*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 1;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+5*8/2].real() - in[0+5*8/2].imag()
      + in[1+5*8/2].real() + in[1+5*8/2].imag()
      - in[2+5*8/2].real() - in[2+5*8/2].imag()
      + in[3+5*8/2].real() + in[3+5*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 2;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+5*8/2].real() - in[0+5*8/2].imag()
      + in[1+5*8/2].real() + in[1+5*8/2].imag()
      - in[2+5*8/2].real() - in[2+5*8/2].imag()
      + in[3+5*8/2].real() + in[3+5*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 3;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+5*8/2].real() - in[0+5*8/2].imag()
      - in[1+5*8/2].real() - in[1+5*8/2].imag()
      + in[2+5*8/2].real() + in[2+5*8/2].imag()
      + in[3+5*8/2].real() + in[3+5*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 4;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+5*8/2].real() - in[0+5*8/2].imag()
      - in[1+5*8/2].real() - in[1+5*8/2].imag()
      + in[2+5*8/2].real() + in[2+5*8/2].imag()
      + in[3+5*8/2].real() + in[3+5*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 5;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+5*8/2].real() - in[0+5*8/2].imag()
      + in[1+5*8/2].real() + in[1+5*8/2].imag()
      + in[2+5*8/2].real() + in[2+5*8/2].imag()
      - in[3+5*8/2].real() - in[3+5*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 6;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+5*8/2].real() - in[0+5*8/2].imag()
      + in[1+5*8/2].real() + in[1+5*8/2].imag()
      + in[2+5*8/2].real() + in[2+5*8/2].imag()
      - in[3+5*8/2].real() - in[3+5*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 7;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   temp = (((unsigned)out[1])<<24) | 
	  (((unsigned)out[1+1])<<16) | 
	  (((unsigned)out[1+2])<<8) | 
	  (((unsigned)out[1+3]));
   temp &= ~(((1<<3)-1) << (32-7-3));
   temp |= maxCorrelationIndex << (32-7-3);
   out[1]   = (temp>>24) & 0xff;
   out[1+1] = (temp>>16) & 0xff;
   out[1+2] = (temp>>8 ) & 0xff;
   out[1+3] = (temp    ) & 0xff;


   maxCorrelationValue = 0.0;
   currentCorrelationValue =
      - in[0+6*8/2].real() - in[0+6*8/2].imag()
      - in[1+6*8/2].real() - in[1+6*8/2].imag()
      - in[2+6*8/2].real() - in[2+6*8/2].imag()
      - in[3+6*8/2].real() - in[3+6*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 0;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+6*8/2].real() - in[0+6*8/2].imag()
      - in[1+6*8/2].real() - in[1+6*8/2].imag()
      - in[2+6*8/2].real() - in[2+6*8/2].imag()
      - in[3+6*8/2].real() - in[3+6*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 1;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+6*8/2].real() - in[0+6*8/2].imag()
      + in[1+6*8/2].real() + in[1+6*8/2].imag()
      - in[2+6*8/2].real() - in[2+6*8/2].imag()
      + in[3+6*8/2].real() + in[3+6*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 2;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+6*8/2].real() - in[0+6*8/2].imag()
      + in[1+6*8/2].real() + in[1+6*8/2].imag()
      - in[2+6*8/2].real() - in[2+6*8/2].imag()
      + in[3+6*8/2].real() + in[3+6*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 3;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+6*8/2].real() - in[0+6*8/2].imag()
      - in[1+6*8/2].real() - in[1+6*8/2].imag()
      + in[2+6*8/2].real() + in[2+6*8/2].imag()
      + in[3+6*8/2].real() + in[3+6*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 4;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+6*8/2].real() - in[0+6*8/2].imag()
      - in[1+6*8/2].real() - in[1+6*8/2].imag()
      + in[2+6*8/2].real() + in[2+6*8/2].imag()
      + in[3+6*8/2].real() + in[3+6*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 5;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+6*8/2].real() - in[0+6*8/2].imag()
      + in[1+6*8/2].real() + in[1+6*8/2].imag()
      + in[2+6*8/2].real() + in[2+6*8/2].imag()
      - in[3+6*8/2].real() - in[3+6*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 6;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+6*8/2].real() - in[0+6*8/2].imag()
      + in[1+6*8/2].real() + in[1+6*8/2].imag()
      + in[2+6*8/2].real() + in[2+6*8/2].imag()
      - in[3+6*8/2].real() - in[3+6*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 7;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   temp = (((unsigned)out[2])<<24) | 
	  (((unsigned)out[2+1])<<16) | 
	  (((unsigned)out[2+2])<<8) | 
	  (((unsigned)out[2+3]));
   temp &= ~(((1<<3)-1) << (32-2-3));
   temp |= maxCorrelationIndex << (32-2-3);
   out[2]   = (temp>>24) & 0xff;
   out[2+1] = (temp>>16) & 0xff;
   out[2+2] = (temp>>8 ) & 0xff;
   out[2+3] = (temp    ) & 0xff;


   maxCorrelationValue = 0.0;
   currentCorrelationValue =
      - in[0+7*8/2].real() - in[0+7*8/2].imag()
      - in[1+7*8/2].real() - in[1+7*8/2].imag()
      - in[2+7*8/2].real() - in[2+7*8/2].imag()
      - in[3+7*8/2].real() - in[3+7*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 0;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+7*8/2].real() - in[0+7*8/2].imag()
      - in[1+7*8/2].real() - in[1+7*8/2].imag()
      - in[2+7*8/2].real() - in[2+7*8/2].imag()
      - in[3+7*8/2].real() - in[3+7*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 1;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+7*8/2].real() - in[0+7*8/2].imag()
      + in[1+7*8/2].real() + in[1+7*8/2].imag()
      - in[2+7*8/2].real() - in[2+7*8/2].imag()
      + in[3+7*8/2].real() + in[3+7*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 2;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+7*8/2].real() - in[0+7*8/2].imag()
      + in[1+7*8/2].real() + in[1+7*8/2].imag()
      - in[2+7*8/2].real() - in[2+7*8/2].imag()
      + in[3+7*8/2].real() + in[3+7*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 3;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+7*8/2].real() - in[0+7*8/2].imag()
      - in[1+7*8/2].real() - in[1+7*8/2].imag()
      + in[2+7*8/2].real() + in[2+7*8/2].imag()
      + in[3+7*8/2].real() + in[3+7*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 4;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+7*8/2].real() - in[0+7*8/2].imag()
      - in[1+7*8/2].real() - in[1+7*8/2].imag()
      + in[2+7*8/2].real() + in[2+7*8/2].imag()
      + in[3+7*8/2].real() + in[3+7*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 5;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+7*8/2].real() - in[0+7*8/2].imag()
      + in[1+7*8/2].real() + in[1+7*8/2].imag()
      + in[2+7*8/2].real() + in[2+7*8/2].imag()
      - in[3+7*8/2].real() - in[3+7*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 6;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   currentCorrelationValue =
      - in[0+7*8/2].real() - in[0+7*8/2].imag()
      + in[1+7*8/2].real() + in[1+7*8/2].imag()
      + in[2+7*8/2].real() + in[2+7*8/2].imag()
      - in[3+7*8/2].real() - in[3+7*8/2].imag();
   if (abs(currentCorrelationValue) > maxCorrelationValue)
   {
      maxCorrelationValue = abs(currentCorrelationValue);
      maxCorrelationIndex = 7;
      maxCorrelationSign = ((currentCorrelationValue > 1) ? 1 : 0);
   }
   temp = (((unsigned)out[2])<<24) | 
	  (((unsigned)out[2+1])<<16) | 
	  (((unsigned)out[2+2])<<8) | 
	  (((unsigned)out[2+3]));
   temp &= ~(((1<<3)-1) << (32-5-3));
   temp |= maxCorrelationIndex << (32-5-3);
   out[2]   = (temp>>24) & 0xff;
   out[2+1] = (temp>>16) & 0xff;
   out[2+2] = (temp>>8 ) & 0xff;
   out[2+3] = (temp    ) & 0xff;




   consume_each(3 * 8 / 3 * 8);

   return (3);
}

} /* namespace spreader */
} /* namespace gr */

