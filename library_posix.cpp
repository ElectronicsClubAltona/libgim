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

#include "library_posix.hpp"

#include <dlfcn.h>
#include <stdexcept>


using util::detail::library_posix;


///////////////////////////////////////////////////////////////////////////////
library_posix::library_posix (const std::experimental::filesystem::path &path):
    m_handle (dlopen (path.c_str (), RTLD_NOW))
{
    if (!m_handle)
        throw std::runtime_error (dlerror ());
}


//-----------------------------------------------------------------------------
library_posix::library_posix (library_posix &&rhs):
    m_handle (nullptr)
{
    std::swap (m_handle, rhs.m_handle);
}


//-----------------------------------------------------------------------------
library_posix::~library_posix ()
{
    if (m_handle)
        dlclose (m_handle);
}


///////////////////////////////////////////////////////////////////////////////
void*
library_posix::symbol (const char *name)
{
    return dlsym (m_handle, name);
}
