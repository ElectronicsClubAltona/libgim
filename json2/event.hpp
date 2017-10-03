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
#ifndef CRUFT_JSON2_EVENT_HPP
#define CRUFT_JSON2_EVENT_HPP

#include "./fwd.hpp"

#include <functional>


namespace util::json2::event {
    // It is important that scalars come after compound types because it
    // simplifies categorisation as we can use a simple '>' to classify the
    // types.
    //
    // the value of the enumerants isn't important, but they might make it
    // fractionally easier to visualise in a debugger on some occasions.
    enum class type_t {
        OBJECT_BEGIN = '{',
        OBJECT_END = '}',
        ARRAY_BEGIN  = '[',
        ARRAY_END  = ']',

        STRING = '"',
        NUMBER = '1',
        BOOLEAN = 't',
        NONE = 'n',
    };

    struct packet {
        type_t type (void) const noexcept;

        const char *first;
        const char *last;
    };

    template <typename PersonalityT = personality::rfc7159>
    const char*
    parse (const std::function<callback_t>&, const char *first, const char *last);
};

#endif