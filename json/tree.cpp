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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */


#include "json/tree.hpp"

#include "json/except.hpp"
#include "json/flat.hpp"

#include "debug.hpp"
#include "io.hpp"
#include "maths.hpp"
#include "types/casts.hpp"

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


using json::tree::node;
using json::tree::object;
using json::tree::array;
using json::tree::string;
using json::tree::number;
using json::tree::boolean;
using json::tree::null;


///////////////////////////////////////////////////////////////////////////////
template <>
bool
is_integer (const json::tree::number &node)
{
    return is_integer (node.native ());
}


//-----------------------------------------------------------------------------
template <>
bool
is_integer (const json::tree::node &node)
{
    return node.is_number () &&
           is_integer (node.as_number ());
}


///////////////////////////////////////////////////////////////////////////////
static std::vector<json::flat::item>::const_iterator
parse (std::vector<json::flat::item>::const_iterator first,
       std::vector<json::flat::item>::const_iterator last,
       std::unique_ptr<json::tree::node> &output);


//-----------------------------------------------------------------------------
static std::vector<json::flat::item>::const_iterator
parse (std::vector<json::flat::item>::const_iterator first,
       std::vector<json::flat::item>::const_iterator last,
       json::tree::array &parent)
{
    for (auto cursor = first; cursor != last; ) {
        if (cursor->tag == json::flat::type::ARRAY_END)
            return cursor + 1;

        std::unique_ptr<json::tree::node> value;
        cursor = ::parse (cursor, last, value);
        parent.insert (std::move (value));
    }

    unreachable ();
}


//-----------------------------------------------------------------------------
static std::vector<json::flat::item>::const_iterator
parse (std::vector<json::flat::item>::const_iterator first,
       std::vector<json::flat::item>::const_iterator last,
       json::tree::object &parent)
{
    for (auto cursor = first; cursor != last; ) {
        if (cursor->tag == json::flat::type::OBJECT_END)
            return cursor + 1;

        CHECK_EQ (cursor->tag, json::flat::type::STRING);

        std::string key (cursor->first + 1, cursor->last - 1);
        ++cursor;

        std::unique_ptr<json::tree::node> val;
        cursor = ::parse (cursor, last, val);

        parent.insert (key, std::move (val));
    }

    unreachable ();
}


//-----------------------------------------------------------------------------
static std::vector<json::flat::item>::const_iterator
parse (std::vector<json::flat::item>::const_iterator first,
       std::vector<json::flat::item>::const_iterator last,
       std::unique_ptr<json::tree::node> &output)
{
    CHECK (first != last);
    CHECK (output.get () == nullptr);

    switch (first->tag) {
        case json::flat::type::NUL:
            output.reset (new json::tree::null ());
            return first + 1;

        case json::flat::type::BOOLEAN:
            CHECK (*first->first == 't' || *first->first == 'f');
            output.reset (new json::tree::boolean (*first->first == 't'));
            return first + 1;

        case json::flat::type::STRING:
            CHECK_NEQ (first->first, first->last);
            output.reset (new json::tree::string (first->first + 1, first->last - 1));
            return first + 1;

        case json::flat::type::INTEGER:
        case json::flat::type::REAL:
            output.reset (new json::tree::number (std::atof (first->first)));
            return first + 1;

        case json::flat::type::ARRAY_BEGIN: {
            auto value = std::make_unique<json::tree::array> ();
            auto cursor = ::parse (first + 1, last, *value);
            output = std::move (value);
            return cursor;
        }

        case json::flat::type::OBJECT_BEGIN: {
            auto value = std::make_unique<json::tree::object> ();
            auto cursor = ::parse (first + 1, last, *value);
            output = std::move (value);
            return cursor;
        }

        default:
            unreachable ();
    }
}


///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<json::tree::node>
json::tree::parse (const boost::filesystem::path &path)
{
    util::mapped_file f (path);
    return parse ((const char*)f.cbegin (), (const char*)f.cend ());
}


//-----------------------------------------------------------------------------
std::unique_ptr<json::tree::node>
json::tree::parse (const std::string &path)
    { return parse (path.c_str (), path.c_str () + path.size ()); }


