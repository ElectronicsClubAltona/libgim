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

#ifndef __UTIL_JSON_TREE_HPP
#define __UTIL_JSON_TREE_HPP

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include "../iterator.hpp"
#include "./flat.hpp"
#include "./fwd.hpp"

namespace json { namespace tree {
    enum type_t {
        OBJECT,
        ARRAY,
        STRING,
        NUMBER,
        BOOLEAN,
        NONE
    };

    /// Parse an encoded form of JSON into a tree structure
    extern std::unique_ptr<node> parse (const boost::filesystem::path &path);
    extern std::unique_ptr<node> parse (const char *start, const char *stop);
    extern std::unique_ptr<node> parse (const char *start);
    extern std::unique_ptr<node> parse (const std::string&);

    extern void write (const json::tree::node&, std::ostream&);

    /// Abstract base for all JSON values
    class node {
        public:
            virtual ~node () { ; }
            virtual std::unique_ptr<node> clone (void) const = 0;

            virtual const object&  as_object  (void) const;
            virtual const array&   as_array   (void) const;
            virtual const string&  as_string  (void) const;
            virtual const number&  as_number  (void) const;
            virtual const boolean& as_boolean (void) const;
            virtual const null&    as_null    (void) const;

            virtual object&  as_object  (void);
            virtual array&   as_array   (void);
            virtual string&  as_string  (void);
            virtual number&  as_number  (void);
            virtual boolean& as_boolean (void);
            virtual null&    as_null    (void);

            // we don't provide operators for conversion due to ambiguities
            // introduced when using indexing operators with pointer
            // arguments.
            virtual bool   as_bool   (void) const;
            virtual float  as_float  (void) const;
            virtual double as_double (void) const;
            virtual size_t as_uint   (void) const;
            virtual const char* as_chars (void) const;

            virtual bool is_object  (void) const { return false; }
            virtual bool is_array   (void) const { return false; }
            virtual bool is_string  (void) const { return false; }
            virtual bool is_number  (void) const { return false; }
            virtual bool is_boolean (void) const { return false; }
            virtual bool is_null    (void) const { return false; }

            virtual type_t type (void) const = 0;

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

            virtual node& operator[] (const std::string&);
            virtual node& operator[] (unsigned int);
            virtual const node& operator[] (const std::string&) const;
            virtual const node& operator[] (unsigned int) const;

            virtual std::ostream& write (std::ostream &os) const = 0;
    };


    /// Represents a JSON object, and contains its children.
    class object final : public node {
        protected:
            typedef std::map<std::string, std::unique_ptr<node>> value_store;
        public:
            typedef value_store::iterator       iterator;
            typedef value_store::const_iterator const_iterator;

        protected:
            value_store m_values;

        public:
            virtual ~object ();
            virtual std::unique_ptr<node> clone (void) const override;

            virtual const object& as_object  (void) const override { return *this; }
            virtual       object& as_object  (void)       override { return *this; }
            virtual bool          is_object  (void) const override { return  true; }
            virtual type_t type (void) const override { return OBJECT; }

            virtual bool operator==(const object &rhs) const override;
            virtual bool operator==(const node   &rhs) const override
                { return rhs == *this; }

            virtual void insert (const std::string &key, std::unique_ptr<node>&& value);
            virtual const node& operator[](const std::string &key) const override;
            virtual node& operator[](const std::string &key) override;
            virtual bool has (const std::string&) const;

            virtual const_iterator find (const std::string&) const;

            virtual const_iterator begin (void) const;
            virtual const_iterator end   (void) const;
            virtual const_iterator cbegin (void) const { return begin (); }
            virtual const_iterator cend   (void) const { return end   (); }

            virtual size_t size (void) const;

            virtual void clear (void);
            virtual void erase (const std::string &key);

            virtual std::ostream& write (std::ostream &os) const override;
    };


    /// Represents a JSON array, and contains its children.
    class array final : public node {
        protected:
            typedef std::vector<std::unique_ptr<node>>::iterator       pointer_array_iterator;
            typedef std::vector<std::unique_ptr<node>>::const_iterator const_pointer_array_iterator;

        public:
            typedef referencing_iterator<pointer_array_iterator>       iterator;
            typedef referencing_iterator<const_pointer_array_iterator> const_iterator;

        protected:
            std::vector<std::unique_ptr<node>> m_values;

        public:
            virtual ~array();
            virtual std::unique_ptr<node> clone (void) const override;

            virtual const array&  as_array   (void) const override { return *this; }
            virtual       array&  as_array   (void)       override { return *this; }
            virtual bool          is_array   (void) const override { return  true; }
            virtual type_t type (void) const override { return ARRAY; }

