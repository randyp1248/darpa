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
#include "go_start_cc_impl.h"
#include "TRITONS.h"


namespace gr {
  namespace correlator_cc {

    go_start_cc::sptr
    go_start_cc::make()
    {
      return gnuradio::get_initial_sptr (new go_start_cc_impl());
    }

    /*
     * The private constructor
     */
    go_start_cc_impl::go_start_cc_impl()
      : gr_block("go_start_cc",
		      gr_make_io_signature(2, 2, sizeof (gr_complex)),
		      gr_make_io_signature(1, 1, sizeof (gr_complex)))
    {
        //_instance = this;
        _server = true;

        set_min_noutput_items(CAPSULE_SYMBOL_LENGTH);
        set_max_noutput_items(CAPSULE_SYMBOL_LENGTH);
    }

    /*
     * Our virtual destructor.
     */
    go_start_cc_impl::~go_start_cc_impl()
    {
    }

    void
    go_start_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        if(_server)
        {
            ninput_items_required[0] = CAPSULE_SYMBOL_LENGTH;
        }
        else ninput_items_required[1] = CAPSULE_SYMBOL_LENGTH;
    }

    int
    go_start_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in_data = (const gr_complex *)input_items[0];
        const gr_complex *in_correlator = (const gr_complex *)input_items[1];
        gr_complex *out = (gr_complex *) output_items[0];

        if(_server)
        {
            for(int i=0; i<ninput_items[0]; i++)
            {
                out[i] = in_data[i];
            }
            _server = false;
        }
        else
        { 
            for(int i=0; i<ninput_items[1]; i++)
            { 
                out[i] = in_correlator[i];
            }
        }
 
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace correlator_cc */
} /* namespace gr */

