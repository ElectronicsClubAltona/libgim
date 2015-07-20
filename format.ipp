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

#if defined(__UTIL_FORMAT_IPP)
#error
#endif
#define __UTIL_FORMAT_IPP

#include "debug.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <iterator>

namespace util {
    namespace detail { namespace format {
        template <typename InputIt>
        void
        render (InputIt first,
                InputIt last,
                std::ostringstream &dest)
        {
            static const char DELIMITER = '%';
            if (std::find (first, last, DELIMITER) != last)
                throw util::format::missing_error ("format specifier without value");

            std::copy (first, last, std::ostream_iterator<char> (dest));
        }


        template <typename InputIt,
                  typename ValueT,
                  typename ...Args>
        void
        render (InputIt first,
                InputIt last,
                std::ostringstream &dest,
                const ValueT& val,
                Args&& ...args)
        {
            CHECK (first <= last);

            static const char DELIMITER = '%';
            auto cursor = std::find (first, last, DELIMITER);
            std::copy (first, cursor, std::ostream_iterator<char> (dest));

            if (cursor == last)
                return;

            auto spec = cursor + 1;
            if (spec == last)
                throw util::format::format_error ("missing format specifier");

            if (*spec != 's')
                throw util::format::format_error ("unhandled format specifier");

            dest << val;

            render (spec + 1, last, dest, std::forward<Args> (args)...);
        }
    } }

    namespace format {
        template <typename ...Args>
        std::string
        render (const std::string &fmt, Args&&... args)
        {
            std::ostringstream out;

            util::detail::format::render (
                fmt.begin (),
                fmt.end (),
                out,
                std::forward<Args> (args)...
            );

            return out.str ();
        }
    }
}
