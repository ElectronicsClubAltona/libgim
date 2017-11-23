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

#ifndef CRUFT_UTIL_JSON2_PERSONALITY_JSONISH_HPP
#define CRUFT_UTIL_JSON2_PERSONALITY_JSONISH_HPP

#include "../fwd.hpp"

#include "./base.hpp"

#include <functional>


namespace util::json2::personality {
    struct jsonish {
        static const char*
        consume_whitespace [[nodiscard]] (
            const char *first,
            const char *last
        ) noexcept;


        static const char*
        parse_value [[nodiscard]] (
            const std::function<callback_t> &cb,
            const char *first, const char *last
        ) { return base<jsonish>::parse_value (cb, first, last); }


        static const char*
        parse_number [[nodiscard]] (
            const std::function<callback_t> &cb,
            const char *first,
            const char *last
        );


        template <int N>
        static const char*
        parse_literal [[nodiscard]] (
            const std::function<callback_t> &cb,
            const char *first,
            const char *last,
            const char (&value)[N]
        ) { return base<jsonish>::parse_literal (cb, first, last, value); }


        static const char*
        parse_string [[nodiscard]] (
            const std::function<callback_t> &cb,
            const char *first,
            const char *last
        );


        static const char*
        parse_array [[nodiscard]] (
            const std::function<callback_t> &cb,
            const char *first,
            const char *last
        ) { return base<jsonish>::parse_array (cb, first, last); }


        static const char*
        parse_key [[nodiscard]] (
            const std::function<callback_t> &cb,
            const char *first,
            const char *last);


        static const char*
        parse_object [[nodiscard]] (
            const std::function<callback_t> &cb,
            const char *first,
            const char *last
        ) { return base<jsonish>::parse_object (cb, first, last); }


        static const char*
        parse_unknown [[nodiscard]] (
            const std::function<callback_t> &cb,
            const char *first,
            const char *last
        ) { return parse_string (cb, first, last); }
    };
};

#endif
