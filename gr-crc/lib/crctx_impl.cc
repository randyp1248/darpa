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
#define FRAME_SIZE 183

static bool debug_var = false;	

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
	set_min_noutput_items(FRAME_SIZE+4);	//Fix the minimum output buffer size (CRC + Capsule)
	set_max_noutput_items(FRAME_SIZE+4);	//Fix the maximum output buffer size (CRC + Capsule)
	firstFrameFlag = true;			//Flag for first frame = true
	_filesize = filesize;
	frameCounter = 0;
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
	if((firstFrameFlag == true) && (_filesize < FRAME_SIZE-4)) 		  // First Frame is last frame: 
	{
	   ninput_items_required[0] = _filesize;				  // Reduce ninput to remaining amount of data
	}       
	if((firstFrameFlag == true) && (_filesize >= FRAME_SIZE-4)) 		  // First Frame but not last 
        {									
	   ninput_items_required[0] = FRAME_SIZE-4;				  // Reduce ninput by 4 bytes for filsize	
	}
	else if (_filesize - ((frameCounter * (FRAME_SIZE)) + 4) < FRAME_SIZE)	  // Last Frame: 
	{
	   ninput_items_required[0] = _filesize - ((frameCounter * (FRAME_SIZE)) + 4); // Reduce ninput to remaining amount of data
	}
	else	//ninput = FULL Frame of Data					  // Middle Frames
	{
	   ninput_items_required[0] = FRAME_SIZE;				  // ninput = FRAME_SIZE
	}
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
	int dataToCopy = FRAME_SIZE;
	unsigned char pad_data[FRAME_SIZE] = {0xFF};
	for (int i=0; i<FRAME_SIZE; i++)
	{
	   pad_data[i] = 0xFF;							//Pad with BLANK (0xFF) Characters
	}
        
	//**********************************************************************************************	
	//************** Determine if First Frame: Middle Frame or Last Frame **************************
	//**********************************************************************************************

	int  crc32;
	current_ninputs = ninput_items[0];		// Locks value to first read	
	//std::cout << "*************** TX ninputs = " << current_ninputs << std::endl;	// Print the input buffer
	//printf ("*** TX FILESIZE *** %d\n", _filesize);

	// **************************************************************************************************
	// First Make sure we have Data. 
	// **************************************************************************************************
	if ((ninput_items[0] <= 0))	
	{
	  //  printf ("***SORRY THERE WAS NO MORE DATA***\n");
	    return -1;	
	}
	// **************************************************************************************************
	// First Frame is the last Frame:  Prepend filesize, Copy Data, Pad with FF's, Append CRC	****
	// **************************************************************************************************
	if ((ninput_items[0] < FRAME_SIZE-4) && (firstFrameFlag == true))		
	{
	    //printf ("TX Made it to 'First/Last Frame'\n");
	    //std::cerr <<  "TX Made it to 'First/Last Frame'\n";
	    dataToCopy = current_ninputs;
	    char *pOut = out;
	    //*((int *) out) = _filesize;							
	    memcpy (&out[0], (char *)&_filesize, 4);		    
            pOut += 4;									
 	    memcpy(pOut, input,  dataToCopy);					
	    pOut += dataToCopy;
	    memcpy(pOut, pad_data, (FRAME_SIZE - (dataToCopy + 4)));		
 	    crc32 = digital_crc32((const unsigned char *)out, FRAME_SIZE);		
	    memcpy(out + FRAME_SIZE, (unsigned char *)&crc32, 4);			
 	    firstFrameFlag = false;	
	   
	    consume_each (dataToCopy);
	    return noutput_items;
	}

	// ******************************************************************************************************
	// First Frame but not last   Prepend filesize, Copy Data, Append CRC				     ****
	// ******************************************************************************************************
	if ((ninput_items[0] >= FRAME_SIZE-4) && (firstFrameFlag == true))
	{
	    //printf ("TX Made it to 'First but not last Frame'\n");
	    //std::cerr << "TX Made it to 'First but not last Frame'\n";
            dataToCopy = FRAME_SIZE - 4;
	    char *pOut = out;
	    *((int *) out) = _filesize;				
            pOut += 4;										
	    memcpy(pOut, input, dataToCopy);				
	    crc32 = digital_crc32((const unsigned char *)out, FRAME_SIZE);					
	    memcpy(out + FRAME_SIZE, (unsigned char *)&crc32, 4);   	
	    firstFrameFlag = false;
	    ++frameCounter;					    	
	    consume_each (dataToCopy);
	    return noutput_items;	
	}

	// ******************************************************************************************************
	// Middle Frame: Copy Data, Append CRC   
	// ******************************************************************************************************

	if ((ninput_items[0] >= FRAME_SIZE) && (firstFrameFlag == false))				
	{
            //printf ("TX Made it to 'MiddleFrame'\n"); 	
            //std::cerr <<  "TX Made it to 'MiddleFrame'\n";

  	    dataToCopy = FRAME_SIZE;				
 	    memcpy(out, input, dataToCopy);				
	    crc32 = digital_crc32((const unsigned char *)out, FRAME_SIZE);					
	    memcpy(out + dataToCopy, (unsigned char *)&crc32, 4);   	
	    ++frameCounter;					    	
	    consume_each (dataToCopy);
	    return noutput_items;	
	}

	// **************************************************************************************************
	// Last Frame -- Copy Data, Pad with FF's, Append CRC						 ****
	// **************************************************************************************************
	if ((ninput_items[0] < FRAME_SIZE) && (firstFrameFlag == false) && (frameCounter > 0))	
	{
	    //printf ("TX Made it to 'LastFrame' \n");
	    //std::cerr << "TX Made it to 'LastFrame' \n";
	    dataToCopy = current_ninputs;
	    memcpy(out, input,  dataToCopy);					
	    memcpy(out + dataToCopy, pad_data, (FRAME_SIZE - dataToCopy));	
 	    crc32 = digital_crc32((const unsigned char *)out, FRAME_SIZE);		
	    memcpy(out + FRAME_SIZE, (unsigned char *)&crc32, 4);			
  	    ++frameCounter;		
  	    consume_each (dataToCopy);
	    return noutput_items;
	}
   }
  } /* namespace crc */
} /* namespace gr */

