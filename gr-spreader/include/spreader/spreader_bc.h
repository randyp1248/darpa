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


#ifndef INCLUDED_SPREADER_SPREADER_BC_H
#define INCLUDED_SPREADER_SPREADER_BC_H

#include <spreader/api.h>
#include <gr_block.h>

namespace gr {
  namespace spreader {

    /*!
     * \brief <+description of block+>
     * \ingroup spreader
     *
     */
    class SPREADER_API spreader_bc : virtual public gr_block
    {
    public:
       typedef boost::shared_ptr<spreader_bc> sptr;

       /*!
        * \brief Return a shared_ptr to a new instance of spreader::spreader_bc.
        *
        * To avoid accidental use of raw pointers, spreader::spreader_bc's
        * constructor is in a private implementation
        * class. spreader::spreader_bc::make is the public interface for
        * creating new instances.
        */
       static sptr make();
    };

  } // namespace spreader
} // namespace gr

#endif /* INCLUDED_SPREADER_SPREADER_BC_H */

