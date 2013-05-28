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

#ifndef INCLUDED_RSCODING_BB_ENCODE_BB_IMPL_H
#define INCLUDED_RSCODING_BB_ENCODE_BB_IMPL_H

#include <rscoding_bb/encode_bb.h>

namespace gr {
  namespace rscoding_bb {

    class encode_bb_impl : public encode_bb
    {
    private:
      // Nothing to declare in this block.

    public:
      encode_bb_impl();
      ~encode_bb_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      // Where all the action really happens
      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);

      void *d_rs_ptr;
    };

  } // namespace rscoding_bb
} // namespace gr

#endif /* INCLUDED_RSCODING_BB_ENCODE_BB_IMPL_H */

