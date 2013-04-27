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


#ifndef INCLUDED_CORRELATOR_CC_CORRELATOR_CC_H
#define INCLUDED_CORRELATOR_CC_CORRELATOR_CC_H

#include <correlator_cc/api.h>
#include <gr_block.h>

namespace gr {
  namespace correlator_cc {

    /*!
     * \brief <+description of block+>
     * \ingroup correlator_cc
     *
     */
    class CORRELATOR_CC_API correlator_cc : virtual public gr_block
    {
     public:
      typedef boost::shared_ptr<correlator_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of correlator_cc::correlator_cc.
       *
       * To avoid accidental use of raw pointers, correlator_cc::correlator_cc's
       * constructor is in a private implementation
       * class. correlator_cc::correlator_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace correlator_cc
} // namespace gr

#endif /* INCLUDED_CORRELATOR_CC_CORRELATOR_CC_H */

