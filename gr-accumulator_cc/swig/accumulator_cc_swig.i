/* -*- c++ -*- */

#define ACCUMULATOR_CC_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "accumulator_cc_swig_doc.i"

%{
#include "accumulator_cc/accumulator_sync_cc.h"
#include "accumulator_cc/expected_cc.h"
%}


%include "accumulator_cc/accumulator_sync_cc.h"
GR_SWIG_BLOCK_MAGIC2(accumulator_cc, accumulator_sync_cc);
%include "accumulator_cc/expected_cc.h"
GR_SWIG_BLOCK_MAGIC2(accumulator_cc, expected_cc);

