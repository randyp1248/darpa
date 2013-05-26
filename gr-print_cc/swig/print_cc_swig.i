/* -*- c++ -*- */

#define PRINT_CC_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "print_cc_swig_doc.i"

%{
#include "print_cc/print_cc.h"
%}


%include "print_cc/print_cc.h"
GR_SWIG_BLOCK_MAGIC2(print_cc, print_cc);
