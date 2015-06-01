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

#ifdef __UTIL_NOISE_FRACTAL_RMF_IPP
#error
#endif
#define __UTIL_NOISE_FRACTAL_RMF_IPP


namespace util { namespace noise { namespace fractal {
    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename B>
    rmf<T,B>::rmf (unsigned _octaves,
                             T        _frequency,
                             T        _lacunarity,
                             T        _amplitude,
                             T        _gain,
                             seed_t   _seed):
        seed (_seed),
        octaves (_octaves),
        frequency (_frequency),
        lacunarity (_lacunarity),
        amplitude (_amplitude),
        gain (_gain),
        basis (_seed)
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    rmf<T,B>::rmf ():
        rmf<T,B> (DEFAULT_OCTAVES,
                  DEFAULT_FREQUENCY,
                  DEFAULT_LACUNARITY,
                  DEFAULT_AMPLITUDE,
                  DEFAULT_GAIN,
                  rand ())
    { ; }


    //-------------------------------------------------------------------------
    template <typename T, typename B>
    constexpr T
    rmf<T,B>::operator() (util::point<2,T> p) const
    {
        const T offset = 1;
        const T H = 1.f;

        T exponents[octaves];
        for (size_t i = 0; i < octaves; ++i)
            exponents[i] = std::pow (std::pow (lacunarity, float (i)), -H);

        T signal = 0;
        T result = 0;
        T weight = 1;

        p *= frequency;

        for (size_t i = 0; i < octaves; ++i) {
            // generates ridged noise
            signal = basis (p);
            signal = std::fabs (signal);
            signal = offset - signal;

            // sharpens the ridges
            signal *= signal;

            // influence by sharpness of previous iteration
            signal *= weight;

            // contribute to the weight
            weight = signal * gain;
            weight = limit (weight, 0, 1);

            // record and continue
            result += signal * exponents[i];

            p *= lacunarity;
        }

        return result;
    }
} } }
