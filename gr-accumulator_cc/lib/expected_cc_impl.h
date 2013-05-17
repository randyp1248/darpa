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

#ifndef INCLUDED_ACCUMULATOR_CC_EXPECTED_CC_IMPL_H
#define INCLUDED_ACCUMULATOR_CC_EXPECTED_CC_IMPL_H

#include <accumulator_cc/expected_cc.h>
#include <accumulator_cc/accumulator_sync_cc.h>

#define CAPSULE_SAMPLE_LENGTH 512

namespace gr {
  namespace accumulator_cc {

    class expected_cc_impl : public expected_cc
    {
     private:
      // Nothing to declare in this block.
      gr_complex _expected[CAPSULE_SAMPLE_LENGTH];
      int _soFarIndex;
      accumulator_cc::accumulator_sync_cc *_accumulator;
      int _count;

     public:
      expected_cc_impl();
      ~expected_cc_impl();
      void set_accumulator_cc(accumulator_cc::accumulator_sync_cc *);
      void set_expected_cc(gr_vector_const_void_star &input);
      int get_count();

      // Where all the action really happens
      int work(int noutput_items,
	       gr_vector_const_void_star &input_items,
	       gr_vector_void_star &output_items);
    };

  } // namespace accumulator_cc
} // namespace gr

#endif /* INCLUDED_ACCUMULATOR_CC_EXPECTED_CC_IMPL_H */

