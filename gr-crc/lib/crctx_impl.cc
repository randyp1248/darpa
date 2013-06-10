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
//#define FRAME_SIZE CAPSULE_SYMBOL_LENGTH
#define FRAME_SIZE 183
#define FILE_SIZE 80

static bool debug_var = false;	

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
	flag = false;
    }

    /* Our virtual destructor */
    crctx_impl::~crctx_impl()
    { }

    void
    crctx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        if(flag == false) 
        {
	   ninput_items_required[0] = FRAME_SIZE-4;	//Output - CRC(4 bytes)
	}
	else
	{
	   ninput_items_required[0] = FRAME_SIZE;	//Output - CRC(4 bytes)
	}
    }

    int
    crctx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
	const char * input = (const char *) input_items[0];
        char * out = (char *) output_items[0];
	int dataToCopy = FRAME_SIZE;

/***********************************************************************************************/	
/*************** Input data, calculate CRC and Append (4 bytes) ********************************/

	int static initial_ninputs = ninput_items[0];		// Locks value to first read	
	int crc32;

	//std::cout << "TX INPUT = " << input << std::endl;		// Print the input buffer

	// prepending file size to the data
	if (flag == false) 
	{
	   *((int *) out) = FILE_SIZE;
           //std::cout << "TX OUTPUT with Filesize ONLY = " << out << std::endl;
	}	    			
	// if we got less than framesize then we're done
	if (ninput_items[0] < FRAME_SIZE)	
	{
           // std::cout << "TX EOF "<< std::endl;
	    return (-1);					//terminate at end of file
	}
	// else, add the data
	else
	{
            char *pOut = out;
	    if(flag == false) 
	    {
                pOut += 4;
  		dataToCopy = FRAME_SIZE - 4;
 		flag = true;
	    }
	    memcpy(pOut, input, dataToCopy);			//copy x 4byte CRC to data capsule
            if(debug_var == false) 
            {
               if(flag == true) 
	       {
                  debug_var = true;
               }
               //std::cout << "TX OUTPUT on First Iteration with NO CRC = " << pOut << std::endl;
            }
	    int crc32 = digital_crc32((const unsigned char *)out, FRAME_SIZE);						
	    memcpy(out + FRAME_SIZE, (unsigned char *)&crc32, 4);	
	}		
	//std::cout << "TX OUTPUT with CRC = " << out << std::endl;		

	consume_each (dataToCopy);
	return noutput_items;			//FRAME_SIZE + 4
    }
  } /* namespace crc */
} /* namespace gr */

