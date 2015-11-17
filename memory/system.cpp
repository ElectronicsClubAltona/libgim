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

#include "./system.hpp"

#include "../except.hpp"
#include "../cast.hpp"

#include <unistd.h>

///////////////////////////////////////////////////////////////////////////////
size_t
util::memory::pagesize (void)
{
    static size_t val;

    if (!val) {
        auto res = sysconf (_SC_PAGE_SIZE);
        if (res == -1)
            errno_error::throw_code ();

        val = sign_cast<unsigned long> (res);
    }

    return val;
}
