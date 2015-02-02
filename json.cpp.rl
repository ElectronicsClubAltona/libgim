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
 * Copyright 2010-2012 Danny Robson <danny@nerdcruft.net>
 */


#include "json.hpp"

#include "debug.hpp"
#include "io.hpp"
#include "maths.hpp"

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


using namespace std;
using namespace util;

//-----------------------------------------------------------------------------
// Parsing

struct parse_context {
    parse_context(json::tree::node *_root):
        root  (_root),
        value (NULL),
        key   (NULL),
        start (NULL),
        stop  (NULL)
    { ; }

    json::tree::node *root,
               *value,
               *key;
    const char *start,
               *stop;
};


%%{
    machine json;

    ## Record whether parsing was successful for future use
    action success
    { __success = true; }

    action failure {
        __success = false;
        /*std::cerr << std::endl
                  << "Failure on: '" << fc << "' in level " << top << " at " << fpc - p
                  << std::endl;
        */
    }


    action new_object { nodestack.push_back (parse_context(new json::tree::object));  }
    action new_array  { nodestack.push_back (parse_context(new json::tree::array));  }

    action new_object_value {
        CHECK (nodestack.back ().root->is_object ());
        CHECK (nodestack.back ().key);
        CHECK (nodestack.back ().value);

        if (!nodestack.back ().key->is_string ())
            throw parse_error ("object keys must be strings");

        json::tree::object *object = (json::tree::object*)nodestack.back ().root;
        object->insert (nodestack.back ().key->as_string (),
                        unique_ptr<json::tree::node> (nodestack.back ().value));
        nodestack.back ().key   = NULL;
        nodestack.back ().value = NULL;
    }

    action new_array_value {
        CHECK (nodestack.back ().root->is_array ());
        CHECK (nodestack.back ().value);

        json::tree::array *array = (json::tree::array *)nodestack.back ().root;
        array->insert (unique_ptr<json::tree::node> (nodestack.back ().value));
        nodestack.back ().value = NULL;
    }

    action new_string  {
        CHECK (!nodestack.empty ());
        CHECK (!nodestack.back ().value);

        std::string value (std::string (nodestack.back ().start,
                                        nodestack.back ().stop));
        nodestack.back ().value = new json::tree::string(value);
    }

    action new_boolean {
        CHECK (!nodestack.empty ());
        CHECK (!nodestack.back ().value);

        throw parse_error ("unable to parse boolean");
    }

    action new_number  {
        CHECK (!nodestack.empty ());

        parse_context &back = nodestack.back ();
        CHECK (!back.value);
        CHECK (back.start);
        CHECK (back.stop);
        CHECK_LE (back.start, back.stop);

        errno = 0;
        char *end;
        double value = strtod (back.start, &end);
        if (end == back.start || errno)
            throw parse_error ("unable to parse number");
        back.value = new json::tree::number (value);
    }

    action new_null    {
        CHECK (!nodestack.empty ());
        CHECK (!nodestack.back ().value);

        nodestack.back().value = new json::tree::null ();
    }

    action new_object_key {
        CHECK (!nodestack.empty ());
        CHECK (nodestack.back ().root->is_object ());
        CHECK (nodestack.back ().value);
        CHECK (!nodestack.back ().key);

        nodestack.back ().key   = nodestack.back ().value;
        nodestack.back ().value = NULL;
    }

    prepush {
        fsmstack.push_back  (0);
    }

    postpop {
        fsmstack.pop_back ();
        __root = nodestack.back ().root;
        if (nodestack.size () > 1)
            (nodestack.rbegin () + 1)->value = nodestack.back ().root;
        nodestack.pop_back ();
    }

    variable stack fsmstack;

    alphtype char;

    ## numerical
    exp      =   [eE]('-' | '+')? digit+;
    frac     =   '.' digit+;
    int      =   '-'? [1-9]? digit+;

    number   =   int (  frac
                      | exp
                      | frac exp)?;

    ## textual
    char     =
          any - (cntrl | '\"' | '\\')
        | '\\\"'
        | '\\\\'
        | '\\/'
        | '\\b'
        | '\\f'
        | '\\n'
        | '\\r'
        | '\\t'
        | '\\u' xdigit{4};

    string = ('"'
              char* >{ nodestack.back ().start = fpc; }
                    %{ nodestack.back ().stop  = fpc; })
              '"'
              @new_string;

    ## other
    boolean =
          'true'  @{ nodestack.back ().value = new json::tree::boolean ( true); }
        | 'false' @{ nodestack.back ().value = new json::tree::boolean (false); };

    ## components
    object   = '{' @{ fhold; fcall _object; } '}';
    array    = '[' @{ fhold; fcall  _array; } ']';

    value    =
          string
        | boolean
        | number  >{ nodestack.back ().start = fpc; } %{ nodestack.back ().stop = fpc; } %new_number
        | object
        | array
        | 'null'  %new_null;

    ## compound data types
    _array   := ('[' @new_array
                  space* ((value %new_array_value space* ',' space*)* value %new_array_value space*)?
              ']')
              $!failure
              @{ fhold; fret; };

    pair     = string %new_object_key space* ':' space* value %new_object_value;
    _object := ('{' @new_object
                 space* ((pair space* ',' space*)* pair space*)?
             '}')
             $!failure
             @{ fhold; fret; };

    json := (space* (object | array) space*)
            $!failure
            %success
            >{ __success = false; };

    write data;
}%%


