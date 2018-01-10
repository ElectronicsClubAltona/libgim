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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */

#include "adler.hpp"

#include "fletcher.hpp"
#include "../debug.hpp"

static constexpr unsigned MODULUS = 65521;

using util::hash::adler32;


///////////////////////////////////////////////////////////////////////////////
adler32::adler32 ():
    fletcher (MODULUS, 1, 0)
{ ; }


/////////////////////////////////////////////////////////////////////////////////
//uint32_t
//util::hash::adler32 (const void* restrict _data, size_t _size) noexcept
//{
//    return adler32 (
//        static_cast<const uint8_t*> (_data),
//        static_cast<const uint8_t*> (_data) + _size
//    );
//}
//
//
////-----------------------------------------------------------------------------
//uint32_t
//util::hash::adler32 (const uint8_t *restrict first, const uint8_t *restrict last) noexcept
//{
//    return fletcher<32, MODULUS, 1, 0> (first, last - first);
//}
