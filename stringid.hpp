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
 * Copyright 2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_STRINGID_HPP
#define __UTIL_STRINGID_HPP

#include <map>
#include <string>

namespace util {
    class stringid {
        public:
            typedef unsigned id_t;

            id_t add  (std::string);
            id_t find (const std::string&) const;

            void clear (void);

        private:
            std::map<const std::string, id_t> m_map;
    };
}

#endif
