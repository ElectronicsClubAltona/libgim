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
 * Copyright 2010-2017 Danny Robson <danny@nerdcruft.net>
 */

#include "colour.hpp"

#include "ascii.hpp"
#include "parse.hpp"


///////////////////////////////////////////////////////////////////////////////
static util::srgba4f
parse_hex (util::view<const char*> str)
{
    if (str.size () != strlen ("#012345"))
        throw std::invalid_argument ("expected length of 7");

    if (str[0] != '#')
        throw std::invalid_argument ("expected leading '#'");

    if (!util::ascii::is_hex (str[1]) ||
        !util::ascii::is_hex (str[2]) ||
        !util::ascii::is_hex (str[3]) ||
        !util::ascii::is_hex (str[4]) ||
        !util::ascii::is_hex (str[5]) ||
        !util::ascii::is_hex (str[6]))
    {
        throw std::invalid_argument ("expected hex digits");
    }

    uint8_t r = util::ascii::from_hex (str[1]) << 4u | util::ascii::from_hex (str[2]);
    uint8_t g = util::ascii::from_hex (str[3]) << 4u | util::ascii::from_hex (str[4]);
    uint8_t b = util::ascii::from_hex (str[5]) << 4u | util::ascii::from_hex (str[6]);

    return util::srgba<4,uint8_t> { r, g, b, 255 }.template cast<float> ();
}



///////////////////////////////////////////////////////////////////////////////
template <>
util::srgba4f
util::parse<util::srgba4f> (util::view<const char*> str)
{
    return parse_hex (str);
}