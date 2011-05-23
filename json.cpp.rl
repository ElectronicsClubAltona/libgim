#include "json.hpp"
#include "maths.hpp"
#include "io.hpp"

#include <cassert>
#include <deque>
#include <stdexcept>
#include <cstdlib>
#include <algorithm>
#include <sstream>

#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


using namespace std;

/*
 * Parsing
 */

struct parse_context {
    parse_context(json::node *_root):
        root  (_root),
        value (NULL),
        key   (NULL),
        start (NULL),
        stop  (NULL)
    { ; }

    json::node *root,
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


    action new_object { nodestack.push_back (parse_context(new json::object));  }
    action new_array  { nodestack.push_back (parse_context(new json::array));  }

    action new_object_value {
        assert (nodestack.back ().root->is_object ());
        assert (nodestack.back ().key);
        assert (nodestack.back ().value);

        if (!nodestack.back ().key->is_string ())
            throw error ("object keys must be strings");

        json::object *object = (json::object *)nodestack.back ().root;
        object->insert (nodestack.back ().key->to_string (),
                        nodestack.back ().value);
        nodestack.back ().key   = NULL;
        nodestack.back ().value = NULL;
    }

    action new_array_value {
        assert (nodestack.back ().root->is_array ());
        assert (nodestack.back ().value);

        json::array *array = (json::array *)nodestack.back ().root;
        array->insert (nodestack.back ().value);
        nodestack.back ().value = NULL;
    }

    action new_string  {
        assert (!nodestack.empty ());
        assert (!nodestack.back ().value);

        std::string value (std::string (nodestack.back ().start,
                                        nodestack.back ().stop));
        nodestack.back ().value = new json::string(value);
    }

    action new_boolean {
        assert (!nodestack.empty ());
        assert (!nodestack.back ().value);

        throw error ("unable to parse boolean");
    }

    action new_number  {
        assert (!nodestack.empty ());
        assert (!nodestack.back ().value);
        
        errno = 0;
        double value = strtod (nodestack.back ().start, NULL);
        if (errno)
            throw error ("unable to parse number");
        nodestack.back ().value = new json::number (value);
    }

    action new_null    {
        assert (!nodestack.empty ());
        assert (!nodestack.back ().value);

        nodestack.back().value = new json::null ();
    }

    action new_object_key {
        assert (!nodestack.empty ());
        assert (nodestack.back ().root->is_object ());
        assert (nodestack.back ().value);
        assert (!nodestack.back ().key);

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
          'true'  @{ nodestack.back ().value = new json::boolean ( true); }
        | 'false' @{ nodestack.back ().value = new json::boolean (false); };

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

    json := (space* object space*)
            $!failure
            %success
            >{ __success = false; };

	write data;
}%%
    

/*
 * Node
 */

namespace json {
    json::node *
    parse (const boost::filesystem::path &path) {
        mapped_file file(path);
        return parse ((const char *)file.data (),
                      (const char *)file.data () + file.size ());
    }


    node *
    parse (const char *start,
           const char *stop) {
        bool __success = true;
        json::node *__root = NULL;
        int cs, top = 0;
        deque <int> fsmstack;
        deque <parse_context> nodestack;

        const char *p   = start,
                   *pe  = stop,
                   *eof = stop;

        %%write init;
        %%write exec;

        if (!__success) 
            throw error ("unable to parse json");

        //__root->print (cout) << endl;
        assert (*__root == *__root);
        return __root;
    }


    node*
    parse (const char *start)
        { return parse (start, start + strlen (start)); }
}


const json::object&
json::node::to_object  (void) const
        { throw error ("node is not an object"); }


const json::array&
json::node::to_array   (void) const
        { throw error ("node is not an array"); }


const json::string&
json::node::to_string  (void) const
        { throw error ("node is not a string"); }


const json::number&
json::node::to_number  (void) const
        { throw error ("node is not a number"); }


const json::boolean&
json::node::to_boolean (void) const
        { throw error ("node is not a boolean"); }


bool
json::node::operator!=(const node &rhs) const
    { return !(*this == rhs); }


/*
 * Object
 */

json::object::~object () {
    for (auto i = m_values.begin (); i != m_values.end (); ++i)
        delete i->second;
}


bool
json::object::operator ==(const json::object &rhs) const {
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
json::object::insert (const std::string _key, json::node* value)
        { m_values[_key] = value; }


const json::node&
json::object::operator[](const std::string &key) const {
    auto value = m_values.find (key);
    if (value == m_values.end ()) {
        ostringstream ss;
        ss << "no key: " << key;
        throw json::error (ss.str());
    }

    return *value->second;
}


std::ostream&
json::object::print (std::ostream &os) const {
    os << "{";

    for (auto i = m_values.begin (); i != m_values.end ();) {
        os << '"' << i->first << "\" : " << *i->second;

        if (++i != m_values.end ())
            os << ",\n";
    }

    os << "}";
    return os;
}


/*
 * Array
 */

json::array::~array() {
    for (auto i = m_values.begin(); i != m_values.end (); ++i)
        delete *i;
}


void
json::array::insert (json::node *_value)
        { m_values.push_back (_value); }


bool
json::array::operator ==(const json::array &rhs) const {
    for (auto i = rhs.m_values.begin (), j = m_values.begin ();
         i != rhs.m_values.end () && j != m_values.end ();
         ++i, ++j)
    { if ((**i) != (**j)) return false; }

    return true;
}


std::ostream&
json::array::print (std::ostream &os) const {
    os << "[ ";

    for (auto i = m_values.begin (); i != m_values.end (); ++i) {
        os << (*i)->print (os);

        if (i != m_values.end () - 1)
            os << ", ";
    }

    os << "]";
    return os;
}


/*
 * String
 */

std::ostream&
json::string::print (std::ostream &os) const {
    os << '"' << m_value << '"';
    return os;
}


bool
json::string::operator ==(const json::string &rhs) const
    { return rhs.m_value == m_value; }


/*
 * Number
 */

std::ostream&
json::number::print (std::ostream &os) const {
    os << '"' << m_value << '"';
    return os;
}


bool
json::number::operator ==(const json::number &rhs) const 
    { return almost_equal (rhs.m_value, m_value); }


/*
 * Boolean
 */

std::ostream&
json::boolean::print (std::ostream &os) const {
    os << '"' << (m_value ? "true" : "false") << '"';
    return os;
}

bool
json::boolean::operator ==(const json::boolean &rhs) const 
    { return rhs.m_value == m_value; }


/*
 * Null
 */ 

std::ostream&
json::null::print (std::ostream &os) const {
    os << "null";
    return os;
}

ostream&
operator <<(ostream &os, const json::node &n)
    { return n.print (os); }
