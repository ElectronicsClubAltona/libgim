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


#include "options.hpp"

#include "config.h"

#include "debug.hpp"
#include "string.hpp"
#include "types.hpp"
#include "types/casts.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstring>
#include <iostream>
#include <stdexcept>


using namespace std;
using namespace util;


/*
 * Generic option operations, failure or default modes
 */


option::option (char        _letter,
                const char *_name,
                const char *_desc,
                bool        _required):
    m_shortopt   (_letter),
    m_longopt    (_name),
    m_description(_desc),
    m_required   (_required),
    m_found      (false)
{ reset(); }



void
option::execute (void) {
    throw runtime_error(
        "Cannot provide no value for the option '" + m_longopt + "'"
    );
}


void
option::execute (const string& data) {
    assert(data.size() > 0);
    throw runtime_error(
        "Cannot provide a value for the option '" + m_longopt + "'"
    );
}


ostream&
operator<< (ostream & os, const option& opt) {
    os  << (opt.is_required () ? " -"  : "[-" ) << opt.shortopt ()
        << (opt.is_required () ? " \t" : "]\t") << opt.longopt  ()
        << "\t"   << opt.description ();
    return os;
}


void
option::finish (void) {
    if (m_required && !m_found)
        throw runtime_error ("Required argument not found: " + m_longopt);
}


/*
 * Nulloption
 */

nulloption::nulloption (char        _letter,
                        const char *_name,
                        const char *_desc,
                        bool        _required):
    option (_letter, _name, _desc, _required)
{ ; }


/*
 * Present option
 */


presentoption::presentoption (char        _letter,
                              const char *_name,
                              const char *_desc,
                              bool       *_data,
                              bool        _required):
    option (_letter, _name, _desc, _required),
    m_data (_data)
{ ; }


void
presentoption::execute (void) {
    *m_data = true;
    m_found = true;
}


/*
 * Value option
 */


namespace util {
    template<>
    bool&
    valueoption<bool>::get_arg(const std::string& arg, bool *val) {
        if      (arg == "true"  || arg == "yes" || arg == "1")
            *val = true;
        else if (arg == "false" || arg == "no"  || arg == "0")
            *val = false;
        else
            throw std::domain_error("Invalid form for boolean argument");

        return *val;
    }


    template <typename T>
    T&
    valueoption<T>::get_arg(const std::string &arg,
                            T                 *val)
    {
        std::istringstream stream (arg, std::istringstream::in);
        stream.exceptions (
              std::istringstream::failbit
            | std::istringstream::badbit
        );

        stream >> *val;
        return *val;
    }
}


template std::string& util::valueoption<std::string>::get_arg (const std::string&, std::string*);

template  int16_t& util::valueoption< int16_t>::get_arg (const std::string&,  int16_t*);
template  int32_t& util::valueoption< int32_t>::get_arg (const std::string&,  int32_t*);
template  int64_t& util::valueoption< int64_t>::get_arg (const std::string&,  int64_t*);

template uint16_t& util::valueoption<uint16_t>::get_arg (const std::string&, uint16_t*);
template uint32_t& util::valueoption<uint32_t>::get_arg (const std::string&, uint32_t*);
template uint64_t& util::valueoption<uint64_t>::get_arg (const std::string&, uint64_t*);


/*
 * bytesoption
 */

bytesoption::bytesoption (char           _letter,
                          const char    *_name,
                          const char    *_desc,
                          size_t        *_data,
                          bytestype      _type,
                          bytesmodifier  _modifier,
                          bool           _required):
    valueoption<size_t> (_letter, _name, _desc, _data, _required),
    m_type (_type),
    m_modifier (_modifier)
{ ; }


bytesoption::bytestype
bytesoption::type_from_character (char c) {
    switch (c) {
        case 'e':
        case 'E':
            return BYTES_EXA;

        case 'p':
        case 'P':
            return BYTES_PETA;

        case 't':
        case 'T':
            return BYTES_TERA;

        case 'g':
        case 'G':
            return BYTES_GIGA;

        case 'm':
        case 'M':
            return BYTES_MEGA;

        case 'k':
        case 'K':
            return BYTES_KILO;

        default:
            throw domain_error("Invalid magnitude specifier");
    }
}


