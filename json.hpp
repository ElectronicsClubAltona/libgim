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
 * Copyright 2010-2012 Danny Robson <danny@blubinc.net>
 */

#ifndef __UTIL_JSON_HPP
#define __UTIL_JSON_HPP

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>


namespace json {
    class node;
    class object;
    class array;
    class string;
    class number;
    class boolean;
    class null;

    extern std::unique_ptr<node> parse (const boost::filesystem::path &path);
    extern std::unique_ptr<node> parse (const char *start, const char *stop);
    extern std::unique_ptr<node> parse (const char *start);
    extern std::unique_ptr<node> parse (const std::string&);

    extern void write (const json::node&, std::ostream&);

    /// Abstract base for all JSON values
    class node {
        public:
            virtual ~node () { ; }

            virtual const object&  as_object  (void) const;
            virtual const array&   as_array   (void) const;
            virtual const string&  as_string  (void) const;
            virtual const number&  as_number  (void) const;
            virtual const boolean& as_boolean (void) const;

            virtual bool is_object  (void) const { return false; }
            virtual bool is_array   (void) const { return false; }
            virtual bool is_string  (void) const { return false; }
            virtual bool is_number  (void) const { return false; }
            virtual bool is_boolean (void) const { return false; } 
            virtual bool is_null    (void) const { return false; }

            virtual bool operator==(const node &rhs) const = 0;
            virtual bool operator!=(const node &rhs) const;
            virtual bool operator==(const object  &) const { return false; }
            virtual bool operator==(const array   &) const { return false; }
            virtual bool operator==(const string  &) const { return false; }
            virtual bool operator==(const number  &) const { return false; }
            virtual bool operator==(const boolean &) const { return false; }
            virtual bool operator==(const null    &) const { return false; }

            virtual bool operator==(const char *rhs) const;
            virtual bool operator!=(const char *rhs) const { return !(*this == rhs); }

            virtual const node& operator[] (const std::string&) const;
            virtual const node& operator[] (unsigned int) const;

            virtual std::ostream& write (std::ostream &os) const = 0;
    };                                          


    /// Represents a JSON object, and contains its children.
    class object : public node {
        protected:
            std::map<std::string, std::unique_ptr<node>> m_values;

        public:
            object () { ; }
            virtual ~object ();

            virtual const object& as_object  (void) const { return *this; }
            virtual bool          is_object  (void) const { return  true; }
            virtual bool operator==(const object &rhs) const;
            virtual bool operator==(const node   &rhs) const
                { return rhs == *this; }

            virtual void insert (const std::string &_key, std::unique_ptr<node>&& value);
            virtual const node& operator[](const std::string &key) const;

            virtual std::ostream& write (std::ostream &os) const;
    };


    /// Represents a JSON array, and contains its children.
    class array : public node {
        public:
            typedef std::vector<std::unique_ptr<node>>::iterator       array_iterator;
            typedef std::vector<std::unique_ptr<node>>::const_iterator const_array_iterator;

        protected:
            std::vector<std::unique_ptr<node>> m_values;

        public:
            virtual ~array();

            virtual const array&  as_array   (void) const { return *this; }
            virtual bool          is_array   (void) const { return  true; }
            virtual bool operator==(const array &rhs) const;
            virtual bool operator==(const node   &rhs) const
                { return rhs == *this; }

            virtual size_t size (void) const
                { return m_values.size (); } 
            virtual node& operator [](unsigned int idx)
                { return *m_values[idx]; }
            virtual const node& operator [](unsigned int idx) const
                { return *m_values[idx]; }

            virtual const_array_iterator begin (void) const   { return m_values.begin (); }
            virtual const_array_iterator end   (void) const   { return m_values.end (); }

            virtual void insert (std::unique_ptr<json::node> &&_value);

            virtual std::ostream& write (std::ostream &os) const;
    };


    /// Represents a JSON string literal.
    class string : public node {
        protected:
            std::string m_value;

        public:
            string (const std::string &_value): m_value (_value) { ; }
            string (const char        *_value): m_value (_value) { ; }

            virtual const string& as_string  (void) const { return *this; }
            virtual bool          is_string  (void) const { return  true; }
            virtual bool operator==(const string &rhs) const;
            virtual bool operator==(const node   &rhs) const
                { return rhs == *this; }

            operator const std::string&(void) const { return m_value; }
            const std::string& native (void) const  { return m_value; }

            virtual std::ostream& write (std::ostream &os) const;
    };


    /// Represents a JSON integer/float literal.
    class number : public node {
        protected:
            double m_value;

        public:
            number (double _value): m_value (_value) { ; }
            number (int _value):    m_value (_value) { ; }

            virtual const number& as_number  (void) const { return *this; }
            virtual bool          is_number  (void) const { return  true; }
            virtual bool operator==(const number &rhs) const;
            virtual bool operator==(const node   &rhs) const
                { return rhs == *this; }

            operator double(void) const { return m_value; }
            double native (void) const  { return m_value; }

            virtual std::ostream& write (std::ostream &os) const;
    };


    /// Represents a JSON boolean literal.
    class boolean : public node {
        protected:
            bool m_value;

        public:
            boolean (bool _value): m_value (_value) { ; }

            virtual const boolean& as_boolean (void) const { return *this; }
            virtual bool           is_boolean (void) const { return  true; }
            virtual bool operator==(const boolean &rhs) const;
            virtual bool operator==(const node   &rhs) const
                { return rhs == *this; }

            operator bool (void) const { return m_value; }
            bool native (void) const { return m_value; }

            virtual std::ostream& write (std::ostream &os) const;
    };


    /// Represents a JSON null value.
    class null : public node {
        public:
            virtual bool is_null (void) const { return  true; }
            virtual bool operator==(const null&) const { return true; }
            virtual bool operator==(const node   &rhs) const
                { return rhs == *this; }

            virtual std::ostream& write (std::ostream &os) const;
    };


    /// The base class for all exceptions throw directly by the json namespace.
    class error : public std::runtime_error {
        public:
            error (const std::string &_what):
                std::runtime_error (_what)
            { ; }
    };

    /// Base class for errors thrown during parsing
    class parse_error : public error {
        public:
            parse_error (const std::string &_what):
                error (_what)
            { ; }
    };

    /// Base class for errors thrown during schema validation
    class schema_error : public error {
        public:
            schema_error (const std::string &_what):
                error (_what)
            { ; }
    };
}


std::ostream&
operator <<(std::ostream &os, const json::node &n);

#endif

