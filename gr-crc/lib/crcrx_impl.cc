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
#include "crcrx_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <digital_crc32.h>
#include "TRITONS.h"
#include <cstring>
//#define FRAME_SIZE CAPSULE_SYMBOL_LENGTH
#define FRAME_SIZE 183

namespace gr {
  namespace crc {

    crcrx::sptr
    crcrx::make()
    //crcrx::make(int filesize)
    {
      //return gnuradio::get_initial_sptr (new crcrx_impl(filesize));
      return gnuradio::get_initial_sptr (new crcrx_impl());
    }

    //**********************************************************************	
    //**************** The private constructor *****************************
    //**********************************************************************	
    crcrx_impl::crcrx_impl()
    //crcrx_impl::crcrx_impl(int filesize)
      : gr_block("crcrx",
		      gr_make_io_signature(1,1, sizeof (char)),	//input signature
		      gr_make_io_signature(1,1, sizeof (char)))	//output signature    
    {
	set_min_noutput_items(FRAME_SIZE);			//Last Frame is remainder of (filesize/FrameSize + 4)
	set_max_noutput_items(FRAME_SIZE);			//Fix the maximum output buffer size (Tx_Capsule - CRC)

   }

    /* Our virtual destructor */
    crcrx_impl::~crcrx_impl()
    { }

    //**********************************************************************	
    //********************** FORECAST **************************************
    //**********************************************************************	
    void
    crcrx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = FRAME_SIZE + 4;
    }

    void
    crcrx_impl::receiveFrame(unsigned short seqNum, unsigned char subSeqNum, unsigned char* payload)
    {
        if (seqNum != currentSeqNum)
	{
	    currentSeqNum = seqNum;
	    receptionBitmap = 0;
	}
        memcpy(buffer+subSeqNum*180, payload, 180);
        receptionBitmap |= 1<<subSeqNum;

	if (receptionBitmap == 0xff)
	{
	    // TODO: Submit the buffer to the socket server
	    receptionBitmap = 0;
	}
    }

    //**********************************************************************	
    //********************* GENERAL WORK ***********************************
    //**********************************************************************	
    int
    crcrx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
	const char * input = (const char *) input_items[0];
        char * out = (char *) output_items[0];
	int dataToCopy;

/***********************************************************************************************/	
 
	int static initial_ninputs = ninput_items[0];			// Locks value to first read
	int crctemp;
        int crcRcvd;	
	int crc32;
        const char * pInput = input;

	// **** Calculate CRC of Data and EXTRACT CRC from DATA         
        crc32 = digital_crc32((const unsigned char *)input, FRAME_SIZE);	
	memcpy((char *)&crctemp, input + FRAME_SIZE, 4);
       
	// Testing CRC 
	if(crc32 == crctemp) 
        {
            printf ("***RX CRC IS GOOD\n");	

            receiveFrame(
	        (((unsigned short)(input[0]))<<8) | ((unsigned short)(input[1])),
	        (unsigned char)(input[2]),
	        (unsigned char*) (input+3));

	}
	else
        {
	    printf("BAD RX CRC: %d\n", crc32);
        }	
        consume_each (FRAME_SIZE + 4);
        return 0;
    }
  } /* namespace crc */
} /* namespace gr */

