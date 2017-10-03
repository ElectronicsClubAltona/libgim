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

#ifndef CRUFT_UTIL_JSON2_FWD_HPP
#define CRUFT_UTIL_JSON2_FWD_HPP

#include "../preprocessor.hpp"

namespace util::json2 {
    namespace personality {
        template <typename> struct base;

        struct rfc7159;
        struct jsonish;

        #define MAP_JSON2_PERSONALITY_TYPES(FUNC) \
        MAP0(FUNC, \
            util::json2::personality::rfc7159,\
            util::json2::personality::jsonish)
    }

    namespace event {
        enum class type_t;
        struct packet;
    }

    using callback_t = void(const event::packet&);

    struct error;
    struct parse_error;
    struct overrun_error;
}

#endif