//-----------------------------------------------------------------------------
std::unique_ptr<json::tree::node>
json::tree::parse (const char *start)
    { return parse (start, start + strlen (start)); }


//-----------------------------------------------------------------------------
void
json::tree::write (const json::tree::node &node, std::ostream &os)
    { node.write (os); }


//-----------------------------------------------------------------------------
std::unique_ptr<json::tree::node>
json::tree::parse (const char *first, const char *last)
{
    std::unique_ptr<json::tree::node> output;
    auto data = json::flat::parse (first, last);
    auto end  = ::parse (data.cbegin (), data.cend (), output);

    CHECK (end == data.cend ());
    (void)end;

    return output;
}


///////////////////////////////////////////////////////////////////////////////
// Type conversion

const json::tree::object&
json::tree::node::as_object  (void) const
    { throw json::type_error ("node is not an object"); }


//-----------------------------------------------------------------------------
const json::tree::array&
json::tree::node::as_array   (void) const
    { throw json::type_error ("node is not an array"); }


//-----------------------------------------------------------------------------
const json::tree::string&
json::tree::node::as_string  (void) const
    { throw json::type_error ("node is not a string"); }


//-----------------------------------------------------------------------------
const json::tree::number&
json::tree::node::as_number  (void) const
    { throw json::type_error ("node is not a number"); }


//-----------------------------------------------------------------------------
const json::tree::boolean&
json::tree::node::as_boolean (void) const
    { throw json::type_error ("node is not a boolean"); }


//-----------------------------------------------------------------------------
const json::tree::null&
json::tree::node::as_null (void) const
    { throw json::type_error ("node is not a null"); }


///////////////////////////////////////////////////////////////////////////////
json::tree::object&
json::tree::node::as_object  (void)
{ throw json::type_error ("node is not an object"); }


//-----------------------------------------------------------------------------
json::tree::array&
json::tree::node::as_array   (void)
    { throw json::type_error ("node is not an array"); }


//-----------------------------------------------------------------------------
json::tree::string&
json::tree::node::as_string  (void)
    { throw json::type_error ("node is not a string"); }


//-----------------------------------------------------------------------------
json::tree::number&
json::tree::node::as_number  (void)
    { throw json::type_error ("node is not a number"); }


//-----------------------------------------------------------------------------
json::tree::boolean&
json::tree::node::as_boolean (void)
    { throw json::type_error ("node is not a boolean"); }


//-----------------------------------------------------------------------------
json::tree::null&
json::tree::node::as_null (void)
    { throw json::type_error ("node is not a null"); }


///////////////////////////////////////////////////////////////////////////////
bool
json::tree::node::as_bool (void) const
{
    return !!as_boolean ();
}


//-----------------------------------------------------------------------------
float
json::tree::node::as_float (void) const
{
    return static_cast<float> (as_number ().native ());
}


//-----------------------------------------------------------------------------
double
json::tree::node::as_double (void) const
{
    return as_number ().native ();
}


//-----------------------------------------------------------------------------
size_t
json::tree::node::as_uint (void) const
{
    auto val = as_number ().native ();
    if (!is_integer (val))
        throw json::type_error ("cast fractional value to uint");

    // TODO: use trunc_cast
    return static_cast<size_t> (val);
}


//-----------------------------------------------------------------------------
const char*
json::tree::node::as_chars (void) const
{
    return as_string ().native ().c_str ();
}


///////////////////////////////////////////////////////////////////////////////
// Global operatoers
bool
json::tree::node::operator!= (const node &rhs) const
    { return !(*this == rhs); }


//-----------------------------------------------------------------------------
bool json::tree::node::operator==(const char *rhs) const
{
    try {
        return as_string ().native () == rhs;
    } catch (const json::type_error&) {
        return false;
    }
}


//-----------------------------------------------------------------------------
json::tree::node&
json::tree::node::operator[] (const std::string &key)
    { return as_object ()[key]; }


//-----------------------------------------------------------------------------
json::tree::node&
json::tree::node::operator[] (unsigned int idx)
    { return as_array()[idx]; }


