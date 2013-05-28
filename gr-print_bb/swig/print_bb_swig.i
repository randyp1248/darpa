/* -*- c++ -*- */

#define PRINT_BB_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "print_bb_swig_doc.i"

%{
#include "print_bb/print_bb.h"
%}


%include "print_bb/print_bb.h"
GR_SWIG_BLOCK_MAGIC2(print_bb, print_bb);
