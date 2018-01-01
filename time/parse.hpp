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
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_TIME_PARSE_HPP
#define CRUFT_UTIL_TIME_PARSE_HPP

#include "../view.hpp"

#include <chrono>
#include <ratio>

namespace util::time::iso8601 {
    /// parse ISO8601 formatted datetime strings
    ///
    /// returns nanoseconds since the UNIX epoch (excluding leap seconds).
    ///
    /// recognises fractional seconds up to the numeric limits of
    /// std::chrono::nanoseconds.
    ///
    /// may throw on improperly formatted strings or unrepresentable values.
    std::chrono::nanoseconds
    parse (util::view<const char*>);
}

#endif
