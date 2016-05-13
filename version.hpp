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
#include <limits>

namespace util {
    struct version {
        // underlying component value type
        using value_t = unsigned;

        static_assert (std::numeric_limits<value_t>::max () >= 2100'00'00,
                       "value_t must be sufficient to store a reasonable future year in YYYYMMDD format");

        /// type of release.
        ///
        /// * must be sorted from earliest to latest
        /// * specific values aren't currently guaranteed.
        enum release_t {
            ALPHA,
            BETA,
            GAMMA,
            PRODUCTION
        };

        version ();
        version (unsigned int _major, unsigned int _minor);
        explicit version (const std::string& str);
        explicit version (const char *str);

        void sanity (void) const;

        // named component indices
        //
        // * must start from zero and be monotonically increasing
        enum offset_t {
            MAJOR = 0,
            MINOR = 1,
            POINT = 2,
            BUILD = 3
        };

// fuck you too freebsd
#ifdef major
#undef major
#endif
#ifdef minor
#undef minor
#endif

        unsigned major (void) const;
        unsigned minor (void) const;
        unsigned point (void) const;
        unsigned build (void) const;

        const unsigned* begin (void) const;
        const unsigned* end   (void) const;

        size_t size;
        std::array<unsigned,4u> components;
        release_t release;

        static version parse (const std::string&);
        static version parse (const char*);

        bool operator<  (const version& rhs) const;
        bool operator>  (const version& rhs) const;
        bool operator>= (const version& rhs) const;
        bool operator<= (const version& rhs) const;
        bool operator== (const version& rhs) const;
    };

    std::ostream& operator<< (std::ostream& os, const util::version& rhs);
}


#endif // __VERSION_HPP
