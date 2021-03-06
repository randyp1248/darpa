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

#ifndef INCLUDED_CRC_CRCRX_IMPL_H
#define INCLUDED_CRC_CRCRX_IMPL_H

#include <crc/crcrx.h>

namespace gr {
  namespace crc {

    class crcrx_impl : public crcrx
    {
    private:
      unsigned short currentSeqNum;
      unsigned char receptionBitmap;
      unsigned char buffer[1440];

      void receiveFrame(unsigned short seqNum, unsigned char subSeqNum, unsigned char* payload);
			
    public:
      crcrx_impl();
      ~crcrx_impl();

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      // Where all the action really happens
      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace crc
} // namespace gr

#endif /* INCLUDED_CRC_CRCRX_IMPL_H */

