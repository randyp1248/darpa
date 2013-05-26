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
#include "print_cc_impl.h"
#include <stdio.h>
#include <stdlib.h>

namespace gr {
  namespace print_cc {

    print_cc::sptr
    print_cc::make()
    {
      return gnuradio::get_initial_sptr (new print_cc_impl());
    }

    /*
     * The private constructor
     */
    print_cc_impl::print_cc_impl()
      : gr_sync_block("print_cc",
		      gr_make_io_signature(1, 1, sizeof (gr_complex)),
		      gr_make_io_signature(1, 1, sizeof (gr_complex)))
    {}

    /*
     * Our virtual destructor.
     */
    print_cc_impl::~print_cc_impl()
    {
    }

    int
    print_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

        for(int i=0; i<noutput_items; i++)
        {
            out[i] = in[i];
            printf("Sample %d: %f+j%f\n", i, in[i].real(), in[i].imag());
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace print_cc */
} /* namespace gr */

