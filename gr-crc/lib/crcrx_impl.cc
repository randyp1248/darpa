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
//	set_min_noutput_items((_filesize % FRAME_SIZE) + 4);	//Last Frame is remainder of (filesize/FrameSize + 4)
	set_min_noutput_items(1);				//Last Frame is remainder of (filesize/FrameSize + 4)
	set_max_noutput_items(FRAME_SIZE);			//Fix the maximum output buffer size (Tx_Capsule - CRC)
	frameCount = 0;
	lastFrameSize = (_filesize % FRAME_SIZE) + 4;
	lastFrameCount = floor(_filesize/FRAME_SIZE);
	firstFrameflag = true;
	frameCount = 1;

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
	//int remainder;
	//int numOfFrames;
	//int frameCount;
	current_ninputs = ninput_items[0];					
        const char * pInput = input;

	// **** Calcalulate CRC of Data and EXTRACT CRC from DATA         
        crc32 = digital_crc32((const unsigned char *)input, FRAME_SIZE);	
	memcpy((char *)&crctemp, input + FRAME_SIZE, 4);
       
	//printf ("*** Current n Inputs = %d\n", current_ninputs);
	
	// Testing CRC 
	if(crc32 == crctemp) 
        {
           printf ("***RX CRC IS GOOD: Frame Count = %d\n", frameCount);	

	   //First Frame get filesize
	   if (frameCount == 1)						
           {
		memcpy(&_filesize, input, 4);		
		numOfFrames = floor((_filesize + 4)/FRAME_SIZE);
		remainder = _filesize - ((FRAME_SIZE * numOfFrames) - 4); 	
		printf ("*** RX FILESIZE *** %d\n", _filesize);
		//printf ("*** RX REMAINDER = %d\n", remainder);
		//printf ("** RX num of FRAMES: %d,\n", numOfFrames);
		// First Frame is last Frame: skip filesize and extract data
		if (numOfFrames == 0)					
		{
	    	  // std::cerr << "** RX MADE IT TO FIRST AND ONLY FRAME \n";
 		   dataToCopy = _filesize;					
		   pInput +=4;		
		   memcpy(out, pInput, dataToCopy);
   		   //printf ("****** RX OUTPUT1: %s\n", out);
		}
		// First Frame is not the only frame
		else
		{
 	    	//   std::cerr << "** RX MADE IT TO FIRST FRAME ... There's More * \n";
		   dataToCopy = FRAME_SIZE-4;					
		   pInput +=4;
		   memcpy(out, pInput, dataToCopy);
		   //printf ("****** RX OUTPUT2: %s\n", out);
		}
	   }
	   // Done with first Frame now lets get Middle Frame
	   else if ((frameCount >= 2) && (frameCount <= numOfFrames))
	   {		
        	//std::cerr << "** RX MADE IT TO MIDDLE FRAME  * \n";
		dataToCopy = FRAME_SIZE;					
		memcpy(out, input, dataToCopy);
		//printf ("****** RX OUTPUT3: %s\n", out);
	   }			
	   // Now let's get the last frame (full or partial)
	   else if (frameCount > numOfFrames) 				
           {
        	//std::cerr << "** RX MADE IT TO LAST FRAME  * \n";
		dataToCopy = remainder;						
		memcpy(out, input, dataToCopy);
		//printf ("****** RX OUTPUT4: %s\n", out);	
	   }			
	   // There must have been some problem 
 	   else if (_filesize == 0)						
	   {			
        	std::cerr << "** RX there was a problem  * \n";
		return -1;	
	   }
	        //printf ("****** RX File Size: %d\n", _filesize);
		//printf ("***** RX Frame Size: %d\n", FRAME_SIZE);
		//printf ("***** RX FrameCount: %d,\n", frameCount);
		//printf ("** RX num of FRAMES: %d,\n", numOfFrames);
		//printf (" RX current_ninputs: %d,\n", current_ninputs);
		//printf (" ***************************************************************\n");
	   ++frameCount;
	   consume_each (FRAME_SIZE + 4);
	   return (dataToCopy);			
	}
	else
        {
	   printf("BAD RX CRC: %d\n", crc32);
	   consume_each (current_ninputs);
	   return (0);
        }	
    }
  } /* namespace crc */
} /* namespace gr */

