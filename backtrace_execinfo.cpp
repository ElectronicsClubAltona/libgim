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
 * Copyright 2010-2014 Danny Robson <danny@nerdcruft.net>
 */


#include "backtrace.hpp"

#include "./debug.hpp"
#include "./exe.hpp"
#include "./io.hpp"
#include "cast.hpp"

#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <execinfo.h>
#include <algorithm>
#include <memory>


///////////////////////////////////////////////////////////////////////////////
debug::backtrace::backtrace (void):
    m_frames (DEFAULT_DEPTH) {

    size_t last;
    size_t size = m_frames.size ();

    while ((last = ::backtrace (&m_frames[0], m_frames.size ())) == size)
        m_frames.resize (size = m_frames.size () * 2);

    CHECK_GT (last, 0);
    m_frames.resize (last);
}


///////////////////////////////////////////////////////////////////////////////
static std::string
addr2line (const void *addr)
{
#if defined(ADDR2LINE)
    using pstream_t = std::unique_ptr<FILE,decltype(&::pclose)>;

    std::ostringstream cmd;
    cmd << ADDR2LINE << " -e " << util::image_path () << ' ' << std::hex << addr;

    pstream_t stream (
        ::popen (cmd.str ().c_str (), "r"),
        ::pclose
    );

    // inefficient to copy from vector to string, but it's not a high priority path
    auto data = util::slurp (stream.get ());
    return std::string (data.cbegin (), data.cend ());

#else
    return "\n";
#endif
}


///////////////////////////////////////////////////////////////////////////////
std::ostream&
debug::operator <<(std::ostream &os, const debug::backtrace &rhs) {
    const auto frames = rhs.frames ();

    // We don't use the array form of unique_ptr as clang fails on ambigious constructors
    typedef std::unique_ptr<char *, decltype(&std::free)> str_t;
    str_t names (backtrace_symbols (frames.data (), frames.size ()), ::free);

    for (unsigned int i = 0; i < frames.size (); ++i)
        os << frames[i] << '\t' << names.get()[i] << '\t' << addr2line (frames[i]);

    return os;
}
