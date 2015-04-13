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

#include "string.hpp"

#define do_type_to_string(T)                                                    \
template <> std::string type_to_string <T>       (void) { return #T; }          \
template <> std::string type_to_string <const T> (void) { return "const " #T; }

do_type_to_string (float)
do_type_to_string (double)

do_type_to_string (  int8_t)
do_type_to_string ( int16_t)
do_type_to_string ( int32_t)
do_type_to_string ( int64_t)

do_type_to_string ( uint8_t)
do_type_to_string (uint16_t)
do_type_to_string (uint32_t)
do_type_to_string (uint64_t)

