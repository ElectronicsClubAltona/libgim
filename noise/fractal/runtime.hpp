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


#ifndef __UTIL_NOISE_FRACTAL_RUNTIME_HPP
#define __UTIL_NOISE_FRACTAL_RUNTIME_HPP

#include "base.hpp"
#include "../../point.hpp"
#include "../../range.hpp"

#include <memory>

namespace util { namespace noise { namespace fractal {
    template <
        size_t S,   // probe point dimensionality
        typename T, // probe point value_type
        typename B  // generating basis function
    >
    struct runtime {
    public:
        using seed_t = uint64_t;

        runtime (seed_t) { }
        runtime () = default;
        runtime (runtime&&) = default;
        runtime (const runtime&) = delete;
        runtime& operator= (const runtime&) = delete;

        // basis functions
        T operator () (point<S,T> p) const { return (*m_child) (p); }

        unsigned octaves (void) const { return m_child->octaves (); }
        unsigned octaves (unsigned _octaves) { return m_child->octaves (_octaves); }

        T H (void) const { return m_child->H (); }
        T H (T _H) { return m_child->H (_H); }
    
        T frequency (void) const { return m_child->frequency (); }
        T frequency (T _frequency) { return m_child->frequency (_frequency); }

        T lacunarity (void) const { return m_child->lacunarity (); }
        T lacunarity (T _lacunarity) { return m_child->lacunarity (_lacunarity); }

        T amplitude (void) const { return m_child->amplitude (); }
        T amplitude (T _amplitude) { return m_child->amplitude (_amplitude); }

        T gain (void) const { return m_child->gain (); }
        T gain (T _gain) { return m_child->gain (_gain); }

        B& basis (void) { return m_child->basis (); }
        const B& basis (void) const { return m_child->basis (); }

        seed_t seed (void) const { return m_child->seed (); }
        seed_t seed (seed_t) { return m_child->seed (); }

    private:
        struct base {
            virtual ~base () = default;

            virtual T operator() (util::point<S,T>) = 0;

            virtual unsigned octaves (void) const = 0;
            virtual unsigned octaves (unsigned) = 0;

            virtual T H (void) const = 0;
            virtual T H (T) = 0;
        
            virtual T frequency (void) const = 0;
            virtual T frequency (T) = 0;

            virtual T lacunarity (void) const = 0;
            virtual T lacunarity (T) = 0;

            virtual T amplitude (void) const = 0;
            virtual T amplitude (T) = 0;

            virtual T gain (void) const = 0;
            virtual T gain (T) = 0;

            virtual B& basis (void) = 0;
            virtual const B& basis (void) const = 0;

            virtual seed_t seed (void) const = 0;
            virtual seed_t seed (seed_t) = 0;
        };

        template <typename F>
        struct child final : public base {
            child (seed_t _seed):
                data (_seed)
            { ; }

           child (seed_t _seed,
                  unsigned _octaves,
                  T _H,
                  T _frequency,
                  T _lacunarity,
                  T _amplitude,
                  T _gain):
               data (_seed,
                     _octaves,
                     _H,
                     _frequency,
                     _lacunarity,
                     _amplitude,
                     _gain)
            { ; }
 
            T operator() (util::point<S,T> p) override { return data (p); }

            unsigned octaves (void) const override { return data.octaves (); }
            unsigned octaves (unsigned _octaves) override { return data.octaves (_octaves); }

            T H (void) const override { return data.H (); }
            T H (T _H) override { return data.H (_H); }
        
            T frequency (void) const override { return data.frequency (); }
            T frequency (T _frequency) override { return data.frequency (_frequency); }

            T lacunarity (void) const override { return data.lacunarity (); }
            T lacunarity (T _lacunarity) override { return data.lacunarity (_lacunarity); }

            T amplitude (void) const override { return data.amplitude (); }
            T amplitude (T _amplitude) override { return data.amplitude (_amplitude); }

            T gain (void) const override { return data.gain (); }
            T gain (T _gain) override { return data.gain (_gain); }

            B& basis (void) override { return data.basis (); }
            const B& basis (void) const override { return data.basis (); }

            seed_t seed (void) const override { return data.seed (); }
            seed_t seed (seed_t _seed) override { return data.seed (_seed); }

            F data;
        };

        std::unique_ptr<base> m_child;

    public:
        template <typename F>
        F&
        reset (seed_t _seed)
        {
            using fractal_t = F;
            using child_t = child<fractal_t>;

            child_t *out;
            m_child.reset (out= new child_t (_seed));
            return out->data;
        }
    };
} } }

#endif
