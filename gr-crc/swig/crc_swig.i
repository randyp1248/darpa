/* -*- c++ -*- */

#define CRC_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "crc_swig_doc.i"

%{
#include "crc/crctx.h"
#include "crc/crcrx.h"
%}


%include "crc/crctx.h"
GR_SWIG_BLOCK_MAGIC2(crc, crctx);
%include "crc/crcrx.h"
GR_SWIG_BLOCK_MAGIC2(crc, crcrx);
