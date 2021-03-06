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

#ifndef INCLUDED_CORRELATOR_CC_GO_START_CC_IMPL_H
#define INCLUDED_CORRELATOR_CC_GO_START_CC_IMPL_H

#include <correlator_cc/go_start_cc.h>

namespace gr {
  namespace correlator_cc {

    class go_start_cc_impl : public go_start_cc
    {
    private:
      // Nothing to declare in this block.
      //go_start_cc *_instance;
      bool _server;

    public:
      go_start_cc_impl();
      ~go_start_cc_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      //static go_start_cc *get_instance();

      // Where all the action really happens
      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace correlator_cc
} // namespace gr

#endif /* INCLUDED_CORRELATOR_CC_GO_START_CC_IMPL_H */

