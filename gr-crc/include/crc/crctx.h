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


#ifndef INCLUDED_CRC_CRCTX_H
#define INCLUDED_CRC_CRCTX_H

#include <crc/api.h>
#include <gr_block.h>

namespace gr {
  namespace crc {

    /*!
     * \brief <+description of block+>
     * \ingroup crc
     *
     */
    class CRC_API crctx : virtual public gr_block
    {
    public:
       typedef boost::shared_ptr<crctx> sptr;

       /*!
        * \brief Return a shared_ptr to a new instance of crc::crctx.
        *
        * To avoid accidental use of raw pointers, crc::crctx's
        * constructor is in a private implementation
        * class. crc::crctx::make is the public interface for
        * creating new instances.
        */
        static sptr make(int filesize);
    };

  } // namespace crc
} // namespace gr

#endif /* INCLUDED_CRC_CRCTX_H */

