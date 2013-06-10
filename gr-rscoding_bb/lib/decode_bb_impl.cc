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
#include "decode_bb_impl.h"
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
static const int rs_init_gfpoly  =   137;       // 0x11d
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
static const int rs_init_nroots  =    68;
static const int N = (1 << rs_init_symsize) - 1;	// 255
static const int K = N - rs_init_nroots;		// 223
//#endif

#define DEBUG_DARPA_RSDECODE 0

namespace gr {
  namespace rscoding_bb {

    decode_bb::sptr
    decode_bb::make()
    {
      return gnuradio::get_initial_sptr (new decode_bb_impl());
    }



    /*
     * The private constructor
     */
    decode_bb_impl::decode_bb_impl()
      : gr_block("decode_bb",
		      gr_make_io_signature(1, 1, sizeof (char)),
		      gr_make_io_signature(1, 1, sizeof (char)))
    {
      struct rs *handle;
      set_min_noutput_items(K);
      set_max_noutput_items(K);
      d_rs_ptr = init_rs_char (rs_init_symsize, rs_init_gfpoly,
		       rs_init_fcr, rs_init_prim, rs_init_nroots);
#if DEBUG_DARPA_RSDECODE
      printf("K = %d \n",K );
      printf("Constructor for decode_bb \n" );
      handle = ( struct rs *)d_rs_ptr;
      if( handle == NULL)
      {
        printf("init_rs_char FAILED! \n");
      }
#endif
      assert (d_rs_ptr != 0);
    }



    /*
     * Our virtual destructor.
     */
    decode_bb_impl::~decode_bb_impl()
    {
      if (d_rs_ptr)
      {
         free_rs_char (d_rs_ptr);
      }
      d_rs_ptr = 0;
    }

    void
    decode_bb_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      // ninput_items_required[0] = noutput_items;
      //printf("N = %d \n",N );
      ninput_items_required[0] = N;
    }

    int
    decode_bb_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      unsigned char *in = (unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      int		ncorrections;
      static int rsdecode_entercount =0,rsdecode_exitcount =0;

      memcpy(out, in, N*sizeof(in));
     // printf("RS Decoder Enter Count = %d Bytes Given= %d\n",++rsdecode_entercount,ninput_items[0]);
      
#if DEBUG_DARPA_RSDECODE    
         printf(" Printing Decoder Input array Starts \n \n");
         for(int i=0;i<N;i++)
         {
            printf(" Decoder Input data %d  %c \n ",i,in[i]);
         }
         printf(" Printing Decoder Input array Ends \n \n");
#endif
        // correct message...
        ncorrections = decode_rs_char(d_rs_ptr, &out[0], 0, 0);
printf("RS corrections = %d\n", ncorrections);

#if DEBUG_DARPA_RSDECODE       
         printf(" Printing Output array Starts \n \n");
         for(int i=0;i<K;i++)
         {
            printf(" Output Data %d  %c \n ",i,out[i]);
         }
         printf(" Printing Output array Ends \n \n");
#endif
        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (N);
     
       // printf("RS Decoder Exit  Count = %d Consumed Bytes = %d\n",++rsdecode_exitcount,K);

        // Tell runtime system how many output items we produced.
        return K;
    }

  } /* namespace rscoding_bb */
} /* namespace gr */

