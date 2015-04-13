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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_VERSION_HPP
#define __UTIL_VERSION_HPP

#include <array>
#include <string>
#include <iostream>


namespace util {
    struct version {
        enum release_t {
            RELEASE_ALPHA,
            RELEASE_BETA,
            RELEASE_GAMMA,
            RELEASE_PRODUCTION
        };

        version ();
        version (unsigned int _major, unsigned int _minor);
        version (const std::string& str);
        version (const char *str);

        void sanity (void) const;

        enum {
            OFFSET_MAJOR = 0,
            OFFSET_MINOR = 1,
            OFFSET_POINT = 2,
            OFFSET_BUILD = 3,

            NUM_OFFSETS
        };

        unsigned major (void) const { return values[OFFSET_MAJOR]; }
        unsigned minor (void) const { return values[OFFSET_MINOR]; }
        unsigned point (void) const { return values[OFFSET_POINT]; }
        unsigned build (void) const { return values[OFFSET_BUILD]; }

        std::array<unsigned int, NUM_OFFSETS> values;
        size_t size;
        release_t release;

        static version parse (const std::string&);
        static version parse (const char*);

        bool operator <  (const version& rhs) const;
        bool operator >  (const version& rhs) const;
        bool operator >= (const version& rhs) const;
        bool operator <= (const version& rhs) const;
        bool operator == (const version& rhs) const;
    };
}

std::ostream& operator<< (std::ostream& os, const util::version& rhs);


#endif // __VERSION_HPP