            virtual bool operator==(const array &rhs) const override;
            virtual bool operator==(const node  &rhs) const override;

            virtual size_t size (void) const;
            virtual node& operator [](unsigned int idx) override;
            virtual const node& operator [](unsigned int idx) const override;

            virtual iterator begin (void);
            virtual iterator end   (void);
            virtual const_iterator begin (void) const;
            virtual const_iterator end   (void) const;
            virtual const_iterator cbegin (void) const;
            virtual const_iterator cend   (void) const;

            virtual void insert (std::unique_ptr<json::tree::node> &&_value);

            virtual std::ostream& write (std::ostream &os) const override;
    };


    /// Represents a JSON string literal.
    class string final : public node {
        protected:
            std::string m_value;

        public:
            explicit string (const std::string &_value): m_value (_value) { ; }
            explicit string (const char        *_value): m_value (_value) { ; }
            string (const char *_first, const char *_last): m_value (_first, _last) { ; }
            virtual std::unique_ptr<node> clone (void) const override;

            virtual const string& as_string  (void) const override { return *this; }
            virtual       string& as_string  (void)       override { return *this; }
            virtual bool          is_string  (void) const override { return  true; }

            virtual type_t type (void) const override { return STRING; }

            virtual bool operator==(const char   *rhs) const override;
            virtual bool operator==(const string &rhs) const override;
            virtual bool operator==(const std::string &rhs) const;
            virtual bool operator==(const node   &rhs) const override
                { return rhs == *this; }

            virtual bool operator!= (const std::string &rhs) const { return !(*this == rhs); }

            virtual size_t size (void) const { return m_value.size (); }

            operator const std::string&(void) const { return m_value; }
            const std::string& native (void) const  { return m_value; }

            virtual std::ostream& write (std::ostream &os) const override;
    };


    /// Represents a JSON integer/float literal.
    class number final : public node {
        protected:
            double m_value;

        public:
            explicit number (double _value): m_value (_value) { ; }
            explicit number (int    _value): m_value (_value) { ; }
            explicit number (size_t _value): m_value (_value) { ; }
            virtual std::unique_ptr<node> clone (void) const override;

            virtual const number& as_number  (void) const override { return *this; }
            virtual       number& as_number  (void)       override { return *this; }
            virtual bool          is_number  (void) const override { return  true; }

            virtual type_t type (void) const override { return NUMBER; }

            virtual bool operator==(const number &rhs) const override;
            virtual bool operator==(const node   &rhs) const override
                { return rhs == *this; }

            operator double(void) const { return m_value; }
            double native (void) const  { return m_value; }

            virtual std::ostream& write (std::ostream &os) const override;
    };


    /// Represents a JSON boolean literal.
    class boolean final : public node {
        protected:
            bool m_value;

        public:
            explicit boolean (bool _value): m_value (_value) { ; }
            virtual std::unique_ptr<node> clone (void) const override;

            virtual const boolean& as_boolean (void) const override { return *this; }
            virtual       boolean& as_boolean (void)       override { return *this; }
            virtual bool           is_boolean (void) const override { return  true; }

            virtual type_t type (void) const override { return BOOLEAN; }

            virtual bool operator==(const boolean &rhs) const override;
            virtual bool operator==(const node   &rhs) const override
                { return rhs == *this; }

            operator bool (void) const { return m_value; }
            bool native (void) const { return m_value; }

            virtual std::ostream& write (std::ostream &os) const override;
    };


    /// Represents a JSON null value.
    class null final : public node {
        public:
            virtual type_t type (void) const override { return NONE; }
            virtual std::unique_ptr<node> clone (void) const override;

            virtual bool operator==(const null&) const override { return true; }
            virtual bool operator==(const node   &rhs) const override
                { return rhs == *this; }

            virtual bool        is_null (void) const override { return  true; }
            virtual const null& as_null (void) const override { return *this; }
            virtual       null& as_null (void)       override { return *this; }

            virtual std::ostream& write (std::ostream &os) const override;
    };


    std::ostream&
    operator <<(std::ostream &os, const json::tree::node &n);


    // Instantiate this template for the type you wish to output. We use a
    // helper class here to avoid partial template specialisation of a
    // function (eg, for templated types).
    template <typename T>
    struct io {
        static std::unique_ptr<json::tree::node> serialise (const T&);
        static T deserialise (const json::tree::node&);
    };
} }

template <typename T, class ...Args>
std::unique_ptr<json::tree::node> to_json (const T &t, Args&&... args) {
    return json::tree::io<T>::serialise (t, std::forward<Args>(args)...);
}

template <typename T, class ...Args>
T from_json (const json::tree::node &n, Args&&... args) {
    return json::tree::io<T>::deserialise (n, std::forward<Args>(args)...);
}

#endif