//-----------------------------------------------------------------------------
const json::tree::node&
json::tree::node::operator[] (const std::string &key) const
    { return as_object ()[key]; }


//-----------------------------------------------------------------------------
const json::tree::node&
json::tree::node::operator[] (unsigned int idx) const
    { return as_array()[idx]; }


///////////////////////////////////////////////////////////////////////////////
// Object
json::tree::object::~object ()
    { ; }


//-----------------------------------------------------------------------------
std::unique_ptr<json::tree::node>
json::tree::object::clone (void) const
{
    auto obj = std::make_unique<json::tree::object> ();

    for (auto &i: *this)
        obj->insert (i.first, i.second->clone ());

    return std::move (obj);
}


//-----------------------------------------------------------------------------
bool
json::tree::object::operator ==(const json::tree::object &rhs) const
{
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


//-----------------------------------------------------------------------------
void
json::tree::object::insert (const std::string &_key, std::unique_ptr<json::tree::node> &&value)
{
    m_values[_key] = move(value);
}


//-----------------------------------------------------------------------------
json::tree::node&
json::tree::object::operator[](const std::string &key)
{
    auto value = m_values.find (key);
    if (value == m_values.end ())
        throw json::key_error (key);

    return *value->second;
}


//-----------------------------------------------------------------------------
const json::tree::node&
json::tree::object::operator[](const std::string &key) const
{
    auto value = m_values.find (key);
    if (value == m_values.end ())
        throw json::key_error (key);

    return *value->second;
}


//-----------------------------------------------------------------------------
bool
json::tree::object::has (const std::string &key) const
{
    return m_values.find (key) != m_values.end ();
}


//-----------------------------------------------------------------------------
json::tree::object::const_iterator
json::tree::object::find (const std::string &key) const
{
    return m_values.find (key);
}


//-----------------------------------------------------------------------------
json::tree::object::const_iterator
json::tree::object::begin (void) const
    { return m_values.begin (); }


//-----------------------------------------------------------------------------
json::tree::object::const_iterator
json::tree::object::end (void) const
    { return m_values.end (); }


//-----------------------------------------------------------------------------
size_t
json::tree::object::size (void) const
{
    return m_values.size ();
}


//-----------------------------------------------------------------------------
void
json::tree::object::clear (void)
{
    m_values.clear ();
}


//-----------------------------------------------------------------------------
void
json::tree::object::erase (const std::string &key)
{
    auto pos = m_values.find (key);
    if (pos == m_values.end ())
        throw json::error ("erasing invalid key");

    m_values.erase (key);
}


//-----------------------------------------------------------------------------
std::ostream&
json::tree::object::write (std::ostream &os) const
{
    os << "{\n";
    {
        util::indenter raii(os);

        for (auto i = m_values.begin (); i != m_values.end ();) {
            os << '"' << i->first << "\" : " << *i->second;

            if (++i != m_values.end ())
                os << ",\n";
        }
    }

    os << "\n}";
    return os;
}


///////////////////////////////////////////////////////////////////////////////
// Array

json::tree::array::~array()
{
    m_values.clear ();
}


//-----------------------------------------------------------------------------
std::unique_ptr<json::tree::node>
json::tree::array::clone (void) const
{
    auto ret = std::make_unique<array> ();

    for (const auto &i: *this)
        ret->insert (i.clone ());

    return std::move (ret);
}


//-----------------------------------------------------------------------------
void
json::tree::array::insert (std::unique_ptr<json::tree::node> &&_value)
{
    m_values.push_back (move (_value));
}


//-----------------------------------------------------------------------------
bool
json::tree::array::operator==(const json::tree::array &rhs) const
{
    for (auto i = rhs.m_values.begin (), j = m_values.begin ();
         i != rhs.m_values.end () && j != m_values.end ();
         ++i, ++j)
    { if ((**i) != (**j)) return false; }

    return true;
}


//-----------------------------------------------------------------------------
bool
json::tree::array::operator== (const node &rhs) const
{
    return rhs == *this;
}


//-----------------------------------------------------------------------------
size_t
json::tree::array::size (void) const
{
    return m_values.size ();
}


//-----------------------------------------------------------------------------
json::tree::node&
json::tree::array::operator[] (unsigned int idx)
{
    return *m_values[idx];
}


//-----------------------------------------------------------------------------
const json::tree::node&
json::tree::array::operator[] (unsigned int idx) const
{
    return *m_values[idx];
}


//-----------------------------------------------------------------------------
json::tree::array::iterator
json::tree::array::begin (void)
{
    return iterator (m_values.begin ());
}



//-----------------------------------------------------------------------------
json::tree::array::iterator
json::tree::array::end (void)
{
    return iterator (m_values.end ());
}



//-----------------------------------------------------------------------------
json::tree::array::const_iterator
json::tree::array::begin (void) const
{
    return const_iterator (m_values.begin ());
}



//-----------------------------------------------------------------------------
json::tree::array::const_iterator
json::tree::array::end (void) const
{
    return const_iterator (m_values.end ());
}



//-----------------------------------------------------------------------------
json::tree::array::const_iterator
json::tree::array::cbegin (void) const
{
    return const_iterator (m_values.cbegin ());
}


//-----------------------------------------------------------------------------
json::tree::array::const_iterator
json::tree::array::cend (void) const
{
    return const_iterator (m_values.cend ());
}


//-----------------------------------------------------------------------------
std::ostream&
json::tree::array::write (std::ostream &os) const {
    os << "[\n";
    {
        util::indenter raii(os);

        for (auto i = m_values.begin (); i != m_values.end (); ++i) {
            (*i)->write (os);

            if (i != m_values.end () - 1)
                os << ",\n";
        }
    }
    os << "\n]";
    return os;
}


///////////////////////////////////////////////////////////////////////////////
// String

std::unique_ptr<json::tree::node>
json::tree::string::clone (void) const
{
    return std::make_unique<json::tree::string> (m_value);
}


//-----------------------------------------------------------------------------
std::ostream&
json::tree::string::write (std::ostream &os) const
{
    os << '"' << m_value << '"';
    return os;
}


//-----------------------------------------------------------------------------
bool
json::tree::string::operator== (const json::tree::string &rhs) const
    { return rhs.m_value == m_value; }


//-----------------------------------------------------------------------------
bool
json::tree::string::operator== (const char *rhs) const
    { return rhs == m_value; }


//-----------------------------------------------------------------------------
bool
json::tree::string::operator== (const std::string &rhs) const
    { return rhs == m_value; }

///////////////////////////////////////////////////////////////////////////////
// Number

std::unique_ptr<json::tree::node>
json::tree::number::clone (void) const
{
    return std::make_unique<json::tree::number> (m_value);
}


//-----------------------------------------------------------------------------
std::ostream&
json::tree::number::write (std::ostream &os) const
{
    os << std::setprecision (std::numeric_limits<double>::digits10) << m_value;
    return os;
}


//-----------------------------------------------------------------------------
bool
json::tree::number::operator ==(const json::tree::number &rhs) const
    { return almost_equal (rhs.m_value, m_value); }


///////////////////////////////////////////////////////////////////////////////
// Boolean

std::unique_ptr<json::tree::node>
json::tree::boolean::clone (void) const
{
    return std::make_unique<json::tree::boolean> (m_value);
}


//-----------------------------------------------------------------------------
std::ostream&
json::tree::boolean::write (std::ostream &os) const
{
    os << (m_value ? "true" : "false");
    return os;
}


//-----------------------------------------------------------------------------
bool
json::tree::boolean::operator ==(const json::tree::boolean &rhs) const
    { return rhs.m_value == m_value; }


///////////////////////////////////////////////////////////////////////////////
// Null

std::unique_ptr<json::tree::node>
json::tree::null::clone (void) const
{
    return std::make_unique<json::tree::null> ();
}


//-----------------------------------------------------------------------------
std::ostream&
json::tree::null::write (std::ostream &os) const
{
    os << "null";
    return os;
}


//-----------------------------------------------------------------------------
std::ostream&
json::tree::operator<< (std::ostream &os, const json::tree::node &n)
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
    io<std::nullptr_t>::serialise (const std::nullptr_t &) {
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
