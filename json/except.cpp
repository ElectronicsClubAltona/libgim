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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "except.hpp"

#include "../format.hpp"


///////////////////////////////////////////////////////////////////////////////
json::parse_error::parse_error (const std::string &_what, size_t _line):
    error (_what),
    line  (_line),
    desc  (_what + " at line " + std::to_string (_line))
{ ; }


//-----------------------------------------------------------------------------
const char*
json::parse_error::what (void) const noexcept
{
    return desc.c_str ();
}


///////////////////////////////////////////////////////////////////////////////
json::key_error::key_error (std::string _key):
    error (to_string (util::format::printf ("missing key '%s'", _key))),
    key (_key)
{ ; }