void
bytesoption::execute (const std::string& data) {
    // We shouldn't get this far into processing with a zero sized argument,
    // that's what the null data execute function is for.
    assert (data.size () > 0);

    size_t defaultvalue = *m_data;

    try {
        bytesmodifier modifier = m_modifier;
        size_t cursor = data.size () - 1;

        // Consume an optional trailing `byte' type
        if (data[cursor] == 'B' || data[cursor] == 'b') {
            if (cursor == 0)
                throw invalid_argument ("Size is too short");
            --cursor;
        }

        // Check if we explicitly request base2
        if (data[cursor] == 'i') {
            modifier = BYTES_BASE2;
            if (cursor == 0)
                throw invalid_argument("Size is too short");
            --cursor;
        }

        // Determine what constant factor is needed for the raw size
        uint64_t multiplier = 1;
        uint64_t modifier_factor;
        switch (modifier) {
            case BYTES_BASE2:
                modifier_factor = 1024;
                break;

            case BYTES_BASE10:
                modifier_factor = 1000;
                break;

            default:
                unreachable ();
        }

        // Find the difference in magnitude between what is desired, and what
        // is specified by the user. Raise the multiplier by that magnitude.
        bytestype specified = m_type;
        try {
            specified = type_from_character (data[cursor]);
            // If the character is a digit, it just means the user skipped the
            // size specifier, which is ok.
        } catch (domain_error &x) {
            if (!isdigit (data[cursor]))
                throw invalid_argument ("Not a size");

            // Falsely increment the cursor if there's no size specifier...
            ++cursor;
        }

        // ... so that we can easily decrement the cursor without special logic
        // after reading the specifiers.
        --cursor;

        multiplier = static_cast<uintmax_t> (std::pow (modifier_factor, (int)specified));
        get_arg (data.substr(0, cursor + 1), m_data);
        *m_data *= multiplier;
    } catch (...) {
        // Ensure that we haven't nuked a default value due to a half
        // completed calculation followed by an exception.

        *m_data = defaultvalue;
        throw;
    }

    m_found = true;
}


/*
 * Internal helper options. Print help and usage.
 * A callback to the processor which triggers output of the help message.
 *
 * This should never be instanced by a user of the system. The processor will
 * automatically adds this to its available options where needed.
 */
class helpoption : public option {
    protected:
        static const char  HELP_CHARACTER;
        static const char *HELP_NAME;
        static const char *HELP_DESCRIPTION;

        processor * m_processor;

    public:
        helpoption (processor * _processor):
            option (HELP_CHARACTER, HELP_NAME, HELP_DESCRIPTION, false),
            m_processor (_processor)
        { ; }


        virtual void execute (void);
        virtual void execute (const std::string& data)
            { option::execute (data); }
};


const char  helpoption::HELP_CHARACTER   = 'h';
const char *helpoption::HELP_NAME        = "help";
const char *helpoption::HELP_DESCRIPTION =
    "display help and usage information";


void
helpoption::execute (void) {
    m_processor->print_usage ();
    exit (EXIT_SUCCESS);
}


/*
 * Command line processing options
 */

processor::processor () {
    add_option (make_unique<helpoption> (this));
}


processor::~processor ()
{ ; }


void
processor::print_usage (void) {
    cout << "Usage: " << m_command << " [options]" << endl;

    for (const auto &i: m_options)
        cout << '\t' << *i << endl;
}


/**
 * Parse a single argument in short form. We are given an offset into the
 * argument array, and arguments. Establishes whether a value is present and
 * passes control over to the option.
 *
 * The format of the options are "-f [b]" || "-abcde"
 *
 * @param pos   the current offset into the argument array
 * @param argc  the size of the argument array
 * @param argv  the argument array given to the application
 *
 * @return the number of tokens consumed for this option; must be at least 1.
 */
unsigned int
processor::parse_short (int pos, int argc, const char **argv) {
    assert (pos > 0);
    assert (pos < argc);

    assert (argv[pos] != NULL);
    const char *arg = argv[pos];

    // Must begin with a dash, then have at least one non-dash character
    assert (arg[0] == '-');
    assert (arg[1] != '-');
    assert (strlen(arg) >= 2);

    // Check if we have a value option, Ie `-a [val]`
    //  First: must be of form '-[a-zA-Z]'.
    if (strlen (arg) == 2) {
        // Second: the next segment (which contains the value) shouldn't begin
        // with a dash.
        if (pos + 1 < argc && argv[pos+1][0] != '-') {
            option * o = m_shortopt[arg[1]];
            if (!o)
                throw runtime_error ("Cannot match option");

            o->execute (argv[pos+1]);
            return 2;
        }
    }

    // We must have a string of no-value flags, `-abcdef...`, execute each
    // option in the list after the dash.
    for(unsigned int i = 1; i < strlen (arg); ++i) {
        option * o = m_shortopt[arg[i]];

        if (!o)
            throw runtime_error ("Cannot match option");
        o->execute ();
    }

    return 1;
}


/**
 * Parse a single argument in long form. We are given an offset into the
 * argument array, and arguments. Establishes whether a value is present and
 * passes control over to the option.
 *
 * The format of the options are "--foo[=bar]"
 *
 * @param pos   the current offset into the argument array
 * @param argc  the size of the argument array
 * @param argv  the argument array given to the application`--foo[=bar]`
 *
 * @return the number of tokens consumed for this option; must be 1.
 */
