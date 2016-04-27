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

#include "./backtrace.hpp"

#include "./debug.hpp"
#include "./except.hpp"
#include "./types.hpp"

#include <windows.h>
#include <dbghelp.h>

#include <iostream>

using debug::backtrace;


///////////////////////////////////////////////////////////////////////////////
backtrace::backtrace ()
{
    static auto ready = SymInitialize (GetCurrentProcess (), nullptr, TRUE);
    CHECK (ready);

    CONTEXT ctx;
    memset (&ctx, 0, sizeof (ctx));
    ctx.ContextFlags = CONTEXT_ALL;
    RtlCaptureContext (&ctx);

    auto image_type = IMAGE_FILE_MACHINE_AMD64;

    STACKFRAME64 frame;
    memset (&frame, 0, sizeof (frame));
    frame.AddrPC.Offset = ctx.Rip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = ctx.Rsp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = ctx.Rsp;
    frame.AddrStack.Mode = AddrModeFlat;

    auto process = GetCurrentProcess ();
    auto thread  = GetCurrentThread ();

    SetLastError (0);

    while (true) {
        if (!StackWalk64 (image_type,
                          process,
                          thread,
                          &frame,
                          &ctx,
                          nullptr,
                          SymFunctionTableAccess64,
                          SymGetModuleBase64,
                          nullptr))
        {
            util::win32_error::throw_code ();
        }

        // we've read the bottom of the stack
        if (frame.AddrReturn.Offset == 0)
            break;

        // we've reached the tail of an (apparently) infinite stack. this is a common occurence.
        if (frame.AddrPC.Offset == frame.AddrReturn.Offset)
            break;

        m_frames.push_back ((void*)frame.AddrPC.Offset);
    }
}


///////////////////////////////////////////////////////////////////////////////
std::ostream&
debug::operator<< (std::ostream &os, const debug::backtrace &b)
{
    const auto self = GetCurrentProcess ();

    struct [[gnu::packed]] {
        IMAGEHLP_SYMBOL64 info;
        char name[255];
    } sym;

    sym.info.SizeOfStruct = sizeof (sym.info);
    sym.info.MaxNameLength = elems (sym.name);

    for (auto frame: b.frames ()) {
        // Find the symbol name
        sym.info.Name[0] = '\0';
        memset (sym.name, 0, sizeof (sym.name));
        SymGetSymFromAddr64 (self, (uintptr_t)frame, nullptr, &sym.info);
        *std::rbegin (sym.name) = '\0';

        os << self << '\t' << frame << '\t' << sym.info.Name << '\n';
    }

    return os;
}
