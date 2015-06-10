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

namespace util { namespace cmdopt {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    option::value<T>::value (std::string _name, T &_data):
        base (std::move (_name)),
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


    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename ...Args>
    T&
    parser::add (char shortname, std::string longname, std::string description, Args&&... args)
    {
        auto handler = std::make_unique<T> (longname, std::forward<Args> (args)...);
        T& ref = *handler;

        m_short.emplace_back (shortname, ref);
        m_long .emplace_back (longname,  ref);

        m_options.emplace_back (std::move (handler));

        return ref;
    }
} }
