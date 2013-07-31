/* -*- c++ -*- */
/* 
 * Copyright 2013 TRITONS
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
#include "packet_source_impl.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <math.h>

static const int MIN_IN = 1;  // mininum number of input streams
static const int MAX_IN = 1;  // maximum number of input streams
#define MIN(X,Y) ((X) < (Y) ? : (X) : (Y))

static int TCP_PORT = 5123;
static char* HOST_NAME = "idb2";

namespace gr {
  namespace packetizer {

    packet_source::sptr
    packet_source::make()
    {
      return gnuradio::get_initial_sptr (new packet_source_impl());
    }

    /*
     * The private constructor
     */
    packet_source_impl::packet_source_impl()
      : gr_sync_block("packet_source",
		      gr_make_io_signature(0, 0, 0),
		      gr_make_io_signature(MIN_IN, MAX_IN, sizeof (unsigned char)))
    {
       struct sockaddr_in serv_addr, clnt_addr;
       struct hostent *server = gethostbyname(HOST_NAME);
       _fd = socket(AF_INET, SOCK_STREAM, 0);

       bzero((char *) &serv_addr, sizeof(serv_addr));
       serv_addr.sin_family = AF_INET;
       bcopy((char *)server->h_addr, 
	    (char *)&serv_addr.sin_addr.s_addr,
	    server->h_length);
       serv_addr.sin_port = htons(TCP_PORT);

       // bind
       clnt_addr.sin_family = AF_INET;
       clnt_addr.sin_addr.s_addr = INADDR_ANY;
       clnt_addr.sin_port = htons(33333);
       bind(_fd, (struct sockaddr *) &clnt_addr, sizeof(clnt_addr));

       // connect
       connect(_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));

       _index = PACKET_LEN;
    }

    /*
     * Our virtual destructor.
     */
    packet_source_impl::~packet_source_impl()
    {
       close(_fd);
    }

    int
    packet_source_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        unsigned char *out = (unsigned char *) output_items[0];

	if (_index == PACKET_LEN)
	{
	   // Read a new packet off the server
	   int pkt_size = PACKET_LEN;
	   send(_fd, &pkt_size, 4, 0);
	   recv(_fd, _packet, pkt_size, 0);
	   _index = 0;
	}

	int data_to_write = std::min(PACKET_LEN-_index, noutput_items);
	memcpy(out, _packet+_index, data_to_write);
	_index += data_to_write;
        return data_to_write;
    }

  } /* namespace packetizer */
} /* namespace gr */

