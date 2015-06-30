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
 * Copyright 2013 Danny Robson <danny@nerdcruft.net>
 */

#include "cmdopt.hpp"

#include "debug.hpp"

#include <algorithm>
#include <cstring>

using util::cmdopt::option::base;
using util::cmdopt::option::bytes;
using util::cmdopt::option::count;
using util::cmdopt::option::null;
using util::cmdopt::option::present;
using util::cmdopt::option::value;
using util::cmdopt::parser;


///////////////////////////////////////////////////////////////////////////////
base::base (std::string _name):
    m_name (_name),
    m_required (false),
    m_seen (false)
{ ; }


//-----------------------------------------------------------------------------
base::~base ()
{ ; }


//-----------------------------------------------------------------------------
void
base::execute (void)
{
    throw invalid_null (m_name);
}


//-----------------------------------------------------------------------------
void
base::execute (const char *restrict)
{
    throw invalid_value (m_name);
}


//-----------------------------------------------------------------------------
void
base::start (void)
{
    m_seen = false;
}


//-----------------------------------------------------------------------------
void
base::finish (void)
{
    if (m_required && !m_seen)
        throw invalid_required (m_name);
}


//-----------------------------------------------------------------------------
std::string
base::name (void) const
{
    return m_name;
}


//-----------------------------------------------------------------------------
bool
base::required (void) const
{
    return m_required;
}


//-----------------------------------------------------------------------------
bool
base::required (bool _required)
{
    return m_required = _required;
}


//-----------------------------------------------------------------------------
bool
base::seen (void) const
{
    return m_seen;
}


//-----------------------------------------------------------------------------
bool
base::seen (bool _seen)
{
    return m_seen = _seen;
}


///////////////////////////////////////////////////////////////////////////////
null::null (std::string _name):
    base (std::move (_name))
{ ; }


//-----------------------------------------------------------------------------
void
null::execute (void)
{
    seen (true);
}


//-----------------------------------------------------------------------------
void
null::execute (const char *restrict)
{
    seen (true);
}


///////////////////////////////////////////////////////////////////////////////
present::present (std::string _name, bool &_data):
    base (std::move (_name)),
    m_data (_data)
{ ; }


//-----------------------------------------------------------------------------
void
present::execute (void)
{
    seen (true);
}


//-----------------------------------------------------------------------------
void
present::finish (void)
{
    m_data = seen ();
    base::finish ();
}


///////////////////////////////////////////////////////////////////////////////
namespace util { namespace cmdopt { namespace option {
    template <>
    void
    value<bool>::execute (const char *restrict str)
    {
        static const std::string TRUE_STRING[] = {
            "true",
            "yes",
            "y",
            "1"
        };

        if (std::any_of (std::begin (TRUE_STRING),
                         std::end (TRUE_STRING),
                         [str] (auto i) { return i == str; }))
        {
            m_data = true;
            return;
        }

        static const std::string FALSE_STRING[] = {
            "false",
            "no",
            "n",
            "0"
        };

        if (std::any_of (std::begin (FALSE_STRING),
                         std::end (FALSE_STRING),
                         [str] (auto i) { return i == str; }))
        {
            m_data = false;
            return;
        }

        base::execute (str);
        seen (true);
    }
} } }


//-----------------------------------------------------------------------------
namespace util { namespace cmdopt { namespace option {
    template class value<uint16_t>;
    template class value<uint32_t>;
    template class value<uint64_t>;
} } }


///////////////////////////////////////////////////////////////////////////////
template <typename T>
count<T>::count (std::string _name, T &_data):
    value<T> (std::move (_name), _data)
{ ; }


//-------------------------------------------------------------------------
template <typename T>
void
count<T>::execute (void)
{
    ++this->data ();
    this->seen (true);
}


//-----------------------------------------------------------------------------
namespace util { namespace cmdopt { namespace option {
    template class count<unsigned>;
} } }


