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
#include "packet_sink_impl.h"

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
static int TCP_PORT = 5125;
static char* HOST_NAME = "idb2";

namespace gr {
  namespace packetizer {

    packet_sink::sptr
    packet_sink::make()
    {
      return gnuradio::get_initial_sptr (new packet_sink_impl());
    }

    /*
     * The private constructor
     */
    packet_sink_impl::packet_sink_impl()
      : gr_sync_block("packet_sink",
		      gr_make_io_signature(MIN_IN, MAX_IN, sizeof (unsigned char)),
		      gr_make_io_signature(0, 0, 0))
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
       clnt_addr.sin_port = htons(33334);
       bind(_fd, (struct sockaddr *) &clnt_addr, sizeof(clnt_addr));

       // connect
       connect(_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));

       _index = 0;
    }

    /*
     * Our virtual destructor.
     */
    packet_sink_impl::~packet_sink_impl()
    {
       close(_fd);
    }

    int
    packet_sink_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
        int ninput_items = noutput_items;
        const unsigned char *in = (const unsigned char *) input_items[0];

	int data_to_read = std::min(PACKET_LEN-_index, ninput_items);
	memcpy(_packet+_index, in, data_to_read);
	_index += data_to_read;

	if (_index == PACKET_LEN)
	{
	   unsigned char dummy[4];
	   recv(_fd, dummy, 4, 0);
	   send(_fd, _packet, PACKET_LEN, 0);
	   _index = 0;
	}

        return data_to_read;
    }

  } /* namespace packetizer */
} /* namespace gr */

