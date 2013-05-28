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
#include "print_bb_impl.h"
#include <stdio.h>
#include <stdlib.h>

namespace gr {
  namespace print_bb {

    print_bb::sptr
    print_bb::make()
    {
      return gnuradio::get_initial_sptr (new print_bb_impl());
    }

    /*
     * The private constructor
     */
    print_bb_impl::print_bb_impl()
      : gr_sync_block("print_bb",
		      gr_make_io_signature(1, 1, sizeof (unsigned char)),
		      gr_make_io_signature(1, 1, sizeof (unsigned char)))
    {}

    /*
     * Our virtual destructor.
     */
    print_bb_impl::~print_bb_impl()
    {
    }

    int
    print_bb_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        const unsigned char *in = (const unsigned char *) input_items[0];
        unsigned char *out = (unsigned char *) output_items[0];

        for(int i=0; i<noutput_items; i++)
        {
            out[i] = in[i];
            printf("element %04d of %04d: 0x%X\n", i, noutput_items, in[i]);
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace print_bb */
} /* namespace gr */

