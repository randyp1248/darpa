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
//#define FRAME_SIZE CAPSULE_SYMBOL_LENGTH
#define FRAME_SIZE 183

static bool debug_var_rx = false;

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
	set_min_noutput_items(FRAME_SIZE -4);	//Fix the minimum output buffer size (Tx_Capsule - CRC)
	set_max_noutput_items(FRAME_SIZE);	//Fix the maximum output buffer size (Tx_Capsule - CRC)
	filesize = 0;
        count = 0;
        passed = 0;
    }

    /* Our virtual destructor */
    crcrx_impl::~crcrx_impl()
    { }

    void
    crcrx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = FRAME_SIZE + 4;
    }

    int
    crcrx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
	const char * input = (const char *) input_items[0];
        char * out = (char *) output_items[0];
	int dataToCopy = FRAME_SIZE;
        static int crcrx_entercount =0, crcrx_exitcount =0;

/***********************************************************************************************/	
/*************** Input data, calculate CRC and Append (4 bytes) ********************************/

	int static initial_ninputs = ninput_items[0];		// Locks value to first read
	int crctemp;
        int crcRcvd;	
	int crc32;

  	//std::cout << "RX INPUT BUFFER RECEIVED = " << input << std::endl;		// Print the input buffer
        //printf("CRC RX Enter Count =%d Bytes Given =%d\n",++crcrx_entercount,ninput_items[0]);
	// if we got less than framesize then we're done
	if (ninput_items[0] < FRAME_SIZE+4)	
	{
            std::cout << "RX EOF "<< std::endl;
	    return (-1);					//terminate at end of file
	}	
	else 
	{
            //char *pOut = out;
	    const char * pInput = input;
            memcpy((char *)&crctemp, input + FRAME_SIZE, 4);
            crc32 = digital_crc32((const unsigned char *)input, FRAME_SIZE);	
		
	    if(crc32 == crctemp)
            {
	        if(flag == false)
                {
		    filesize = *(int *)input;
		    flag = true;
		    dataToCopy = FRAME_SIZE - 4;	
		    pInput +=4;
		}
	        memcpy(out, pInput, dataToCopy);			//copy x 4byte CRC to data capsule
                if(debug_var_rx == false) 
                {
                  if(flag == true) 
	          {
                    debug_var_rx = true;
                  }
                 // std::cout << "RX OUTPUT on First Iteration = " << out << std::endl;
                }
                count++; 
                passed++;
                printf("CRC PASS %d passed of %d\n", passed, count);
	    }
	    else
	    {
	   	//std::cout << "CRC FAILED \n " << out << std::endl;
                count++;
                printf("CRC FAILED %d passed of %d\n", passed, count);
                //printf("CRC RX Exit Count =%d Bytes after recomving CRC =%d\n",++crcrx_exitcount,dataToCopy);
/*
for (int i=0; i<FRAME_SIZE; ++i)
{
   printf("0x%08x\n", input[i]);
}
*/
                consume_each(FRAME_SIZE +4);		
	        return 0;					//terminate at end of file
	    }
	}

	//std::cout << "RX OUTPUT WITHOUT CRC = " << out << std::endl;		
        //printf("CRC RX Exit Count =%d Bytes after recomving CRC =%d\n",++crcrx_exitcount,dataToCopy);

	consume_each (FRAME_SIZE + 4);
	return dataToCopy;			//FRAME_SIZE
    }
  } /* namespace crc */
} /* namespace gr */

