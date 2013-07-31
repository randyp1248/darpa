/* -*- c++ -*- */

#define PACKETIZER_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "packetizer_swig_doc.i"

%{
#include "packetizer/packet_source.h"
#include "packetizer/packet_sink.h"
%}


%include "packetizer/packet_source.h"
GR_SWIG_BLOCK_MAGIC2(packetizer, packet_source);

%include "packetizer/packet_sink.h"
GR_SWIG_BLOCK_MAGIC2(packetizer, packet_sink);
