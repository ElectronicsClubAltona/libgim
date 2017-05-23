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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_FIXUP_EXPERIMENTAL_FILESYSTEM_HPP
#define CRUFT_UTIL_FIXUP_EXPERIMENTAL_FILESYSTEM_HPP

#include <string>

///////////////////////////////////////////////////////////////////////////////
namespace std::experimental::filesystem {
    class path {
    public:
        using value_type = char;
        using string_type = std::basic_string<value_type>;
        
        static constexpr value_type preferred_separator = '/';

        path ();
        explicit path (const path&);

        template <class Source>
        path (const Source &s):
            m_path (s)
        { ; }

        template <class InputT>
        path (InputT first, InputT last):
            m_path (first, last)
        { ; }

        std::string string (void) const;

        const string_type& native (void) const;
        const value_type* c_str (void) const;

        path filename (void) const;
        path stem (void) const;

        path& operator/= (const path&);

    private:
        string_type m_path;
    };

    path operator/ (const path&, const path&);

    bool operator== (const path&, const path&);

    //bool is_directory (file_status);
    bool is_directory (const path&);
    //bool is_directory (const path&, error_code&);

    template <class CharT, class Traits>
    std::basic_ostream<CharT,Traits>&
    operator<< (std::basic_ostream<CharT,Traits> &os, const path &p)
    { return os << p.native (); }
}


#endif

