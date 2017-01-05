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

#ifndef __UTIL_WIN32_REGISTRY_HPP
#define __UTIL_WIN32_REGISTRY_HPP

#include <windows.h>

#include <set>
#include <string>

namespace util::win32 {
    class key {
    public:
        key (HKEY root, const char *child, REGSAM rights = KEY_READ);
        ~key ();

        template <typename T>
        T data (const char *name = nullptr) const;

        std::set<std::string>
        values (void) const;

    private:
        HKEY m_handle;
    };
}

#endif
