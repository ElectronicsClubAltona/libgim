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
 * Copyright 2011-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_STRING_HPP
#define __UTIL_STRING_HPP

#include "./view.hpp"


bool
strbegins(const char *restrict str,
          const char *restrict prefix);


namespace util {
    struct tokeniser {
    public:
        using string_type = std::string;
        using value_type  = string_type::value_type;

        tokeniser (const std::string &value, char separator);
        tokeniser (std::string &&value, char separator) = delete;

        struct iterator {
        public:
            using range_type = util::view<string_type::const_iterator>;

            iterator (range_type range, char separator);

            iterator  operator++ (int);
            iterator& operator++ (void);

            range_type operator* (void) const;

            bool operator== (const iterator&) const;

        private:
            const value_type  m_separator;
            range_type m_range;
            string_type::const_iterator m_end;
        };

        iterator cbegin (void) const;
        iterator begin  (void) const;

        iterator cend (void) const;
        iterator end  (void) const;

    private:
        const string_type &m_value;
        const value_type m_separator;
    };
}

#endif // __UTIL_STRING_HPP

