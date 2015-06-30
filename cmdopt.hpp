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


#ifndef __UTIL_CMDLINE_HPP
#define __UTIL_CMDLINE_HPP

#include <memory>
#include <tuple>
#include <vector>
#include <stdexcept>
#include <string>

namespace util { namespace cmdopt {
    namespace option {
        class base {
        public:
            base (std::string name);
            virtual ~base ();

            virtual void execute (void);
            virtual void execute (const char *restrict);
            virtual void start   (void);
            virtual void finish  (void);

            std::string name (void) const;

            bool required (void) const;
            bool required (bool);

            bool seen (void) const;
            bool seen (bool);

        private:
            std::string m_name;
            bool m_required;
            bool m_seen;
        };


        class null : public base {
        public:
            null (std::string name);

            virtual void execute (void) override;
            virtual void execute (const char *restrict) override;
        };


        template <typename T>
        class value : public base {
        public:
            value (std::string name, T&);

            using base::execute;
            void execute (const char *restrict) override;

            T  data (void) const;
            T& data (void);

        private:
            T& m_data;
        };


        template <typename T = unsigned>
        class count : public value<T> {
        public:
            count (std::string name, T&);

            using value<T>::execute;
            void execute (void) override;
        };


        class bytes : public value<size_t> {
        public:
            using value<size_t>::value;

            using value<size_t>::execute;
            void execute (const char *restrict) override;
        };
    }


    class error : public std::runtime_error
    { using runtime_error::runtime_error; };

    class invalid_key : public error
    { using error::error; };

    class invalid_value : public error
    { using error::error; };

    class invalid_null : public error
    { using error::error; };

    class invalid_required : public error
    { using error::error; };


    class parser {
    public:
        template <typename T, typename ...Args>
        T& add (char shortname,
                std::string longname,
                std::string description,
                Args&&...);

        int scan (int argc, char *const *argv);

    private:
        int parse_long  (int pos, int argc, char *const *argv);
        int parse_short (int pos, int argc, char *const *argv);


        using short_t = std::tuple<char,option::base&>;
        using long_t  = std::tuple<std::string,option::base&>;

        std::vector<short_t> m_short;
        std::vector<long_t>  m_long;

        std::vector<std::unique_ptr<option::base>> m_options;
    };
} }

#include "cmdopt.ipp"

#endif