unsigned int
processor::parse_long (int pos, int argc, const char ** argv) {
    assert (pos > 0);
    assert (pos < argc);

    assert (argv[pos] != NULL);
    const char *arg = argv[pos];

    // We must have at least two hyphens, and two letters (else a short opt)
    assert (arg[0] == '-');
    assert (arg[1] == '-');
    assert (strlen (arg) >= 4);

    // Skip past the dashes to the argument name
    arg += 2;

    // If there's an equals it's has a value to extract
    const char *data = strchr (arg, '=');
    if (data) {
        arg = strndup (arg, sign_cast<size_t> (data - arg));
        //arg = strndup (arg, data - arg); // Copy just the arg name
        ++data;                          // Skip the '='
    }

    option *o = m_longopt[arg];
    if (!o)
        throw runtime_error ("Cannot match option");

    if (data)
        o->execute (data);
    else
        o->execute ();

    return 1;
}


/**
 * Pass long and short options to specific parsing handlers.
 *
 * Establishes enough context to determine if the argument is long, short, or
 * none. The functions parse_long and parse_short are given the task of
 * dispatching control to the relevant option handlers.
 *
 * @param argc the raw parameter from the application main
 * @param argv the raw parameter from the application main
 */

void
processor::parse_args (int argc, const char ** argv) {
    // While technically we can have zero arguments, for practical purposes
    // this is a reasonable method at catching odd errors in the library.
    assert (argc > 0);
    assert (argv[0]);

    m_command.assign (argv[0]);

    // Must make sure each option has a chance to reset state (clear flags,
    // etc) between parses
    for (auto &i: m_options)
        i->reset ();

    const int FIRST_ARGUMENT = 1;
    try {
        for (int i = FIRST_ARGUMENT; i < argc; ++i) {
            // An argument must begin with a dash, if not we've reached the
            // end of the argument lists or we have a parsing error.
            if (argv[i][0] != '-')
                return;

            // An argument must consist of at least one non-dash character
            if (strlen (argv[i]) <= 1)
                throw runtime_error ("Invalid argument");

            // Actually hand off args to be parsed. Subtract one from the
            // tokens consumed, as the for loop increments tokens too.
            unsigned int consumed;
            if (argv[i][1] != '-')
                consumed = parse_short (i, argc, argv);
            else
                consumed = parse_long (i, argc, argv);

            assert (consumed >= 1);
            i += sign_cast<int> (consumed - 1);
        }
    } catch (runtime_error &x) {
        print_usage ();
        exit (EXIT_FAILURE);
    }

    for (auto &i: m_options)
        i->finish ();
}


void
processor::add_option (std::unique_ptr<option> opt) {
    if (m_shortopt.find (opt->shortopt ()) != m_shortopt.end ())
        throw logic_error ("Short option already exists");
    if (m_longopt.find (opt->longopt   ()) != m_longopt.end ())
        throw logic_error ("Long option already exists");

    m_shortopt[opt->shortopt ()] = opt.get ();
    m_longopt [opt->longopt  ()] = opt.get ();

    m_options.push_back (move (opt));
}


std::unique_ptr<option>
processor::remove_option (char letter) {
    // Locate the option by short name
    const auto s_candidate = m_shortopt.find (letter);
    if (s_candidate == m_shortopt.end ())
        throw logic_error ("Cannot remove an option which is not present");
    option *target = (*s_candidate).second;

    // Locate the long option entry
    const auto l_candidate = m_longopt.find (target->longopt ());
    assert (l_candidate != m_longopt.end ());

    // Remove all references and return
    auto prime = std::find_if (
        m_options.begin (),
        m_options.end (),
        [&target] (std::unique_ptr<option> &rhs) { return rhs.get () == target; }
    );
    std::unique_ptr<option> opt = move (*prime);

    m_shortopt.erase (s_candidate);
    m_longopt.erase  (l_candidate);
    m_options.erase  (prime);

    return opt;
}


std::unique_ptr<option>
processor::remove_option (const char *name) {
    // Locate the option by long name
    const auto l_candidate = m_longopt.find (name);
    if (l_candidate == m_longopt.end ())
        throw logic_error ("Cannot remove an option which is not present");
    option *target = (*l_candidate).second;

    // Locate the short option entry
    const auto s_candidate = m_shortopt.find (target->shortopt ());
    assert (s_candidate != m_shortopt.end ());

    // Remove all references and return
    auto prime = std::find_if (
        m_options.begin (),
        m_options.end (),
        [&target] (std::unique_ptr<option> &rhs) { return rhs.get () == target; }
    );
    std::unique_ptr<option> opt = move (*prime);

    m_shortopt.erase (s_candidate);
    m_longopt.erase  (l_candidate);
    m_options.erase  (prime);

    return opt;
}


/* Parse args from a stream, one arg per line
*/
//void parse_stream (std::istream & is) {
//    unique_ptr<char[]> buffer (new char [MAX_CHUNK_LENGTH + 1]);
//}

