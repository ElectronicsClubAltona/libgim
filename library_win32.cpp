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

#include "library_win32.hpp"

#include "except.hpp"

using util::detail::win32::library;


///////////////////////////////////////////////////////////////////////////////
library::library (const char *path):
    m_handle (LoadLibraryA (path))
{
    if (!m_handle)
        win32_error::throw_code ();
}


//-----------------------------------------------------------------------------
library::library (const std::string &path):
    library (path.c_str ())
{ ; }


//-----------------------------------------------------------------------------
library::~library ()
{
    FreeLibrary (m_handle);
}


///////////////////////////////////////////////////////////////////////////////
void*
library::symbol (const char *name)
{
    return reinterpret_cast<void*> (GetProcAddress (m_handle, name));
}
