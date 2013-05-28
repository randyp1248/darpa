/* -*- c++ -*- */

#define SPREADER_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "spreader_swig_doc.i"

%{
#include "spreader/spreader_bc.h"
#include "spreader/despreader_cb.h"
%}


%include "spreader/spreader_bc.h"
GR_SWIG_BLOCK_MAGIC2(spreader, spreader_bc);
%include "spreader/despreader_cb.h"
GR_SWIG_BLOCK_MAGIC2(spreader, despreader_cb);
