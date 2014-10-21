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
 * Copyright 2013-2014 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_OPTIONS_HPP
#define __UTIL_OPTIONS_HPP

#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>


/**
 * Maximum length of an option, its value, and any supplementary characters
 * required for interpreting them. Ie, strlen("--foo=bar")
 */
const size_t MAX_CHUNK_LENGTH = 1023;

namespace util {
    /**
     * The generic base class for all options.
     *
     * Stores enough information to determine whether a short or long style option
     * has been specified. The `execute' methods performs an action if the option
     * is either present, or present with a value.
     *
     * By default they will throw an exception and can be relied upon to provide
     * some form of abort action.
     */
    class option {
            protected:
                    char              m_shortopt;
                    const std::string m_longopt;
                    const std::string m_description;
                    bool              m_required;
                    bool              m_found;

            public:
                    option (char        _letter,
                            const char *_name,
                            const char *_desc,
                            bool        _required);

                    virtual ~option() { ; }

                    virtual void execute (void) = 0;
                    virtual void execute (const std::string& _data) = 0;
                    virtual void finish  (void);

                    virtual bool is_required (void) const
                        { return m_required; }

                    virtual void reset(void)
                        { m_found = false; }

                    virtual char shortopt(void) const
                        { return m_shortopt; }
                    virtual const std::string& longopt(void) const
                        { return m_longopt; }
                    virtual const std::string& description(void) const
                        { return m_description; }
    };


    /**
     * Specifies a debugging or deprecated option.
     *
     * The option will be recognised but no action will be performed when
     * encountered. A description will still be visible when usage information is
     * printed.
     */
    class nulloption : public option {
        public:
            nulloption(char        _letter,
                    const char *_name,
                    const char *_desc,
                    bool        _required = false);

            virtual void execute(void)
                { m_found = true; }
            virtual void execute(const std::string&)
                { m_found = true; }
    };


    /**
     * Sets a boolean value to reflect if the option has been specified.
     *
     * A provided boolean pointer will be set to false if the option isn't
     * present, true if present.
     * Assumes that the user will not modify the value for the duration of option
     * processing.
     * Throws an exception if a value is specified for the option.
     */
    class presentoption : public option {
        protected:
            bool *m_data;

        public:
            presentoption(char        _letter,
                          const char *_name,
                          const char *_desc,
                          bool       *_data,
                          bool        _required = false);

            virtual void execute(void);
            virtual void execute(const std::string& data)
                { option::execute(data); }

            virtual void reset(void)
                { *m_data = false; }
    };


    /**
     * Parses a value given on the command line into a specified type.
     *
     * Given a value, return it to the user via the specified pointer. Uses the
     * istream operators to read the value, so it should be relatively generic.
     * If the value is malformed then an exception will be thrown.
     * If there is no value, then perform the default action.
     *
     * It is safe to assume that when no value has been extracted the data pointer
     * provided will not be overwritten. Useful for pre-loading with default
     * values.
     */
    template<typename T>
    class valueoption : public option {
        protected:
            T *m_data;

        public:
            valueoption(char        _letter,
                        const char *_name,
                        const char *_desc,
                        T          *_data,
                        bool        _required = false):
                option (_letter, _name, _desc, _required),
                m_data (_data)
            { ; }

            virtual void execute(void) 
                { option::execute(); }
            virtual void execute(const std::string& data) {
                get_arg(data, m_data);
                m_found = true;
            }

        protected:
            // Retrieve string to value conversion
            static
            T& get_arg(const std::string &arg,
                       T                 *val);

            static
            T& get_arg(const std::string &_arg,
                       T                 *val,
                       const T           &defaultval) {
                try {
                    return get_arg(_arg, val);
                } catch(...) {
                    return *val = defaultval;
                }
            }

    };


    /**
     * Interpret a (possibly) numeric value as a data size.
     *
     * A developer specifies the default scale of the value and what base it is
     * to be interpreted in - when not otherwise explicitly stated in the
     * arguments.
     *
     * Recognises various postfixes and modifiers to a numeric value such as MiB.
     */
    class bytesoption : public valueoption<size_t> {
        public:
            /* Description of types available for parsing
            */
            enum bytestype {
                BYTES_SINGLE = 0,
                BYTES_KILO   = 1,
                BYTES_MEGA   = 2,
                BYTES_GIGA   = 3,
                BYTES_TERA   = 4,
                BYTES_PETA   = 5,
                BYTES_EXA    = 6,

                // Currently does not support yota or zeta as there can be
                // trouble converting them without loss into 64bit quantities.
                // That and they're fricking huge...
                //
                // BYTES_ZETA,
                // BYTES_YOTA,

                NUM_BYTESTYPE
            };

            enum bytesmodifier {
                BYTES_BASE2,
                BYTES_BASE10,

                NUM_BYTESMODIFIER
            };

        protected:
            bytestype m_type;
            bytesmodifier m_modifier;

            static bytestype type_from_character(char c);


        public:
            /* Constructors and methods
            */
            bytesoption(char           _letter,
                        const char    *_name,
                        const char    *_desc,
                        size_t        *_data,
                        bytestype      _type     = BYTES_SINGLE,
                        bytesmodifier  _modifier = BYTES_BASE2,
                        bool           _required = false);

            virtual void execute(const std::string &);
    };


    /**
     * High level handler for option registration, dispatch, and interpretation.
     *
     * The processor is the central point of operation on a program's arguments.
     * It contains a list of all options which will interpret and act upon any
     * arguments. These must be registered by the user prior to argument parsing,
     * each of which must be primed with data to act upon separately.
     *
     * parse_args will perform minimal interpretation of the arguments to allow
     * dispatch to registered options. No direct actions upon any input is
     * performed within this class, merely dispatch and tokenisation. 
     */
    class processor {
        protected:
            std::map<char, option *> m_shortopt;
            std::map<std::string, option *> m_longopt;

            std::list<std::unique_ptr<option>> m_options;

            // The command to execute the application
            std::string m_command;

        protected:
            unsigned int parse_short(int pos, int argc, const char ** argv);
            unsigned int parse_long(int pos, int argc, const char ** argv);

        public:
            processor();
            ~processor();

            void print_usage(void);

            // TODO: Use function overloading here...
            void parse_args(int argc, const char ** argv);
            void parse_args(int argc, char ** argv)
                { parse_args(argc, const_cast<const char**>(argv)); }
            void parse_stream(std::istream & is);


            void add_option(std::unique_ptr<option>);

            std::unique_ptr<option> remove_option(char letter);
            std::unique_ptr<option> remove_option(const char * name);
            std::unique_ptr<option> remove_option(const std::string& name)
                { return remove_option (name.c_str()); }
            std::unique_ptr<option> remove_option(const option *opt)
                { return remove_option (opt->longopt ()); }
    };
}

#endif
