#include "backtrace.hpp"

#include "debug.hpp"

#include <cstdlib>
#include <execinfo.h>
#include <algorithm>
#include <memory>

using namespace std;


debug::backtrace::backtrace (void):
    m_frames (DEFAULT_DEPTH) {

    int final;
    while ((final = ::backtrace (&m_frames[0], m_frames.size ())) == m_frames.size ())
        m_frames.resize (m_frames.size () * 2);

    check_hard (final > 0);
    m_frames.resize ((unsigned)final);
}


ostream&
debug::operator <<(ostream &os, const debug::backtrace &rhs) {
    const auto frames = rhs.frames ();

    typedef unique_ptr<char *[], decltype(&std::free)> unique_str;
    unique_str names (backtrace_symbols (&frames[0], frames.size ()), std::free);

    for (unsigned int i = 0; i < frames.size (); ++i)
        os << frames[i] << "\t" << names[i] << "\n";

    return os;
}
