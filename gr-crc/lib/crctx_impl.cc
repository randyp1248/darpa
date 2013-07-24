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

 /* Modified by Dave Kier 5/9/13 for Darpa/UCSD Capstone */


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
//#define FRAME_SIZE CAPSULE_SYMBOL_LENGTH
#define REPETITION (3)
#define FRAME_SIZE (183)

namespace gr {
  namespace crc {

    crctx::sptr
    crctx::make(int filesize)
    {
      return gnuradio::get_initial_sptr (new crctx_impl(filesize));
    }

    //**********************************************************************	
    //**************** The private constructor *****************************
    //**********************************************************************	
    crctx_impl::crctx_impl(int filesize)
      : gr_block("crctx",
		      gr_make_io_signature(1,1, sizeof (char)),	//input signature
		      gr_make_io_signature(1,1, sizeof (char)))	//output signature
    {
	set_min_noutput_items(REPETITION*8*(FRAME_SIZE+4));	//Fix the minimum output buffer size #REP * (CRC + Capsule)
	set_max_noutput_items(REPETITION*8*(FRAME_SIZE+4));	//Fix the maximum output buffer size #REP * (CRC + Capsule)
    }

    /***********************************************/
    /********** Our virtual destructor *************/
    /***********************************************/
    
    crctx_impl::~crctx_impl()
    { }


    /***********************************************/
    /**************** FOREAST **********************/
    /***********************************************/
                void
    crctx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {	
        ninput_items_required[0] = 1440;
    }

    //**********************************************************************	
    //********************* GENERAL WORK ***********************************
    //**********************************************************************	
    int
    crctx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
	const char * input = (const char *) input_items[0];
        char * out = (char *) output_items[0];

	for(int rep=0; rep<REPETITION; ++rep)
	{
	    for(int subSeqNum=0; subSeqNum<8; ++subSeqNum)
	    {
	        out[(rep*8+subSeqNum)*187 + 0] = input[6];
	        out[(rep*8+subSeqNum)*187 + 1] = input[7];
	        out[(rep*8+subSeqNum)*187 + 2] = subSeqNum;
		memcpy(out+(rep*8+subSeqNum)*187+3, input+subSeqNum*180, 180);					
 	        *((unsigned int*)(out+(rep*8+subSeqNum)*187+183)) = digital_crc32((const unsigned char *)(out+(rep*8+subSeqNum)*187), 183);		
	    }
	}
	consume_each(1440);
	return (REPETITION*8*(FRAME_SIZE+4));
    }
  } /* namespace crc */
} /* namespace gr */

