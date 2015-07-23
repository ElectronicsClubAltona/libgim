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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#if defined(__UTIL_NOISE_BASIS_PATCH_IPP)
#error
#endif
#define __UTIL_NOISE_BASIS_PATCH_IPP


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::noise::basis::patch<T>::patch (seed_t _seed):
    m_seed (_seed)
{ ; }


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::range<T>
util::noise::basis::patch<T>::bounds (void) const
{
    return { T{0}, T{1} };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
util::noise::basis::patch<T>::operator () (point2<T> p) const
{
    // extract integer and fractional parts. be careful to always round down
    // (particularly with negatives) and avoid rounding errors.
    auto p_int = p.template cast<intmax_t> ();
    if (p.x < 0) p_int.x -= 1;
    if (p.y < 0) p_int.y -= 1;
    auto p_rem = (p - p_int).template as<point> ();

    T closest = std::numeric_limits<T>::infinity ();
    T value;

    for (signed y = -1; y <= 1; ++y)
        for (signed x = -1; x <= 1; ++x) {
            util::vector2i offset {x, y};
            auto c = centroid (p_int + offset);
            auto d = util::distance (p_rem, c + offset);

            if (d < closest) {
                closest = d;
                value = generate (p_int + offset);
            }
        }

    return value;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::noise::seed_t
util::noise::basis::patch<T>::seed (void) const
{
    return m_seed;
}


//-----------------------------------------------------------------------------
template <typename T>
util::noise::seed_t
util::noise::basis::patch<T>::seed (util::noise::seed_t _seed)
{
    return m_seed = _seed;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::point2<T>
util::noise::basis::patch<T>::centroid (util::point2i p) const
{
    using util::hash::murmur2::mix;

    auto u = mix (m_seed, mix (uint64_t (p.x), uint64_t (p.y)));
    auto v = mix (u, m_seed);

    auto r = util::point<2,T> {
        (u & 0xffff) / T{0xffff},
        (v & 0xffff) / T{0xffff}
    };

    CHECK_LIMIT (r, T{0}, T{1});
    return r;
}


//-----------------------------------------------------------------------------
template <typename T>
T
util::noise::basis::patch<T>::generate (util::point2i p) const
{
    using util::hash::murmur2::mix;

    auto u = mix (m_seed, mix (uint64_t (p.x), uint64_t (p.y)));
    return (u & 0xffff) / T{0xffff};
}
