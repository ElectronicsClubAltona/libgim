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
 * Copyright 2015, 2017 Danny Robson <danny@nerdcruft.net>
 */


#ifndef CRUFT_UTIL_URI_HPP
#define CRUFT_UTIL_URI_HPP

#include "view.hpp"

#include <array>
#include <string>
#include <stdexcept>


namespace util {
    // parsing of rfc3986 uniform resource identifiers
    //
    // does not currently perform normalisation (scheme or protocol),
    // comparison, or other associated operations. though these should be
    // added in the future.
    //
    // note that the parsed results may not always conform to expectations
    // for some protocols. eg, mailto identifiers are complex to parse
    // reliably and would require a specialised parser to be reliable.
    //
    // not all fields will be present for all protocols (or all instances of
    // any given protocol). eg, the "tel" is unlikely to have port numbers.
    class uri {
    public:
        explicit uri (std::string &&);
        explicit uri (const char *str);
        uri (const char *first, const char *last);

        class parse_error : public std::runtime_error
        { using runtime_error::runtime_error; };


        // URI:     'https://user:password@example.com:80/path/to?foo=bar#fragment'
        //
        // SCHEME:          'https'
        // HIERARCHICAL:    'user:password@example.com:80/path/to'
        // AUTHORITY:       'user:password@example.com:80'
        // USER:            'user:password'
        // HOST:            'example.com'
        // PORT:            '80'
        // PATH:            '/path/to'
        // QUERY:           'foo=bar'
        // FRAGMENT:        'fragment'
        enum component {
            SCHEME,
            HIERARCHICAL,
                AUTHORITY,
                    USER,
                    HOST,
                    PORT,
                PATH,
            QUERY,
            FRAGMENT,

            NUM_COMPONENTS
        };

        view<const char*> get (component) const;

        view<const char*> scheme (void) const { return get (SCHEME); }
        view<const char*> heirarchical (void) const { return get (HIERARCHICAL); }
        view<const char*> authority (void) const { return get (AUTHORITY); }
        view<const char*> user (void) const { return get (USER); }
        view<const char*> host (void) const { return get (HOST); }
        view<const char*> port (void) const { return get (PORT); }
        view<const char*> path (void) const { return get (PATH); }
        view<const char*> query (void) const { return get (QUERY); }
        view<const char*> fragment (void) const { return get (FRAGMENT); }

        auto components (void) const noexcept { return m_views; }

        static std::string percent_decode (view<const char*>);

    private:
        std::array<view<const char*>, NUM_COMPONENTS> m_views;
        std::string m_value;
    };

    std::ostream& operator<< (std::ostream&, const uri&);
    std::ostream& operator<< (std::ostream&, uri::component);
}


#endif
