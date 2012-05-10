#include "backtrace.hpp"

#include "debug.hpp"
#include "except.hpp"
#include "memory.hpp"

#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <dbghelp.h>


debug::backtrace::backtrace (void) {
    m_frames.resize (DEFAULT_DEPTH);

    auto process = GetCurrentProcess();
    if (!SymInitialize (process, NULL, TRUE))
        win32_error::throw_code ();

    while (CaptureStackBackTrace (1, m_frames.size (), m_frames.data (), NULL) == m_frames.size ())
        m_frames.resize (m_frames.size () * 2);
}


std::ostream&
debug::operator <<(std::ostream &os, const debug::backtrace &rhs) {
    os << "Fuck Windows and it's stupid backtracing\n";
    return os;

    auto process = GetCurrentProcess ();
    if (!process)
        win32_error::throw_code ();

    static const size_t MAX_LENGTH = 255;
    scoped_malloc<void> symbol_mem (calloc (sizeof (SYMBOL_INFO) + MAX_LENGTH + 1, 1));

    SYMBOL_INFO *symbol  = static_cast<SYMBOL_INFO*> (symbol_mem.get ());
    symbol->MaxNameLen   = MAX_LENGTH;
    symbol->SizeOfStruct = sizeof (SYMBOL_INFO);

    for (void *frame: rhs.frames ()) {
        std::cerr << process << " " << frame << " " << symbol << "\n";
        SymFromAddr (process, (DWORD64)frame, 0, symbol);
        std::cerr << frame << "\t" << symbol->Name << "\n";
    }

    return os;
}
