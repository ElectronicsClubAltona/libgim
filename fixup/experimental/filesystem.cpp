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

#include "filesystem.hpp"

#include "../../except.hpp"

#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace ns = std::experimental::filesystem;


///////////////////////////////////////////////////////////////////////////////
ns::path::path ()
{ ; }


//-----------------------------------------------------------------------------
ns::path::path (const path &p):
    m_path (p.m_path)
{ ; }


///////////////////////////////////////////////////////////////////////////////
std::string
ns::path::string (void) const
{
    return m_path;
}


///////////////////////////////////////////////////////////////////////////////
const ns::path::string_type&
ns::path::native (void) const
{
    return m_path;
}


//-----------------------------------------------------------------------------
const ns::path::value_type*
ns::path::c_str (void) const
{
    return m_path.c_str ();
}


///////////////////////////////////////////////////////////////////////////////
ns::path
ns::path::filename (void) const
{
    auto slash = m_path.find_last_of (preferred_separator);
    if (slash == decltype(m_path)::npos)
        return m_path;
    return ns::path (m_path.cbegin () + slash, m_path.cend ());
}

//-----------------------------------------------------------------------------
ns::path
ns::path::stem (void) const
{
    auto name = filename ();

    auto first = name.m_path.cbegin ();
    auto last  = std::find_if (first, name.m_path.cend (), [] (auto c) { return c == '.'; });
    return path (first, last);
}


///////////////////////////////////////////////////////////////////////////////
ns::path
ns::operator/ (const ns::path &a, const ns::path &b)
{
    return ns::path (a) /= b;
}


//-----------------------------------------------------------------------------
ns::path&
ns::path::operator/= (const path &rhs)
{
    m_path += preferred_separator + rhs.m_path;
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
bool
ns::operator== (const path &a, const path &b)
{
    return a == b;
}


///////////////////////////////////////////////////////////////////////////////
bool
ns::is_directory (const path &p)
{
    struct stat buf;

    if (stat (p.c_str (), &buf))
        ::util::errno_error::throw_code ();

    return S_ISDIR (buf.st_mode);
}
