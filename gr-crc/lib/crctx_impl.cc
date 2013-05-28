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
#include "crctx_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <digital_crc32.h>
#include "TRITONS.h"
#include <cstring>
#define FRAME_SIZE CAPSULE_SYMBOL_LENGTH
namespace gr {
  namespace crc {

    crctx::sptr
    crctx::make()
    {
      return gnuradio::get_initial_sptr (new crctx_impl());
    }

    /* The private constructor */
    crctx_impl::crctx_impl()
      : gr_block("crctx",
		      gr_make_io_signature(1,1, sizeof (char)),	//input signature
		      gr_make_io_signature(1,1, sizeof (char)))	//output signature
    {
	set_min_noutput_items(FRAME_SIZE+4);	//Fix the minimum output buffer size (CRC + Capsule)
	set_max_noutput_items(FRAME_SIZE+4);	//Fix the maximum output buffer size (CRC + Capsule)
    }

    /* Our virtual destructor */
    crctx_impl::~crctx_impl()
    { }

    void
    crctx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
	    ninput_items_required[0] = noutput_items-4;	//Output - CRC(4 bytes)
    }

    int
    crctx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
	const char * input = (const char *) input_items[0];
        char * out = (char *) output_items[0];

/***********************************************************************************************/	
/*************** Input data, calculate CRC and Append (4 bytes) ********************************/

	int static initial_ninputs = ninput_items[0];		// Locks value to first read	
	//char crc32[] = {"1234"};
	std::cout << "TX INPUT = " << input << std::endl;		// Print the input buffer

	if (ninput_items[0] < FRAME_SIZE)	
	    return (-1);					//terminate
	else
	{
	    memcpy(out, input, FRAME_SIZE);			
	    int crc32 = digital_crc32(out);			// %%%%% ADD BACK IN AFTER DEBUGGING %%%%%%			
	    memcpy(out + FRAME_SIZE, (unsigned char *) (void *) &crc32, 4);	
	}		
	std::cout << "TX OUTPUT with CRC = " << out << std::endl;		

	consume_each (FRAME_SIZE);
	return noutput_items;			//FRAME_SIZE + 4
    }
  } /* namespace crc */
} /* namespace gr */

