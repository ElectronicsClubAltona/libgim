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

#include <memory>

namespace util { namespace noise { namespace fractal {
    template <class B>
    struct runtime {
    public:
        using value_t = typename B::value_t;
        using point_t = typename B::point_t;

        runtime (seed_t) { }
        runtime () = default;
        runtime (runtime&&) = default;
        runtime (const runtime&) = delete;
        runtime& operator= (const runtime&) = delete;

        // basis functions
        value_t operator () (point_t p) const noexcept { return (*m_child) (p); }

        unsigned octaves (void) const { return m_child->octaves (); }
        unsigned octaves (unsigned _octaves) { return m_child->octaves (_octaves); }

        value_t H (void) const { return m_child->H (); }
        value_t H (value_t _H) { return m_child->H (_H); }
    
        value_t frequency (void) const { return m_child->frequency (); }
        value_t frequency (value_t _frequency) { return m_child->frequency (_frequency); }

        value_t lacunarity (void) const { return m_child->lacunarity (); }
        value_t lacunarity (value_t _lacunarity) { return m_child->lacunarity (_lacunarity); }

        value_t amplitude (void) const { return m_child->amplitude (); }
        value_t amplitude (value_t _amplitude) { return m_child->amplitude (_amplitude); }

        value_t gain (void) const { return m_child->gain (); }
        value_t gain (value_t _gain) { return m_child->gain (_gain); }

        B& basis (void) { return m_child->basis (); }
        const B& basis (void) const { return m_child->basis (); }

        seed_t seed (void) const { return m_child->seed (); }
        seed_t seed (seed_t) { return m_child->seed (); }

    private:
        struct base {
            virtual ~base () = default;

            virtual value_t operator() (point_t) const noexcept = 0;

            virtual unsigned octaves (void) const = 0;
            virtual unsigned octaves (unsigned) = 0;

            virtual value_t H (void) const = 0;
            virtual value_t H (value_t) = 0;
        
            virtual value_t frequency (void) const = 0;
            virtual value_t frequency (value_t) = 0;

            virtual value_t lacunarity (void) const = 0;
            virtual value_t lacunarity (value_t) = 0;

            virtual value_t amplitude (void) const = 0;
            virtual value_t amplitude (value_t) = 0;

            virtual value_t gain (void) const = 0;
            virtual value_t gain (value_t) = 0;

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
                  value_t _H,
                  value_t _frequency,
                  value_t _lacunarity,
                  value_t _amplitude,
                  value_t _gain):
               data (_seed,
                     _octaves,
                     _H,
                     _frequency,
                     _lacunarity,
                     _amplitude,
                     _gain)
            { ; }
 
            value_t operator() (point_t p) const noexcept  override { return data (p); }

            unsigned octaves (void) const override { return data.octaves (); }
            unsigned octaves (unsigned _octaves) override { return data.octaves (_octaves); }

            value_t H (void) const override { return data.H (); }
            value_t H (value_t _H) override { return data.H (_H); }
        
            value_t frequency (void) const override { return data.frequency (); }
            value_t frequency (value_t _frequency) override { return data.frequency (_frequency); }

            value_t lacunarity (void) const override { return data.lacunarity (); }
            value_t lacunarity (value_t _lacunarity) override { return data.lacunarity (_lacunarity); }

            value_t amplitude (void) const override { return data.amplitude (); }
            value_t amplitude (value_t _amplitude) override { return data.amplitude (_amplitude); }

            value_t gain (void) const override { return data.gain (); }
            value_t gain (value_t _gain) override { return data.gain (_gain); }

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
