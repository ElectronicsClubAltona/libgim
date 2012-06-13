#include "backtrace.hpp"

#include "debug.hpp"
#include "types/casts.hpp"

#include <cstdlib>
#include <execinfo.h>
#include <algorithm>
#include <memory>

using namespace std;


debug::backtrace::backtrace (void):
    m_frames (DEFAULT_DEPTH) {

    int final;
    while ((final = ::backtrace (&m_frames[0], m_frames.size ())) == sign_cast<ssize_t> (m_frames.size ()))
        m_frames.resize (m_frames.size () * 2);

    CHECK_HARD (final > 0);
    m_frames.resize ((unsigned)final);
}


ostream&
debug::operator <<(ostream &os, const debug::backtrace &rhs) {
    const auto frames = rhs.frames ();

    // We don't use the array form of unique_ptr as clang fails on ambigious constructors
    typedef unique_ptr<char *, decltype(&std::free)> unique_str;
    unique_str names (backtrace_symbols (frames.data (), frames.size ()), ::free);

    for (unsigned int i = 0; i < frames.size (); ++i)
        os << frames[i] << "\t" << names.get()[i] << "\n";

    return os;
}
