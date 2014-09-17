/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */


#include "backtrace.hpp"

#include "debug.hpp"
#include "types/casts.hpp"

#include <cstdlib>
#include <execinfo.h>
#include <algorithm>
#include <memory>


using namespace std;


//-----------------------------------------------------------------------------
debug::backtrace::backtrace (void):
    m_frames (DEFAULT_DEPTH) {

    size_t last;
    size_t size = m_frames.size ();

    while ((last = ::backtrace (&m_frames[0], m_frames.size ())) == size)
        m_frames.resize (size = m_frames.size () * 2);

    CHECK_HARD (last > 0);
    m_frames.resize (last);
}


//-----------------------------------------------------------------------------
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
