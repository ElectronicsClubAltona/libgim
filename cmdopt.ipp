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
 * Copyright 2013 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_CMDLINE_IPP
#error
#endif
#define __UTIL_CMDLINE_IPP

#include <sstream>

#include "./introspection.hpp"
#include "./iterator.hpp"

namespace util { namespace cmdopt {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    option::value<T>::value (std::string _name, std::string _description, T &_data):
        base (std::move (_name), std::move (_description)),
        m_data (_data)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T>
    void
    option::value<T>::execute (const char *restrict str)
    {
        try {
            std::istringstream os (str);
            os.exceptions (
                  std::istringstream::failbit
                | std::istringstream::badbit
            );
            os >> m_data;

            if (!os.eof ())
                throw invalid_value (__func__);
        } catch (...) {
            throw invalid_value (__func__);
        }

        seen (true);
    }


    //-------------------------------------------------------------------------
    namespace detail {
        template <typename T>
        std::enable_if_t<!std::is_enum<T>::value, const std::string&>
        value_example (void)
        {
            static const std::string EXAMPLE =
                std::string {"<"} +
                std::string {to_string<T> ()} +
                std::string {">"};

            return EXAMPLE;
        }

        template <typename T>
        std::enable_if_t<std::is_enum<T>::value, const std::string&>
        value_example (void)
        {
            static const std::string EXAMPLE = [] (void) {
                std::ostringstream os;
                std::copy (std::cbegin (enum_traits<T>::names),
                           std::cend   (enum_traits<T>::names),
                           infix_iterator<const char*> (os, "|"));
                return os.str ();
            } ();
            return EXAMPLE;
        }
    }


    //-------------------------------------------------------------------------
    template <typename T>
    const std::string&
    option::value<T>::example (void) const
    {
        return detail::value_example<T> ();
    }


    //-----------------------------------------------------------------------------
    template <typename T>
    T
    option::value<T>::data (void) const
    {
        return m_data;
    }


    //-----------------------------------------------------------------------------
    template <typename T>
    T&
    option::value<T>::data (void)
    {
        return m_data;
    }


    //-----------------------------------------------------------------------------
    template <typename T>
    T&
    option::value<T>::data (T _data)
    {
        return m_data = _data;
    }


    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename ...Args>
    T&
    parser::add (char shortname,
                 std::string longname,
                 std::string description,
                 Args&&... args)
    {
        auto handler = std::make_unique<T> (longname, description, std::forward<Args> (args)...);
        T& ref = *handler;

        m_short.emplace_back (shortname, ref);
        m_long .emplace_back (longname,  ref);

        m_options.emplace_back (shortname, longname, std::move (handler));

        return ref;
    }
} }
