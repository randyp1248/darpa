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


#ifndef INCLUDED_SPREADER_DESPREADER_CB_H
#define INCLUDED_SPREADER_DESPREADER_CB_H

#include <spreader/api.h>
#include <gr_block.h>

namespace gr {
  namespace spreader {

    /*!
     * \brief <+description of block+>
     * \ingroup spreader
     *
     */
    class SPREADER_API despreader_cb : virtual public gr_block
    {
    public:
       typedef boost::shared_ptr<despreader_cb> sptr;

       /*!
        * \brief Return a shared_ptr to a new instance of spreader::despreader_cb.
        *
        * To avoid accidental use of raw pointers, spreader::despreader_cb's
        * constructor is in a private implementation
        * class. spreader::despreader_cb::make is the public interface for
        * creating new instances.
        */
       static sptr make();
    };

  } // namespace spreader
} // namespace gr

#endif /* INCLUDED_SPREADER_DESPREADER_CB_H */

