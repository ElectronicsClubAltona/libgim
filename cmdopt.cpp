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
 * Copyright 2013-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "./cmdopt.hpp"

#include "./cast.hpp"
#include "./debug.hpp"

#include <cstring>
#include <iostream>
#include <iomanip>

using namespace util::cmdopt;
using namespace util::cmdopt::option;


///////////////////////////////////////////////////////////////////////////////
base::~base ()
{ ; }


//-----------------------------------------------------------------------------
void
base::execute (void)
{
    throw invalid_null ();
}


//-----------------------------------------------------------------------------
void
base::execute (const char *restrict value)
{
    throw invalid_value (value);
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
        throw invalid_required ();
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


//-----------------------------------------------------------------------------
const std::string&
null::example (void) const
{
    static const std::string EXAMPLE;
    return EXAMPLE;
}


///////////////////////////////////////////////////////////////////////////////
present::present (bool &_data):
    m_data (_data)
{ ; }


//-----------------------------------------------------------------------------
void
present::execute (void)
{
    seen (true);
}


//-----------------------------------------------------------------------------
const std::string&
present::example (void) const
{
    static const std::string EXAMPLE;
    return EXAMPLE;
}


//-----------------------------------------------------------------------------
void
present::finish (void)
{
    m_data = seen ();
    base::finish ();
}


//-----------------------------------------------------------------------------
namespace util::cmdopt::option {
    template class value<uint16_t>;
    template class value<uint32_t>;
    template class value<uint64_t>;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
count<T>::count (T &_data):
    value<T> (_data)
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
namespace util::cmdopt::option {
    template class count<unsigned>;
}


///////////////////////////////////////////////////////////////////////////////
static size_t
suffix_to_multiplier (char c)
{
    switch (c) {
    case 'e':
    case 'E':
        return util::pow (1024UL, 6);

    case 'p':
    case 'P':
        return util::pow (1024UL, 5);

    case 't':
    case 'T':
        return util::pow (1024UL, 4);

    case 'g':
    case 'G':
        return util::pow (1024UL, 3);

    case 'm':
    case 'M':
        return util::pow (1024UL, 2);

    case 'k':
    case 'K':
        return util::pow (1024UL, 1);

    default:
        const char str[2] = { c, '\0' };
        throw std::invalid_argument (str);
    }
}


//-----------------------------------------------------------------------------
void
bytes::execute (const char *restrict str)
{
    const char *tail;
    const char *last = str + strlen (str);
    unsigned long val = std::strtoul (const_cast<char *> (str), const_cast<char**> (&tail), 10);

    CHECK_LE (tail, last);

    if (tail == str) {
        throw invalid_value (str);
    } else if (tail == last) {
        data (val);
    } else if (tail + 1 == last) {
        try {
            data (val * suffix_to_multiplier (*tail));
        } catch (const std::invalid_argument&)
        { throw invalid_value (str); }
    } else
        throw invalid_value (str);
}


///////////////////////////////////////////////////////////////////////////////
int
parser::scan (int argc, const char *const *argv)
{
    CHECK_GE (argc, 0);
    CHECK    (argv);

    for (auto &j: m_options)
        std::get<std::unique_ptr<option::base>> (j)->start ();

    // start iterating after our program's name
    int i = 1;
    while (i < argc) {
        auto len = strlen (argv[i]);

        // bail if there's no potential for an option
        if (len < 2 || argv[i][0] != '-')
            break;

        // stop processing named options on '--'
        if (len == 2 && argv[i][1] == '-') {
            ++i;
            break;
        }

        // parse longopt
        auto inc = argv[i][1] == '-'
             ? parse_long  (i, argc, argv)
             : parse_short (i, argc, argv);

        CHECK_GT (inc, 0);
        i += inc;
    }

    // process the positional arguments
    for (size_t cursor = 0; i < argc && cursor < m_positional.size (); ++i, ++cursor)
        m_positional[cursor].get ().execute (argv[i]);

    // ensure we've processed all the arguments
    if (i != argc)
        throw unhandled_argument (i);

    // allow arguments to check if they've been successfully handled
    for (auto &j: m_options)
        std::get<std::unique_ptr<option::base>> (j)->finish ();

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
    if (key == "help")
        print_help (argc, argv);

    // find the handler
    auto handle_pos = std::find_if (m_long.begin (),
                                 m_long.end (),
                                 [&] (auto i) { return std::get<std::string> (i) == key; });
    if (handle_pos == m_long.end ())
        throw invalid_key (key);

    auto &handler = std::get<option::base&> (*handle_pos);

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
            if (letter == 'h')
                print_help (argc, argv);

            auto hpos = std::find_if (m_short.begin (),
                                      m_short.end (),
                                      [letter] (auto j) { return std::get<char> (j) == letter; });
            if (hpos == m_short.end ())
                throw invalid_key (std::to_string (letter));
            std::get<option::base&> (*hpos).execute ();
        }

        return 1;
    }

    // we have a value following
    auto letter = argv[pos][1];
    auto hpos = std::find_if (m_short.begin (),
                              m_short.end (),
                              [letter] (auto i) { return std::get<char> (i) == letter; });
    if (hpos == m_short.end ())
        throw invalid_key (std::to_string (letter));
    std::get<option::base&> (*hpos).execute (argv[pos+1]);

    return 2;
}


