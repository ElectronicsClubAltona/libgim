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

#include "./term.hpp"

#include "./string.hpp"

#include <boost/filesystem.hpp>
#include <cstdlib>
#include <stdexcept>

using util::term::csi::graphics;


static const boost::filesystem::path DEFAULT_SEARCH_DIR = "/usr/share/terminfo";

constexpr char util::term::csi::code::CSI;

const graphics graphics::RESET (0);


///////////////////////////////////////////////////////////////////////////////
// find the terminfo path within a directory, for a key
//
// throws an exception if not found
static
boost::filesystem::path
find_terminfo_path (const boost::filesystem::path &dir,
                    const std::string &key)
{
    const char letter[2] = { key[0], '\0' };
    auto candidate = dir / letter / key;

    if (!boost::filesystem::is_directory (candidate))
        throw std::runtime_error ("path not found");

    return candidate;
}


//-----------------------------------------------------------------------------
// find the terminfo path for a given key.
//
// throws an exception if not found
static
boost::filesystem::path
find_terminfo_path (const std::string &key)
{
    // check if the path is explicitly listed. must not fall through.
    if (const char *dir = getenv ("TERMINFO")) {
        return find_terminfo_path (dir, key);
    }

    // check if we have a path at $HOME. falls through.
    if (const char *home = getenv ("HOME")) {
        boost::filesystem::path HOME (home);
        try {
            return find_terminfo_path (HOME / ".terminfo", key);
        } catch (...) { }
    }

    // check colon seperated list in TERMINFO_DIRS
    if (const char *dirs = getenv ("TERMINFO_DIRS")) {
        auto tok = util::make_tokeniser (dirs, ':');

        for (auto i: tok) {
            try {
                return find_terminfo_path (
                    i.empty () ?
                    DEFAULT_SEARCH_DIR :
                    boost::filesystem::path (i.cbegin (), i.cend ()), key
                );
            } catch (...) { }
        }
    }

    return find_terminfo_path ("/usr/share/terminfo", key);
}


///////////////////////////////////////////////////////////////////////////////
bool
util::term::has_csi_support (void)
{
    // HACK: this will do until we get a terminfo/termcap reader
    return getenv ("TERM") == std::string ("XTERM");
}


///////////////////////////////////////////////////////////////////////////////
graphics::graphics (layer _layer, hue _hue):
    m_value (_layer + _hue)
{ ; }


//-----------------------------------------------------------------------------
graphics::graphics (char _value):
    m_value (_value)
{ ; }


///////////////////////////////////////////////////////////////////////////////
char
graphics::value (void) const
{
    return m_value;
}


//-----------------------------------------------------------------------------
char
graphics::value (char _value)
{
    return m_value = _value;
}


///////////////////////////////////////////////////////////////////////////////
std::ostream&
util::term::csi::operator<< (std::ostream &os, graphics g)
{
    return os << code::CSI << '[' << +g.value () << decltype(g)::terminator;
}
