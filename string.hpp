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

#include "view.hpp"


namespace util {
    std::string to_utf8 (const wchar_t*);
    std::string to_utf8 (const std::wstring&);
}


bool
strbegins(const char *restrict str,
          const char *restrict prefix);


namespace util {
    template <typename Iterator>
    struct tokeniser {
    public:
        using value_type = typename std::iterator_traits<Iterator>::value_type;
        using range_type = view<Iterator>;

        tokeniser (Iterator first, Iterator last, value_type separator);

        struct iterator {
        public:
            iterator  operator++ (int);
            iterator& operator++ (void)&;

            range_type operator* (void) const;

            bool operator== (const iterator&) const;
            bool operator!= (const iterator&) const;

        private:
            iterator (range_type range, value_type separator);

            const value_type m_separator;
            range_type m_range;
            Iterator m_end;

            friend tokeniser;
        };

        iterator cbegin (void) const;
        iterator begin  (void) const;

        iterator cend (void) const;
        iterator end  (void) const;

    private:
        const range_type m_range;
        const value_type m_separator;
    };

    
    tokeniser<std::string::const_iterator> make_tokeniser (const std::string&, std::string::value_type);
    tokeniser<std::string::const_iterator> make_tokeniser (std::string&&, std::string::value_type) = delete;
    tokeniser<const char*> make_tokeniser (const char*, char);

}

#endif // __UTIL_STRING_HPP

