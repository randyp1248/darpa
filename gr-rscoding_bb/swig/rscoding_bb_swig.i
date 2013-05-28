/* -*- c++ -*- */

#define RSCODING_BB_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "rscoding_bb_swig_doc.i"

%{
#include "rscoding_bb/encode_bb.h"
#include "rscoding_bb/decode_bb.h"
%}


%include "rscoding_bb/encode_bb.h"
GR_SWIG_BLOCK_MAGIC2(rscoding_bb, encode_bb);
%include "rscoding_bb/decode_bb.h"
GR_SWIG_BLOCK_MAGIC2(rscoding_bb, decode_bb);