//-----------------------------------------------------------------------------
// External support

template <>
bool
is_integer (const json::tree::number &node) {
    return is_integer (node.native ());
}


template <>
bool
is_integer (const json::tree::node &node) {
    return node.is_number () &&
           is_integer (node.as_number ());
}


//-----------------------------------------------------------------------------
// Node

std::unique_ptr<json::tree::node>
json::tree::parse (const boost::filesystem::path &path) {
    auto data = slurp (path);
    return parse (static_cast <const char *> (data.get ()));
}


std::unique_ptr<json::tree::node>
json::tree::parse (const std::string &path)
    { return parse (path.c_str (), path.c_str () + path.size ()); }

std::unique_ptr<json::tree::node>
json::tree::parse (const char *start,
             const char *stop) {
    bool __success = true;
    json::tree::node *__root = nullptr;
    size_t top = 0;
    int cs;
    deque <int> fsmstack;
    deque <parse_context> nodestack;

    const char *p   = start,
               *pe  = stop,
               *eof = stop;

    %%write init;
    %%write exec;

    if (!__success) {
        std::ostringstream os;
        os << "unable to parse json at char " << (p - start);
        throw parse_error (os.str ());
    }

    return std::unique_ptr<json::tree::node> (__root);
}


std::unique_ptr<json::tree::node>
json::tree::parse (const char *start)
    { return parse (start, start + strlen (start)); }


void
json::tree::write (const json::tree::node &node, std::ostream &os)
    { node.write (os); }


//-----------------------------------------------------------------------------
// Type conversion

const json::tree::object&
json::tree::node::as_object  (void) const
        { throw type_error ("node is not an object"); }


const json::tree::array&
json::tree::node::as_array   (void) const
        { throw type_error ("node is not an array"); }


const json::tree::string&
json::tree::node::as_string  (void) const
        { throw type_error ("node is not a string"); }


const json::tree::number&
json::tree::node::as_number  (void) const
        { throw type_error ("node is not a number"); }


const json::tree::boolean&
json::tree::node::as_boolean (void) const
        { throw type_error ("node is not a boolean"); }


const json::tree::null&
json::tree::node::as_null (void) const
        { throw type_error ("node is not a null"); }


//-----------------------------------------------------------------------------
// Global operatoers

bool
json::tree::node::operator!= (const node &rhs) const
    { return !(*this == rhs); }


bool json::tree::node::operator==(const char *rhs) const {
    try {
        return as_string ().native () == rhs;
    } catch (const json::tree::type_error&) {
        return false;
    }
}


const json::tree::node&
json::tree::node::operator[] (const std::string &key) const
    { return as_object ()[key]; }


const json::tree::node&
json::tree::node::operator[] (unsigned int idx) const
    { return as_array()[idx]; }


//-----------------------------------------------------------------------------
// Object

json::tree::object::~object ()
    { ; }


bool
json::tree::object::operator ==(const json::tree::object &rhs) const {
    for (auto i = rhs.m_values.begin (), j = m_values.begin ();
         i != rhs.m_values.end () && j != m_values.end ();
         ++i, ++j)
    {
        if (i->first != j->first)
            return false;
        if ((*i->second) != (*j->second))
            return false;
    }

    return true;
}


void
json::tree::object::insert (const std::string &_key, unique_ptr<json::tree::node> &&value)
        { m_values[_key] = move(value); }


