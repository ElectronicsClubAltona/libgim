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
 * Copyright:
 *      2012-2016, Danny Robson <danny@nerdcruft.net>
 */

#include "./backtrace.hpp"

#include "./win32/handle.hpp"
#include "./debug.hpp"
#include "./except.hpp"

#include <cstdlib>
#include <windows.h>
#include <dbghelp.h>


///////////////////////////////////////////////////////////////////////////////
debug::backtrace::backtrace (void)
{
    m_frames.resize (DEFAULT_DEPTH);

    auto process = GetCurrentProcess();
    if (!SymInitialize (process, NULL, TRUE))
        util::win32_error::throw_code ();

    while (true) {
        auto res = CaptureStackBackTrace (1, m_frames.size (), m_frames.data (), NULL);
        if (res != m_frames.size ()) {
            m_frames.resize (res);
            break;
        }

        m_frames.resize (m_frames.size () * 2);
    }
}


///////////////////////////////////////////////////////////////////////////////
std::ostream&
debug::operator <<(std::ostream &os, const debug::backtrace &rhs) {
    static auto self  = GetCurrentProcess ();
    static auto ready = SymInitialize (self, nullptr, TRUE);
    CHECK (ready);

    static constexpr size_t MAX_LENGTH = 255;
    struct {
        SYMBOL_INFO info;
        char name[MAX_LENGTH + 1];
    } symbol;

    symbol.info.MaxNameLen   = MAX_LENGTH;
    symbol.info.SizeOfStruct = sizeof (SYMBOL_INFO);

    for (void *frame: rhs.frames ()) {
        symbol.name[0] = '\0';
        SymFromAddr (self, (DWORD64)frame, 0, &symbol.info);
        symbol.name[MAX_LENGTH] = '\0';

        os << self << "\t" << frame << "\t" << symbol.name << "\n";
    }

    return os;
}