///////////////////////////////////////////////////////////////////////////////
static size_t
suffix_to_multiplier (char c)
{
    switch (c) {
    case 'e':
    case 'E':
        return pow (1024UL, 6);

    case 'p':
    case 'P':
        return pow (1024UL, 5);

    case 't':
    case 'T':
        return pow (1024UL, 4);

    case 'g':
    case 'G':
        return pow (1024UL, 3);

    case 'm':
    case 'M':
        return pow (1024UL, 2);

    case 'k':
    case 'K':
        return pow (1024UL, 1);

    default:
        throw util::cmdopt::invalid_value ("bytes");
    }
}


void
bytes::execute (const char *restrict str)
{
    const char *tail;
    const char *last = str + strlen (str);
    unsigned long val = std::strtoul (const_cast<char *> (str), const_cast<char**> (&tail), 10);

    CHECK_LE (tail, last);

    if (tail == str) {
        throw invalid_value (name ());
    } else if (tail == last) {
        data (val);
    } else if (tail + 1 == last) {
        data (val * suffix_to_multiplier (*tail));
    } else
        throw invalid_value (name ());
}


///////////////////////////////////////////////////////////////////////////////
int
parser::scan (int argc, const char *const *argv)
{
    CHECK_GE (argc, 0);
    CHECK    (argv);

    for (auto &j: m_options)
        j->start ();

    // start iterating after our program's name
    int i = 1;
    while (i < argc) {
        auto len = strlen (argv[i]);

        // bail if there's no potential for an option
        if (len < 2 || argv[i][0] != '-')
            return i;

        // parse longopt
        auto inc = argv[i][1] == '-'
             ? parse_long  (i, argc, argv)
             : parse_short (i, argc, argv);
        
        CHECK_GT (inc, 0);
        i += inc;
    }

    for (auto &j: m_options)
        j->finish ();

    return i;
}


//-----------------------------------------------------------------------------
int
parser::parse_long (int pos, int argc, const char *const *argv)
{
    CHECK_LT (pos, argc);
    CHECK_GE (pos, 0);
    CHECK_GE (argc, 0);
    CHECK    (argv);

    CHECK_EQ (argv[pos][0], '-');
    CHECK_EQ (argv[pos][1], '-');

    // break first atom into components and extract the key
    const char *start = argv[pos] + 2;
    const char *eq    = strchr (start, '=');
    const char *last  = start + strlen (start);

    std::string key { start, eq ? eq : last };

    // find the handler
    auto handle_pos = std::find_if (m_long.begin (),
                                 m_long.end (),
                                 [&] (auto i) { return std::get<0> (i) == key; });
    if (handle_pos == m_long.end ())
        throw invalid_key (key);

    auto &handler = std::get<1> (*handle_pos);

    // maybe grab a value from the next atom and dispatch
    if (!eq) {
        // check the next atom for the value
        if (pos + 1 < argc)
            if (argv[pos + 1][0] != '-') {
                handler.execute (argv[pos+1]);
                return 2;
            }

        handler.execute ();
    } else {
        handler.execute (eq+1);
    }

    return 1;
}


//-----------------------------------------------------------------------------
int
parser::parse_short (int pos, int argc, const char *const *argv)
{
    CHECK_LT (pos, argc);
    CHECK_GE (pos, 0);
    CHECK_GE (argc, 0);
    CHECK    (argv);

    CHECK_EQ  (argv[pos][0], '-');
    CHECK_NEQ (argv[pos][1], '-');

    // we have a run of no-value keys
    auto len = strlen (argv[pos]);
    if (len > 2 || pos + 1 == argc || argv[pos+1][0] == '-') {
        for (size_t i = 1; i < len; ++i) {
            auto letter = argv[pos][i];

            auto hpos = std::find_if (m_short.begin (),
                                      m_short.end (),
                                      [letter] (auto j) { return std::get<0> (j) == letter; });
            if (hpos == m_short.end ())
                throw invalid_key (std::to_string (letter));
            std::get<1> (*hpos).execute ();
        }

        return 1;
    }

    // we have a value following
    auto letter = argv[pos][1];
    auto hpos = std::find_if (m_short.begin (),
                              m_short.end (),
                              [letter] (auto i) { return std::get<0> (i) == letter; });
    if (hpos == m_short.end ())
        throw invalid_key (std::to_string (letter));
    std::get<1> (*hpos).execute (argv[pos+1]);

    return 2;
}
