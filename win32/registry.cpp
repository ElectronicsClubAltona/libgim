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

#include "registry.hpp"

#include "error.hpp"
#include "../debug.hpp"
#include "../except.hpp"

#include <string>
#include <cstdint>

using util::win32::key;


///////////////////////////////////////////////////////////////////////////////
typedef uint64_t QWORD;

//-----------------------------------------------------------------------------
template <typename T> constexpr DWORD type_to_id (void);

template <> constexpr DWORD type_to_id<DWORD> (void) { return REG_DWORD; }
template <> constexpr DWORD type_to_id<QWORD> (void) { return REG_QWORD; }
template <> constexpr DWORD type_to_id<void>  (void) { return REG_NONE;  }
template <> constexpr DWORD type_to_id<std::string> (void) { return REG_SZ; }


//-----------------------------------------------------------------------------
template <typename T> constexpr DWORD restrict_to_id (void);

template <> constexpr DWORD restrict_to_id<DWORD> (void) { return RRF_RT_DWORD; }
template <> constexpr DWORD restrict_to_id<QWORD> (void) { return RRF_RT_QWORD; }
template <> constexpr DWORD restrict_to_id<void>  (void) { return RRF_RT_REG_NONE; }
template <> constexpr DWORD restrict_to_id<std::string> (void) { return RRF_RT_REG_SZ; }


///////////////////////////////////////////////////////////////////////////////
key::key (HKEY root, const char *child, REGSAM rights)
{
    auto err = RegOpenKeyEx (root, child, 0, rights, &m_handle);
    win32::error::try_code (err);
}


//-----------------------------------------------------------------------------
key::~key ()
{
    auto err = RegCloseKey (m_handle);
    win32::error::try_code (err);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
key::data (const char *name) const
{
    T     value;
    DWORD type;
    DWORD size = sizeof (value);

    auto err = RegGetValue (m_handle, name, &value, restrict_to_id<T> (), nullptr, &value, &size);
    win32::error::try_code (err);

    return value;
}


//-----------------------------------------------------------------------------
std::set<std::string>
key::values (void) const
{
    std::set<std::string> all;

    for (DWORD i = 0; ; ++i) {
        std::string name (255, '\0');
        DWORD size = name.size ();

        auto err = RegEnumValue (m_handle, i, &name[0], &size, nullptr, nullptr, nullptr, nullptr);
        if (ERROR_NO_MORE_ITEMS == err)
            return all;
        if (ERROR_SUCCESS != err)
            win32::error::throw_code (err);

        CHECK_GT (size, 0u);
        name.resize (size - 1);

        all.insert (std::move (name));
    }
}
