/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
 *
 * Spread input bits with an indexed walsh code and map to constellation.
 *
 * <<<=================================================================>>>
 * <<<=================================================================>>>
 * <<< This file is autogenerated from pngen.pl.  Do not edit directly >>>
 * <<<=================================================================>>>
 * <<<=================================================================>>>
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

#include <gr_io_signature.h>
#include "spreader_bc_impl.h"
#include <stdio.h>

namespace gr {
namespace spreader {

static const int MIN_IN = 1;
static const int MAX_IN = 1;
static const int MIN_OUT = 1;
static const int MAX_OUT = 1;

gr_complex spreader_bc_impl::walshTable[32][32/2] = 
{
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475-0.7071067811865475j)},
   {gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(+0.7071067811865475-0.7071067811865475j),gr_complex(-0.7071067811865475+0.7071067811865475j)}
};

spreader_bc::sptr
spreader_bc::make()
{
  return gnuradio::get_initial_sptr (new spreader_bc_impl());
}

spreader_bc_impl::spreader_bc_impl()
   : gr_block("spreader_bc",
                  gr_make_io_signature(MIN_IN, MAX_IN, sizeof(unsigned char)),
                  gr_make_io_signature(MIN_OUT, MAX_OUT, sizeof(gr_complex)))
{
   set_min_noutput_items(5 * 8 / 5 * 32/2);
   set_max_noutput_items(5 * 8 / 5 * 32/2);
}

spreader_bc_impl::~spreader_bc_impl()
{
}

void
spreader_bc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
{
   ninput_items_required[0] = 5;
}

int
spreader_bc_impl::general_work (int noutput_items,
                   gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items)
{
   const unsigned char* in = (unsigned char*) input_items[0];
   gr_complex* out = (gr_complex*) output_items[0];

   if ((noutput_items < (5 * 8 / 5 * 32/2)) ||
       (ninput_items[0] < 5))
   {
      return 0;
   }

/*
   printf("Spreading index: %d\n",
      (((
         (((unsigned)in[0])<<24) | 
         (((unsigned)in[0+1])<<16) | 
         (((unsigned)in[0+2])<<8) | 
         (((unsigned)in[0+3]))
      ) >> (32-0-5)) & ((1<<5)-1))
   );
*/
   memcpy(out+0*32/2, &walshTable[
      (((
         (((unsigned)in[0])<<24) | 
         (((unsigned)in[0+1])<<16) | 
         (((unsigned)in[0+2])<<8) | 
         (((unsigned)in[0+3]))
      ) >> (32-0-5)) & ((1<<5)-1))
   ][0], 32/2*sizeof(gr_complex));
/*
   printf("Spreading index: %d\n",
      (((
         (((unsigned)in[0])<<24) | 
         (((unsigned)in[0+1])<<16) | 
         (((unsigned)in[0+2])<<8) | 
         (((unsigned)in[0+3]))
      ) >> (32-5-5)) & ((1<<5)-1))
   );
*/
   memcpy(out+1*32/2, &walshTable[
      (((
         (((unsigned)in[0])<<24) | 
         (((unsigned)in[0+1])<<16) | 
         (((unsigned)in[0+2])<<8) | 
         (((unsigned)in[0+3]))
      ) >> (32-5-5)) & ((1<<5)-1))
   ][0], 32/2*sizeof(gr_complex));
/*
   printf("Spreading index: %d\n",
      (((
         (((unsigned)in[1])<<24) | 
         (((unsigned)in[1+1])<<16) | 
         (((unsigned)in[1+2])<<8) | 
         (((unsigned)in[1+3]))
      ) >> (32-2-5)) & ((1<<5)-1))
   );
*/
   memcpy(out+2*32/2, &walshTable[
      (((
         (((unsigned)in[1])<<24) | 
         (((unsigned)in[1+1])<<16) | 
         (((unsigned)in[1+2])<<8) | 
         (((unsigned)in[1+3]))
      ) >> (32-2-5)) & ((1<<5)-1))
   ][0], 32/2*sizeof(gr_complex));
/*
   printf("Spreading index: %d\n",
      (((
         (((unsigned)in[1])<<24) | 
         (((unsigned)in[1+1])<<16) | 
         (((unsigned)in[1+2])<<8) | 
         (((unsigned)in[1+3]))
      ) >> (32-7-5)) & ((1<<5)-1))
   );
*/
   memcpy(out+3*32/2, &walshTable[
      (((
         (((unsigned)in[1])<<24) | 
         (((unsigned)in[1+1])<<16) | 
         (((unsigned)in[1+2])<<8) | 
         (((unsigned)in[1+3]))
      ) >> (32-7-5)) & ((1<<5)-1))
   ][0], 32/2*sizeof(gr_complex));
/*
   printf("Spreading index: %d\n",
      (((
         (((unsigned)in[2])<<24) | 
         (((unsigned)in[2+1])<<16) | 
         (((unsigned)in[2+2])<<8) | 
         (((unsigned)in[2+3]))
      ) >> (32-4-5)) & ((1<<5)-1))
   );
*/
   memcpy(out+4*32/2, &walshTable[
      (((
         (((unsigned)in[2])<<24) | 
         (((unsigned)in[2+1])<<16) | 
         (((unsigned)in[2+2])<<8) | 
         (((unsigned)in[2+3]))
      ) >> (32-4-5)) & ((1<<5)-1))
   ][0], 32/2*sizeof(gr_complex));
/*
   printf("Spreading index: %d\n",
      (((
         (((unsigned)in[3])<<24) | 
         (((unsigned)in[3+1])<<16) | 
         (((unsigned)in[3+2])<<8) | 
         (((unsigned)in[3+3]))
      ) >> (32-1-5)) & ((1<<5)-1))
   );
*/
   memcpy(out+5*32/2, &walshTable[
      (((
         (((unsigned)in[3])<<24) | 
         (((unsigned)in[3+1])<<16) | 
         (((unsigned)in[3+2])<<8) | 
         (((unsigned)in[3+3]))
      ) >> (32-1-5)) & ((1<<5)-1))
   ][0], 32/2*sizeof(gr_complex));
/*
   printf("Spreading index: %d\n",
      (((
         (((unsigned)in[3])<<24) | 
         (((unsigned)in[3+1])<<16) | 
         (((unsigned)in[3+2])<<8) | 
         (((unsigned)in[3+3]))
      ) >> (32-6-5)) & ((1<<5)-1))
   );
*/
   memcpy(out+6*32/2, &walshTable[
      (((
         (((unsigned)in[3])<<24) | 
         (((unsigned)in[3+1])<<16) | 
         (((unsigned)in[3+2])<<8) | 
         (((unsigned)in[3+3]))
      ) >> (32-6-5)) & ((1<<5)-1))
   ][0], 32/2*sizeof(gr_complex));
/*
   printf("Spreading index: %d\n",
      (((
         (((unsigned)in[4])<<24) | 
         (((unsigned)in[4+1])<<16) | 
         (((unsigned)in[4+2])<<8) | 
         (((unsigned)in[4+3]))
      ) >> (32-3-5)) & ((1<<5)-1))
   );
*/
   memcpy(out+7*32/2, &walshTable[
      (((
         (((unsigned)in[4])<<24) | 
         (((unsigned)in[4+1])<<16) | 
         (((unsigned)in[4+2])<<8) | 
         (((unsigned)in[4+3]))
      ) >> (32-3-5)) & ((1<<5)-1))
   ][0], 32/2*sizeof(gr_complex));


   consume_each (5);

   return (5 * 8 / 5 * 32/2);
}

} /* namespace spreader */
} /* namespace gr */

