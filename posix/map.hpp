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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __CRUFT_UTIL_POSIX_HPP
#define __CRUFT_UTIL_POSIX_HPP

#include "./fd.hpp"

#include <sys/mman.h>
#include <cstdint>


namespace util::posix {
    class map {
    public:
        map (const map&) = delete;
        map (map&&);

        map (size_t size, int prot, int flags);
        map (size_t size, int prot, int flags, const fd&, off_t offset = 0);
        ~map ();

        map& operator= (const map&) = delete;
        map& operator= (map&&);

        uint8_t* begin (void);
        uint8_t* end   (void);
        const uint8_t* begin  (void) const;
        const uint8_t* end    (void) const;
        const uint8_t* cbegin (void) const;
        const uint8_t* cend   (void) const;

        void* data (void);
        const void* data (void) const;

        void sync (void *addr, size_t len, int flags);

        bool empty (void) const;
        size_t size (void) const;

        enum class resize_t { MOVE, NOMOVE };
        void resize (size_t newsize, resize_t = resize_t::NOMOVE);

    private:
        void    *m_addr;
        size_t   m_size;
    };
}

#endif
