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


#ifndef __UTIL_CMDLINE_HPP
#define __UTIL_CMDLINE_HPP

#include <functional>
#include <memory>
#include <exception>
#include <string>
#include <tuple>
#include <vector>

namespace util { namespace cmdopt {
    namespace option {
        class base {
        public:
            // we deal almost exclusively with vtables, so disable copying
            // just in case we do something stupid.
            base () { }
            base (const base&) = delete;
            base& operator= (const base&) = delete;

            virtual ~base ();

            virtual void execute (void);
            virtual void execute (const char *restrict);
            virtual void start   (void);
            virtual void finish  (void);

            virtual const std::string& example (void) const = 0;

            bool required (void) const;
            bool required (bool);

            bool seen (void) const;
            bool seen (bool);

        private:
            bool m_required = false;
            bool m_seen = false;
        };


        class null : public base {
        public:
            virtual void execute (void) override;
            virtual void execute (const char *restrict) override;

            virtual const std::string& example (void) const override;
        };


        class present : public base {
        public:
            explicit present (bool&);
            explicit present (bool&&) = delete;

            using base::execute;
            virtual void execute (void) override;

            virtual const std::string& example (void) const override;

            virtual void finish (void) override;

        private:
            bool &m_data;
        };


        template <typename T>
        class value : public base {
        public:
            explicit value (T&);
            explicit value (T&&) = delete;

            using base::execute;
            void execute (const char *restrict) override;

            const std::string& example (void) const override;

            const T& data (void) const&;
            T& data (void) &;
            T& data (T) &;

        private:
            T& m_data;
        };


        template <typename T = unsigned>
        class count : public value<T> {
        public:
            explicit count (T&);
            explicit count (T&&) = delete;

            using value<T>::execute;
            void execute (void) override;
        };


        class bytes : public value<size_t> {
        public:
            bytes (size_t &_value): value (_value) { }

            using value<size_t>::execute;
            void execute (const char *restrict) override;
        };
    }


    class parser {
    public:
        template <typename T, typename ...Args>
        T& add (char shortname,
                std::string longname,
                std::string description,
                Args&&...);

        template <typename T, typename ...Args>
        T&
        append (std::string description, Args&&...);

        int scan (int argc, const char *const *argv);

    private:
        int parse_long  (int pos, int argc, const char *const *argv);
        int parse_short (int pos, int argc, const char *const *argv);

        void print_help [[noreturn]] (int argc, const char *const *argv) const;

        using short_t = std::tuple<char,option::base&>;
        using long_t  = std::tuple<std::string,option::base&>;

        std::vector<short_t> m_short;
        std::vector<long_t>  m_long;
        std::vector<std::reference_wrapper<option::base>> m_positional;

        std::vector<
            std::tuple<
                std::string, // description
                std::unique_ptr<option::base>
            >
        > m_options;
    };


    class error : public std::exception { };

    class invalid_key : public error {
    public:
        invalid_key (std::string _key);
        virtual const char* what (void) const noexcept override;

    private:
        const std::string m_key;
    };

    class invalid_value : public error {
    public:
        invalid_value (std::string _value);
        virtual const char* what (void) const noexcept override;

    private:
        const std::string m_value;
    };

    class invalid_null : public error {
    public:
        virtual const char* what (void) const noexcept override;
    };

    class invalid_required : public error {
    public:
        virtual const char* what (void) const noexcept override;
    };

    class unhandled_argument : public error {
    public:
        unhandled_argument (int index);
        virtual const char* what (void) const noexcept override;

        int index (void) const noexcept;

    private:
        const int m_index;
    };
} }

#include "cmdopt.ipp"

#endif
