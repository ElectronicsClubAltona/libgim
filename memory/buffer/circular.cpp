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

#include "circular.hpp"

#include "../../debug.hpp"
#include "../../maths.hpp"
#include "../../posix/except.hpp"
#include "../../raii.hpp"
#include "../../random.hpp"
#include "../system.hpp"

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <algorithm>

using util::memory::buffer::circular;


///////////////////////////////////////////////////////////////////////////////
// generate a random string that could be used as a path leaf
//
// it looks a lot like a shitty tmpnam replacement because it is. we can't use
// tmpnam without security warnings being emitted by binutils linker, despite
// using it safely in this particular scenario.
static void
tmpname (std::string &str, size_t length)
{
    static const char alphanum[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";

    str.resize (length);
    std::generate_n (str.begin (), length, [&] (void) {
        return util::random::choose (alphanum);
    });
}


///////////////////////////////////////////////////////////////////////////////
template <typename ValueT>
circular<ValueT>::circular (size_t bytes)
{
    bytes = max (bytes, sizeof (value_type));
    bytes = round_up (bytes, pagesize ());

    int fd = -1;

    constexpr size_t RETRIES = 128;
    constexpr size_t NAME_LENGTH = 16;
    std::string name (NAME_LENGTH, '\0');

    // keep generating paths and attempting to create the shm backing. we may
    // fall through this loop upon failure, so be sure to check the validity
    // of the fd at the end.
    for (size_t i = 0; fd < 0 && i < RETRIES; ++i) {
        tmpname (name, NAME_LENGTH);
        name[0] = '/';

        fd = shm_open (name.c_str (), O_EXCL | O_CREAT | O_TRUNC | O_RDWR, 0600);
    }

    if (fd < 0)
        throw std::runtime_error ("unable to generate shm name");

    // setup a desctructor for the shm data. mmap retains a reference, so do
    // this whether we succeed or fail in the next phase.
    util::scoped_function raii ([&name] (void) { shm_unlink (name.c_str ()); });

    // embiggen to the desired size
    if (ftruncate (fd, bytes))
        posix::error::throw_code ();

    // pre-allocate a sufficiently large virtual memory block. it doesn't
    // matter much what flags we use because we'll just be overwriting it
    // shortly.
    m_begin = reinterpret_cast<ValueT*> (
        mmap (nullptr, bytes * 2, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)
    );
    if (MAP_FAILED == m_begin)
        posix::error::throw_code ();

    // preemptively setup an unmapping object in case the remapping fails
    util::scoped_function unmapper ([this, bytes] (void) { munmap (m_begin, bytes); });

    // overwrite the map with two adjacent copies of the memory object. this
    // must be a shared mapping for the values to propogate across segments.
    auto prot = PROT_READ | PROT_WRITE;
    auto flag = MAP_FIXED | MAP_SHARED;

    m_begin = reinterpret_cast<ValueT*> (mmap (m_begin,         bytes, prot, flag, fd, 0));
    m_end   = reinterpret_cast<ValueT*> (mmap (m_begin + bytes, bytes, prot, flag, fd, 0));

    if (m_begin == MAP_FAILED || m_end == MAP_FAILED)
        posix::error::throw_code ();

    // all went well, disarm the failsafe
    unmapper.clear ();
}


//-----------------------------------------------------------------------------
template <typename ValueT>
circular<ValueT>::~circular ()
{
    auto res = munmap (m_begin, 2 * (m_end - m_begin));
    (void)res;
    CHECK_ZERO (res);
}


///////////////////////////////////////////////////////////////////////////////
template <typename ValueT>
ValueT*
circular<ValueT>::begin (void)&
{
    return m_begin;
}


//-----------------------------------------------------------------------------
template <typename ValueT>
ValueT*
circular<ValueT>::end (void)&
{
    return m_end;
}


///////////////////////////////////////////////////////////////////////////////
template <typename ValueT>
size_t
circular<ValueT>::size (void) const
{
    return m_end - m_begin;
}


//-----------------------------------------------------------------------------
template <typename ValueT>
typename circular<ValueT>::iterator
circular<ValueT>::constrain (iterator cursor)
{
    CHECK_LIMIT (cursor, m_begin, m_begin + 2 * size ());

    return m_begin + (cursor - m_begin) % size ();
}


//-----------------------------------------------------------------------------
template <typename ValueT>
util::view<typename circular<ValueT>::iterator>
circular<ValueT>::constrain (util::view<iterator> window)
{
    CHECK_LIMIT (window.begin (), m_begin, m_begin + 2 * size ());
    CHECK_LIMIT (window.end   (), m_begin, m_begin + 2 * size ());

    auto first = window.begin ();
    auto last  = first + window.size ();
    util::view res { first, last };

    CHECK_EQ (res.size (), window.size ());
    CHECK_LE (res.begin (), res.end ());
    return res;
}


///////////////////////////////////////////////////////////////////////////////
template class util::memory::buffer::circular<char>;
template class util::memory::buffer::circular<uint8_t>;