const json::tree::node&
json::tree::object::operator[](const std::string &key) const {
    auto value = m_values.find (key);
    if (value == m_values.end ()) {
        ostringstream ss;
        ss << "no key: " << key;
        throw json::tree::error (ss.str());
    }

    return *value->second;
}


bool
json::tree::object::has (const std::string &key) const {
    return m_values.find (key) != m_values.end ();
}


void
json::tree::object::clear (void)
    { m_values.clear (); }


void
json::tree::object::erase (const std::string &key) {
    auto pos = m_values.find (key);
    if (pos == m_values.end ())
        throw json::tree::error ("erasing invalid key");

    m_values.erase (key);
}


json::tree::object::const_iterator
json::tree::object::begin (void) const
    { return m_values.begin (); }


json::tree::object::const_iterator
json::tree::object::end (void) const
    { return m_values.end (); }


std::ostream&
json::tree::object::write (std::ostream &os) const {
    os << "{\n";
    {
        indenter raii(os);

        for (auto i = m_values.begin (); i != m_values.end ();) {
            os << '"' << i->first << "\" : " << *i->second;

            if (++i != m_values.end ())
                os << ",\n";
        }
    }

    os << "\n}";
    return os;
}


//-----------------------------------------------------------------------------
// Array

json::tree::array::~array()
{
    m_values.clear ();
}


void
json::tree::array::insert (unique_ptr<json::tree::node> &&_value)
{
    m_values.push_back (move (_value));
}


bool
json::tree::array::operator ==(const json::tree::array &rhs) const {
    for (auto i = rhs.m_values.begin (), j = m_values.begin ();
         i != rhs.m_values.end () && j != m_values.end ();
         ++i, ++j)
    { if ((**i) != (**j)) return false; }

    return true;
}


std::ostream&
json::tree::array::write (std::ostream &os) const {
    os << "[\n";
    {
        indenter raii(os);

        for (auto i = m_values.begin (); i != m_values.end (); ++i) {
            (*i)->write (os);

            if (i != m_values.end () - 1)
                os << ",\n";
        }
    }
    os << "\n]";
    return os;
}


//-----------------------------------------------------------------------------
// String

std::ostream&
json::tree::string::write (std::ostream &os) const {
    os << '"' << m_value << '"';
    return os;
}


bool
json::tree::string::operator ==(const json::tree::string &rhs) const
    { return rhs.m_value == m_value; }


bool
json::tree::string::operator ==(const char *rhs) const
    { return rhs == m_value; }


//-----------------------------------------------------------------------------
// Number

std::ostream&
json::tree::number::write (std::ostream &os) const {
    os << setprecision (numeric_limits<double>::digits10) << m_value;
    return os;
}


bool
json::tree::number::operator ==(const json::tree::number &rhs) const
    { return almost_equal (rhs.m_value, m_value); }


//-----------------------------------------------------------------------------
// Boolean

std::ostream&
json::tree::boolean::write (std::ostream &os) const {
    os << (m_value ? "true" : "false");
    return os;
}

bool
json::tree::boolean::operator ==(const json::tree::boolean &rhs) const
    { return rhs.m_value == m_value; }


//-----------------------------------------------------------------------------
// Null

std::ostream&
json::tree::null::write (std::ostream &os) const {
    os << "null";
    return os;
}

ostream&
json::tree::operator <<(ostream &os, const json::tree::node &n)
    { return n.write (os); }


//-----------------------------------------------------------------------------
// to_json

namespace json { namespace tree {
    template <>
    std::unique_ptr<node>
    io<bool>::serialise (const bool &b) {
        return std::unique_ptr<node> (new boolean (b));
    }

    template <>
    std::unique_ptr<node>
    io<nullptr_t>::serialise (const nullptr_t &) {
        return std::unique_ptr<node> (new null ());
    }

    template <>
    std::unique_ptr<node>
    io<std::string>::serialise (const std::string &s) {
        return std::unique_ptr<node> (new string (s));
    }

    template <>
    std::unique_ptr<node>
    io<int>::serialise (const int &i) {
        return std::unique_ptr<node> (new number (i));
    }

    template <>
    std::unique_ptr<node>
    io<size_t>::serialise (const size_t &i) {
        return std::unique_ptr<node> (new number (i));
    }

    template <>
    std::unique_ptr<node>
    io<float>::serialise (const float &f) {
        return std::unique_ptr<node> (new number (f));
    }
} }
