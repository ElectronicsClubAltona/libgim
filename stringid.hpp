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
 * Copyright 2014-2018 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_STRINGID_HPP
#define CRUFT_UTIL_STRINGID_HPP

#include "view.hpp"

#include <map>
#include <string>

namespace util {
    class stringid {
    public:
        typedef size_t id_t;


        ///////////////////////////////////////////////////////////////////////
        id_t add  (std::string);


        //---------------------------------------------------------------------
        template <typename T>
        id_t add (util::view<T> key)
        {
            return add (
                std::string{
                    std::cbegin (key),
                    std::cend (key)
                }
            );
        }


        ///////////////////////////////////////////////////////////////////////
        id_t find (const std::string&) const;


        //---------------------------------------------------------------------
        template <typename T>
        id_t find (util::view<T> key) const
        {
            return find (
                std::string {
                    std::cbegin (key),
                    std::cend (key)
                }
            );
        }


        ///////////////////////////////////////////////////////////////////////
        void clear (void);


    private:
        std::map<const std::string, id_t> m_map;
    };
}

#endif
