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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <gr_io_signature.h>
#include "encode_bb_impl.h"
//#include "../../../gnuradio/gnuradio-core/src/lib/reed-solomon/char.h"


extern "C" {
#include "rs.h"
}

/* Reed-Solomon codec control block */
struct rs {
  unsigned int mm;              /* Bits per symbol */
  unsigned int nn;              /* Symbols per block (= (1<<mm)-1) */
  unsigned char *alpha_to;      /* log lookup table */
  unsigned char *index_of;      /* Antilog lookup table */
  unsigned char *genpoly;       /* Generator polynomial */
  unsigned int nroots;     /* Number of generator roots = number of parity symbols */
  unsigned char fcr;        /* First consecutive root, index form */
  unsigned char prim;       /* Primitive element, index form */
  unsigned char iprim;      /* prim-th root of 1, index form */
};

#if 0
//RS Data for (n,k) = (63,55)
static const int rs_init_symsize =     6;
static const int rs_init_gfpoly  =    67;       // 0x43
static const int rs_init_fcr     =     1;	// first consecutive root
static const int rs_init_prim    =     1;	// primitive is 1 (alpha)
static const int rs_init_nroots  =    8;
static const int N = (1 << rs_init_symsize) - 1;	// 63
static const int K = N - rs_init_nroots;		// 55
#endif

#if 0
//RS Data for (n,k) = (127,117)
static const int rs_init_symsize =     7;
static const int rs_init_gfpoly  =   137;       // 0x89
static const int rs_init_fcr     =     1;	// first consecutive root
static const int rs_init_prim    =     1;	// primitive is 1 (alpha)
static const int rs_init_nroots  =    10;
static const int N = (1 << rs_init_symsize) - 1;	// 127
static const int K = N - rs_init_nroots;		// 117
#endif

//#if 0
//RS Data for (n,k) = (255,223)
static const int rs_init_symsize =     8;
static const int rs_init_gfpoly  =   285;       // 0x11d
static const int rs_init_fcr     =     1;	// first consecutive root
static const int rs_init_prim    =     1;	// primitive is 1 (alpha)
static const int rs_init_nroots  =    32;
static const int N = (1 << rs_init_symsize) - 1;	// 255
static const int K = N - rs_init_nroots;		// 223
//#endif

#define NNN    (255)
#define DEBUG_DARPA_RSENCODE 0
#define DEBUG_DARPA_RSERROR  0




namespace gr {
  namespace rscoding_bb {

    encode_bb::sptr
    encode_bb::make()
    {
      return gnuradio::get_initial_sptr (new encode_bb_impl());
    }

    /*
     * The private constructor
     */
    encode_bb_impl::encode_bb_impl()
      : gr_block("encode_bb",
		      gr_make_io_signature(1, 1, sizeof (char)),
		      gr_make_io_signature(1, 1, sizeof (char)))
    {
      struct rs *handle;
      set_min_noutput_items(N);
      set_max_noutput_items(N);
      printf("N = %d \n",N );
      d_rs_ptr = init_rs_char(rs_init_symsize, rs_init_gfpoly,
		       rs_init_fcr, rs_init_prim, rs_init_nroots);
#if DEBUG_DARPA_RSENCODE
      printf("Constructor for encode_bb \n" );
      handle = ( struct rs *)d_rs_ptr;
      if( handle == NULL)
      {
        printf("init_rs_char FAILED! \n");
      }
      printf("mm =%d, nn =%d,  nroots =%d, fcr =%u, prim =%u, iprim =%u \n ",handle->mm,handle->nn,handle->nroots,handle->fcr,handle->prim,handle->iprim);
#endif
      assert (d_rs_ptr != 0);
      
    }


    /*
     * Our virtual destructor.
     */
    encode_bb_impl::~encode_bb_impl()
    {
      if (d_rs_ptr)
      {
        free_rs_char(d_rs_ptr);
      }
      d_rs_ptr = 0;
    }

    void
    encode_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      // ninput_items_required[0] = noutput_items;
      ninput_items_required[0] = K;
    }

    int
    encode_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      unsigned char *in =  (unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      int errlocs[NNN];
      int errors,errloc;
      unsigned char errval;

      memcpy(out, in, K*sizeof(in)); 
      encode_rs_char(d_rs_ptr, &out[0], &out[K]);

#if DEBUG_DARPA_RSENCODE
      printf(" Printing Input array Starts \n \n");
      for(int i=0;i<K;i++)
      {
        printf(" Input Data %d  %c \n ",i,in[i]);
      }
      printf(" Printing Input array Ends \n \n");

      printf(" Printing Encoded array Starts \n \n");
      for(int i=0;i<N;i++)
      {
        printf(" Encoded data %d  %c \n ",i,out[i]);
      }
      printf(" Printing Encoded array Ends \n \n");
#endif

#if DEBUG_DARPA_RSERROR
      /*
      for( int i=0;i<2;i++)
      {
        out[i]= (out[i] | (unsigned char)0xF);
      }
      */ 

      errors = 16;

      /* seed with errors */
      memset(errlocs,0,sizeof(errlocs));
      for(int i=0;i<(unsigned int)errors;i++)
      {
	do 
	{
	  errval = (unsigned char)(random() & NNN);
	} while(errval == 0); /* Error value must be nonzero */

	do
	{
	  errloc = random() % NNN;
	} while(errlocs[errloc] != 0); /* Must not choose the same location twice */

	errlocs[errloc] = 1;
	out[errloc] ^= errval;
      }
#endif

#if DEBUG_DARPA_RSENCODE
      printf(" Printing Error Encoded array Starts \n \n");
      for(int i=0;i<N;i++)
      {
        printf(" Error Encoded data %d  %c \n ",i,out[i]);
      }
      printf(" Printing Error Encoded array Ends \n \n");
#endif

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      // consume_each (noutput_items);
      consume_each (K);

      // Tell runtime system how many output items we produced.
      // return noutput_items;
      return N;
    }

  } /* namespace rscoding_bb */
} /* namespace gr */

