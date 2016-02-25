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
 * Copyright 2015-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "../except.hpp"

///////////////////////////////////////////////////////////////////////////////
template <typename ...Args>
void
util::posix::dir::scan(std::function<void(const char*, Args&...)> cb, Args &...args)
{
    rewind ();

    for (dirent *cursor; errno = 0, cursor = readdir (m_handle); )
        cb (cursor->d_name, args...);

    errno_error::try_code ();
}


//-----------------------------------------------------------------------------
template <typename ...Args>
void
util::posix::dir::scan (void (*cb) (const char*, Args&...), Args &...args)
{
    rewind ();

    for (dirent *cursor; errno = 0, cursor = readdir (m_handle); )
        cb (cursor->d_name, args...);

    errno_error::try_code ();
}
