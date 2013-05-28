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
#include "crcrx_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <digital_crc32.h>
#include "TRITONS.h"
#include <cstring>
#define FRAME_SIZE CAPSULE_SYMBOL_LENGTH

namespace gr {
  namespace crc {

    crcrx::sptr
    crcrx::make()
    {
      return gnuradio::get_initial_sptr (new crcrx_impl());
    }

    /* The private constructor */
    crcrx_impl::crcrx_impl()
      : gr_block("crcrx",
		      gr_make_io_signature(1,1, sizeof (char)),	//input signature
		      gr_make_io_signature(1,1, sizeof (char)))	//output signature    
    {
	set_min_noutput_items(FRAME_SIZE);	//Fix the minimum output buffer size (Tx_Capsule - CRC)
	set_max_noutput_items(FRAME_SIZE);	//Fix the maximum output buffer size (Tx_Capsule - CRC)
    }

    /* Our virtual destructor */
    crcrx_impl::~crcrx_impl()
    { }

    void
    crcrx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
	ninput_items_required[0] = noutput_items + 4;	
    }

    int
    crcrx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
	const char * input = (const char *) input_items[0];
        char * out = (char *) output_items[0];

/***********************************************************************************************/	
/*************** Input data, calculate CRC and Append (4 bytes) ********************************/

	int static initial_ninputs = ninput_items[0];		// Locks value to first read
	unsigned char * crctemp[4];
        int crcRcvd;	
	int crc32;
	std::cout << "RX INPUT = " << input << std::endl;		// Print the input buffer

	if (ninput_items[0] < FRAME_SIZE+4)	
	    return (-1);					//terminate
	else
	{
	   memcpy(out, input, FRAME_SIZE);			
	   crc32 = digital_crc32(out);			
	   memcpy(crctemp, (unsigned char *) (void *) input + FRAME_SIZE, 4);
           crcRcvd = *((int*)(input+FRAME_SIZE));	
	}

        std::cout << "CRC = " << crc32 << std::endl;
        std::cout << "CRCRcvd = " << crcRcvd << std::endl;
	if (crcRcvd != crc32)
	{ 
		std::cout << "CRC NOT EQUAL" << out << std::endl;		
		printf("****\n");
	}
	
	std::cout << "RX OUTPUT WITHOUT CRC = " << out << std::endl;		

	consume_each (FRAME_SIZE+4);
	return noutput_items;			//FRAME_SIZE
    }
  } /* namespace crc */
} /* namespace gr */

