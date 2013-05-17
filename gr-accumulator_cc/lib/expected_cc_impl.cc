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
#include "expected_cc_impl.h"

namespace gr {
  namespace accumulator_cc {

    expected_cc::sptr
    expected_cc::make()
    {
      return gnuradio::get_initial_sptr (new expected_cc_impl());
    }

    /*
     * The private constructor
     */
    expected_cc_impl::expected_cc_impl()
      : gr_sync_block("expected_cc",
		      gr_make_io_signature(1, 1, sizeof (gr_complex)),
		      gr_make_io_signature(1, 1, sizeof (gr_complex)))
    {
        _accumulator = NULL; 
        _count = 0;
        _soFarIndex = 0;
    }

    /*
     * Our virtual destructor.
     */
    expected_cc_impl::~expected_cc_impl()
    {
    }

    void
    expected_cc_impl::set_accumulator_cc(accumulator_cc::accumulator_sync_cc *accumulator)
    {
        _accumulator = accumulator;
    }

    void
    expected_cc_impl::set_expected_cc(gr_vector_const_void_star &input)
    //expected_cc_impl::set_expected_cc(std::vector<gr_complex> &input)
    {
        const gr_complex *in = (const gr_complex *) input[0];
        for(int i=0; i<CAPSULE_SAMPLE_LENGTH; i++)
        {
            _expected[i] = in[i];
        }
    }

    int
    expected_cc_impl::get_count()
    {
        return _count;
    }

    int
    expected_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        for(int i=0; i<noutput_items; i++) {
            out[i] = in[i];
        }

        for(int i=0; i<noutput_items; i++) {
            if(_expected[_soFarIndex] == in[i])
            {   
                _soFarIndex++;
                if(_soFarIndex == CAPSULE_SAMPLE_LENGTH)
                {
                    _soFarIndex = 0;
                    _count++;

                    // Reset our accumulator
                    _accumulator->reset_frame();
                }
            }
            else
            {
                // If we get a value that doesn't match then start
                // the index all over
                _soFarIndex = 0;
            }
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace accumulator_cc */
} /* namespace gr */

