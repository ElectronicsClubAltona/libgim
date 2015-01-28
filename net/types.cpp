/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2011 Danny Robson <danny@nerdcruft.net>
 */

#include "types.hpp"
#include "../debug.hpp"

#if defined(HAVE_WINSOCK2_H)
#else
	#include <netdb.h>
#endif

//-----------------------------------------------------------------------------
using namespace net;


//-----------------------------------------------------------------------------
protocol
net::string_to_protocol (const std::string &_name)
    { return net::string_to_protocol (_name.c_str ()); }


protocol
net::string_to_protocol (const char *_name) {
    struct protoent *entry = getprotobyname (_name);
    // TODO: Throw an exception...
    CHECK (entry);
    return (protocol)entry->p_proto;
}


std::string
net::protocol_to_string (protocol _protocol) {
    struct protoent *entry = getprotobynumber ((int)_protocol);
    CHECK (entry);
    return entry->p_name;
}