///////////////////////////////////////////////////////////////////////////////
void
parser::print_help (const int argc,
                    const char *const *argv) const
{
    (void)argc;

    CHECK_EQ (m_short.size (), m_options.size ());
    CHECK_EQ (m_long.size (), m_options.size ());

    if (m_options.empty ())
        exit (0);

    // find the longest long form argument so we can set field alignment 
    auto largestwidth = std::max_element (
        m_long.begin (),
        m_long.end (),
        [] (const auto &a, const auto &b)
    {
        return std::get<std::string> (a).size () < std::get<std::string> (b).size ();
    });
    auto longwidth = std::get<std::string> (*largestwidth).size ();

    // find the longest example text
    auto largestexample = std::max_element (
        m_options.cbegin (),
        m_options.cend (),
        [] (const auto &a, const auto &b)
    {
        const auto &example_a = std::get<std::unique_ptr<option::base>> (a)->example ();
        const auto &example_b = std::get<std::unique_ptr<option::base>> (b)->example ();

        return example_a.size () > example_b.size ();
    });

    auto longexample = std::get<std::unique_ptr<option::base>> (*largestexample)->example ().size ();

    // field width requires an alignment. we don't care about preserving
    // state as we're about to bail anyway
    std::cout << std::left;

    // print all the option info
    std::cout << "usage: " << argv[0] << '\n';

    for (auto &o: m_options) {
        auto ptr = std::get<std::unique_ptr<option::base>> (o).get ();

        auto s = std::find_if (
            std::cbegin (m_short),
            std::cend   (m_short),
            [ptr] (auto j) {
                return &std::get<option::base&> (j) == ptr;
            }
        );

        auto l = std::find_if (
            std::cbegin (m_long),
            std::cend   (m_long),
            [ptr] (auto j) {
                return &std::get<option::base&> (j) == ptr;
            }
        );

        std::cout << '\t';
        if (s != std::cend (m_short))
            std::cout << '-' << std::get<char> (*s) << '\t';
        else
            std::cout << '\t';

        std::cout << std::setw (trunc_cast<int> (longwidth));
        if (l != std::cend (m_long))
            std::cout << std::get<std::string> (*l) << '\t';
        else
            std::cout << ' ' << '\t';

        std::cout << std::setw (trunc_cast<int> (longexample)) << ptr->example () << '\t'
                  << std::setw (0) << std::get<std::string> (o)
                  << '\n';
    }

    exit (0);
}


///////////////////////////////////////////////////////////////////////////////
invalid_key::invalid_key (std::string _key):
    m_key (std::move (_key))
{ ; }


//-----------------------------------------------------------------------------
const char*
invalid_key::what (void) const noexcept
{
    return m_key.c_str ();
}


///////////////////////////////////////////////////////////////////////////////
invalid_value::invalid_value (std::string _value):
    m_value (_value)
{ ; }


//-----------------------------------------------------------------------------
const char*
invalid_value::what (void) const noexcept
{
    return m_value.c_str ();
}


///////////////////////////////////////////////////////////////////////////////
const char*
invalid_null::what (void) const noexcept
{
    static const char WHAT[] = "unexpected null option was encountered";
    return WHAT;
}


///////////////////////////////////////////////////////////////////////////////
const char*
invalid_required::what (void) const noexcept
{
    static const char WHAT[] = "required option not seen";
    return WHAT;
}


///////////////////////////////////////////////////////////////////////////////
unhandled_argument::unhandled_argument (int _index):
    m_index (_index)
{ ; }


//-----------------------------------------------------------------------------
int
unhandled_argument::index (void) const noexcept
{
    return m_index;
}


//-----------------------------------------------------------------------------
const char*
unhandled_argument::what (void) const noexcept
{
    static const char WHAT[] = "unhandled argument";
    return WHAT;
}
