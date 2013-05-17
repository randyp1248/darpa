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


#ifndef INCLUDED_ACCUMULATOR_CC_ACCUMULATOR_SYNC_CC_H
#define INCLUDED_ACCUMULATOR_CC_ACCUMULATOR_SYNC_CC_H

#include <accumulator_cc/api.h>
#include <gr_sync_block.h>

namespace gr {
  namespace accumulator_cc {

    /*!
     * \brief <+description of block+>
     * \ingroup accumulator_cc
     *
     */
    class ACCUMULATOR_CC_API accumulator_sync_cc : virtual public gr_sync_block
    {
     public:
      typedef boost::shared_ptr<accumulator_sync_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of accumulator_cc::accumulator_sync_cc.
       *
       * To avoid accidental use of raw pointers, accumulator_cc::accumulator_sync_cc's
       * constructor is in a private implementation
       * class. accumulator_cc::accumulator_sync_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make();
      virtual void reset_frame() = 0;
    };

  } // namespace accumulator_cc
} // namespace gr

#endif /* INCLUDED_ACCUMULATOR_CC_ACCUMULATOR_SYNC_CC_H */

