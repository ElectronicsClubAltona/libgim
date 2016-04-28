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
 * Copyright 2015-2016 Danny Robson <danny@nerdcruft.net>
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
#include <boost/filesystem/path.hpp>

namespace util {
    namespace detail { namespace format {
        ///////////////////////////////////////////////////////////////////////
        template <typename T>
        inline bool
        is_type_specifier (const char*)
        { return false; }


        //---------------------------------------------------------------------
        template <>
        inline bool
        is_type_specifier<char> (const char *s)
        { return *s == 'c'; }


        //---------------------------------------------------------------------
        template <>
        inline bool
        is_type_specifier<const char*> (const char *s)
        { return *s == 's'; }


        //---------------------------------------------------------------------
        template <>
        inline bool
        is_type_specifier<char*> (const char *s)
        { return *s == 's'; }


        //---------------------------------------------------------------------
        template <>
        inline bool
        is_type_specifier<std::string> (const char *s)
        { return *s == 's'; }


        //---------------------------------------------------------------------
        template <>
        inline bool
        is_type_specifier<boost::filesystem::path> (const char *s)
        { return *s == 's'; }


        //---------------------------------------------------------------------
        template <>
        inline bool
        is_type_specifier<size_t> (const char *s)
        { return *s == 'u' || *s == 'x'; }


        //---------------------------------------------------------------------
        template <>
        inline bool
        is_type_specifier<unsigned> (const char *s)
        { return *s == 'u' || *s == 'x'; }


        //---------------------------------------------------------------------
        template <>
        inline bool
        is_type_specifier<int> (const char *s)
        {
            return *s == 'i' || *s == 'd' || *s == 'x';
        }


        //---------------------------------------------------------------------
        template <>
        inline bool
        is_type_specifier<float> (const char *s)
        {
            switch (*s) {
            case 'e':
            case 'E':
            case 'f':
            case 'F':
            case 'g':
            case 'G':
            case 'a':
            case 'A':
                return true;

            default:
                return false;
            }
        }


        ///////////////////////////////////////////////////////////////////////
        template <typename T>
        inline bool
        is_valid_specifier (const char *s)
        {
            return *s == '!' || is_type_specifier<T> (s);
        }


        ///////////////////////////////////////////////////////////////////////
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


        //---------------------------------------------------------------------
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

            using namespace std::literals;

            static const char DELIMITER = '%';
            auto cursor = std::find (first, last, DELIMITER);
            std::copy (first, cursor, std::ostream_iterator<char> (dest));

            if (cursor == last)
                return;

            auto spec = cursor + 1;
            if (spec == last)
                throw util::format::format_error ("missing format specifier"s);

            if (!is_valid_specifier<typename std::decay<ValueT>::type> (&*spec))
                throw util::format::invalid_specifier<ValueT> (*spec);

            if (*spec == 'x') {
                dest << std::hex << val << std::dec;
            } else
                dest << val;

            render (spec + 1, last, dest, std::forward<Args> (args)...);
        }
    } }

    ///////////////////////////////////////////////////////////////////////////
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


    ///////////////////////////////////////////////////////////////////////////
    // TODO: we'd like to use typeid here for type naming, but we don't allow
    // RTTI. revisit this when introspection is more advanced.
    template <typename ValueT>
    format::invalid_specifier<ValueT>::invalid_specifier (char _specifier):
        format_error (
            format::render ("invalid specifier '%c' for type '%s'",
                            _specifier,
                            "unimplemented")
        ),
        m_specifier (_specifier)
    { ; }


    //-------------------------------------------------------------------------
    template <typename ValueT>
    char
    format::invalid_specifier<ValueT>::specifier (void) const
    { return m_specifier; }
}
