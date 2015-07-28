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
 * Copyright 2012-2015 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_NOISE_BASIS_RUNTIME_HPP
#define __UTIL_NOISE_BASIS_RUNTIME_HPP

#include "../basis.hpp"
#include "../../point.hpp"
#include "../../range.hpp"

#include <memory>

namespace util { namespace noise { namespace basis {
    template <typename T>
    struct runtime {
    public:
        runtime (seed_t) {}
        runtime (runtime&&) = default;
        runtime (const runtime&) = delete;
        runtime& operator= (const runtime&) = delete;

        // basis functions
        T operator () (util::point<2,T> p) const { return (*m_child) (p); }
        range<T> bounds (void) const { return m_child->bounds (); }

        seed_t seed (void) const { return m_child->seed (); }
        seed_t seed (seed_t) { return m_child->seed (); }

    private:
        struct base {
            virtual ~base () = default;
            virtual T operator() (util::point<2,T>) const = 0;
            virtual range<T> bounds (void) const = 0;
            virtual seed_t seed (void) const = 0;
            virtual seed_t seed (seed_t) = 0;
        };

        template <typename B>
        struct child : public base {
            template <typename ...Args>
            child (seed_t _seed, Args&& ...args): data (_seed, std::forward<Args> (args)...) { }
            virtual T operator() (util::point<2,T> p) const override { return data (p); }
            virtual range<T> bounds (void) const override { return data.bounds (); }
            virtual seed_t seed (void) const override { return data.seed (); }
            virtual seed_t seed (seed_t _seed) override { return data.seed (_seed); }

        private:
            B data;
        };

        std::unique_ptr<base> m_child;

    public:
        template <typename B, typename ...Args>
        void
        reset (seed_t _seed, Args&& ...args)
        {
            using basis_t = B;
            using child_t = child<basis_t>;

            m_child.reset (new child_t (_seed, std::forward<Args> (args)...));
        }
    };
} } }

#endif

