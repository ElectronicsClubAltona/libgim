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

#ifndef __UTIL_MEMORY_BUFFER_CIRCULAR_HPP
#define __UTIL_MEMORY_BUFFER_CIRCULAR_HPP

#include <cstddef>

namespace util { namespace memory { namespace buffer {
    // buffer size is advisory and will likely depend on page size. the user
    // must check the size after creation if this field is important for
    // their usage.
    class circular {
    public:
        using value_type = char;

        circular (size_t bytes);
        ~circular ();

        circular (const circular&) = delete;
        circular (circular&&) = delete;
        circular& operator= (const circular&) = delete;
        circular& operator= (circular&&) = delete;

        char& operator[] (size_t);
        const char& operator[] (size_t) const;

        char* begin (void);
        char* end   (void);

        const char* begin (void) const;
        const char* end   (void) const;

        size_t size (void) const;

    private:
        char *m_begin, *m_end;

    };
} } }

#endif
