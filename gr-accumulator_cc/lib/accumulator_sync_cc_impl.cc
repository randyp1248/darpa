/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
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
#include "accumulator_sync_cc_impl.h"

namespace gr {
  namespace accumulator_cc {
  
    static const int MIN_IN = 1;
    static const int MAX_IN = 1;
    static const int MIN_OUT = 1;
    static const int MAX_OUT = 1;

    accumulator_sync_cc::sptr
    accumulator_sync_cc::make()
    {
      return gnuradio::get_initial_sptr (new accumulator_sync_cc_impl());
    }

    /*
     * The private constructor
     */
    //accumulator_sync_cc_impl::accumulator_sync_cc_impl(int capsule_sample_length)
    accumulator_sync_cc_impl::accumulator_sync_cc_impl()
      : gr_sync_block("accumulator_sync_cc",
		      gr_make_io_signature(MIN_IN, MAX_IN, sizeof (gr_complex)),
		      gr_make_io_signature(MIN_OUT, MAX_OUT, sizeof (gr_complex)))
    {
        //_capsuleSampleLength = capsule_sample_length;
        //_currentFrameSample = new gr_complex[_capsuleSampleLength];
        reset_frame();
    }

    /*
     * Our virtual destructor.
     */
    accumulator_sync_cc_impl::~accumulator_sync_cc_impl()
    {
        //delete[] _currentFrameSample;
    }

    void
    accumulator_sync_cc_impl::reset_frame()
    {
        // Reinitialize each sample
        const gr_complex zero = gr_complex(0, 0);

        // Setting to gr_complex 0 values instead of memset 0
        for(int i=0; i<CAPSULE_SAMPLE_LENGTH; i++) {
        //for(int i=0; i<_capsuleSampleLength; i++) {
            _currentFrameSample[i] = zero;
        }
  
        _accIndex = 0;
    }

    int
    accumulator_sync_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        for(int i=0; i<noutput_items; i++) {
            _currentFrameSample[_accIndex] += in[i];
            out[i] = _currentFrameSample[_accIndex];

            _accIndex++;
 
            // Restart current accumulator back at 0
            //if(_capsuleSampleLength == _accIndex) {
            if(CAPSULE_SAMPLE_LENGTH == _accIndex) {
                _accIndex = 0;
            }
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace accumulator_cc */
} /* namespace gr */

