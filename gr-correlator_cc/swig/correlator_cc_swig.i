/* -*- c++ -*- */

#define CORRELATOR_CC_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "correlator_cc_swig_doc.i"

%{
#include "correlator_cc/correlator_cc.h"
#include "correlator_cc/preamble_insert_cc.h"
#include "correlator_cc/go_start_cc.h"
%}


%include "correlator_cc/correlator_cc.h"
GR_SWIG_BLOCK_MAGIC2(correlator_cc, correlator_cc);
%include "correlator_cc/preamble_insert_cc.h"
GR_SWIG_BLOCK_MAGIC2(correlator_cc, preamble_insert_cc);
%include "correlator_cc/go_start_cc.h"
GR_SWIG_BLOCK_MAGIC2(correlator_cc, go_start_cc);
