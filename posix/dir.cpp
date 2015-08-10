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

#include "dir.hpp"

#include "../except.hpp"

using util::posix::dir;


///////////////////////////////////////////////////////////////////////////////
//struct entry {
//public:
//    entry (dirent *_handle, DIR *_parent): m_handle (_handle), m_parent (_parent) { ; }
//
//    const char* name (void) const { return m_handle->d_name; }
//
//    entry& operator++ (void) {
//        dirent *next = readdir (m_parent);
//        assert (!next || next != m_handle);
//        m_handle = next;
//        return *this;
//    }
//
//    entry& operator* (void) { return *this; }
//    const entry& operator* (void) const;
//    bool operator!= (entry rhs) const { return m_handle != rhs.m_handle || m_parent != rhs.m_parent; }
//
//    struct dirent *m_handle;
//    DIR *m_parent;
//};


///////////////////////////////////////////////////////////////////////////////
dir::dir (const char *path):
    m_handle (opendir (path))
{
    if (!m_handle)
        errno_error::throw_code ();
}


//-----------------------------------------------------------------------------
dir::~dir ()
{
    errno_error::try_code (closedir (m_handle));
}


///////////////////////////////////////////////////////////////////////////////
dir::operator DIR* (void)
{
    return m_handle;
}


///////////////////////////////////////////////////////////////////////////////
void
dir::scan(std::function<void(const char*)> cb)
{
    rewind ();

    for (dirent *cursor; errno = 0, cursor = readdir (m_handle); )
        cb (cursor->d_name);

    errno_error::try_code ();
}


///////////////////////////////////////////////////////////////////////////////
void
dir::rewind (void)
{
    rewinddir (m_handle);
}
