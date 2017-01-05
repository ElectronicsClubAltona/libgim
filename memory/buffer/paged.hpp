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

#ifndef __UTIL_MEMORY_BUFFER_PAGED_HPP
#define __UTIL_MEMORY_BUFFER_PAGED_HPP

#include <cstddef>

namespace util::memory::buffer {
    class paged {
    public:
        using value_type = char;

        paged (size_t bytes, size_t window);
        ~paged ();

        paged (const paged&) = delete;
        paged (paged &&) = delete;
        paged& operator= (const paged&) = delete;
        paged& operator= (paged &&) = delete;

        char* begin (void)&;
        char* end   (void)&;

        const char* cbegin (void)&;
        const char* cend   (void)&;

        const char* begin (void) const&;
        const char* end   (void) const&;

        void access (char*);

        size_t size (void) const;
        size_t capacity (void) const;
        size_t window (void) const;

    private:
        void commit  (char*);
        void release (char*);

        char *m_begin, *m_end, *m_cursor;
        size_t m_window;
    };
}

#endif
