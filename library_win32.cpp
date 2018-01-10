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

#include "win32/except.hpp"

using util::detail::win32::library;


///////////////////////////////////////////////////////////////////////////////
library::library (const std::experimental::filesystem::path &path):
    m_handle (LoadLibraryA (path.c_str ()))
{
    if (!m_handle)
        win32::error::throw_code ();
}


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
