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


#ifndef __UTIL_URI_HPP
#define __UTIL_URI_HPP

#include "view.hpp"

#include <string>
#include <stdexcept>


namespace util {
    class uri {
    public:
        explicit uri (std::string &&);
        explicit uri (const char *str);
        uri (const char *first, const char *last);

        class parse_error : public std::runtime_error
        { using runtime_error::runtime_error; };

        enum component : unsigned {
            SCHEME,
            AUTHORITY,
            PATH,
            QUERY,
            FRAGMENT,

            NUM_COMPONENTS
        };

        view<const char*> get (component);

        static std::string percent_decode (view<const char*>);

    private:
        view<const char*> m_views[NUM_COMPONENTS];
        std::string m_value;
    };

    std::ostream& operator<< (std::ostream&, uri::component);
}


#endif
