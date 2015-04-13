/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

