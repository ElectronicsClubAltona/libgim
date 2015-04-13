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

#include "extent.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <cmath>


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::extent<S,T>::extent (vector<S,T> _v)
{
    std::copy (std::begin (_v.data),
               std::end   (_v.data),
               std::begin (this->data));
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
T
util::extent<S,T>::diameter (void) const
{
    return static_cast<T> (
        std::sqrt (
            std::accumulate (std::begin (this->data),
                             std::end   (this->data),
                             T {0},
                             [] (auto a, auto b) { return a + b * b; })
        )
    );
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T
util::extent<S,T>::area (void) const
{
    return std::accumulate (std::begin (this->data),
                            std::end   (this->data),
                            T {1},
                            std::multiplies<T> ());
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::extent<S,T>
util::extent<S,T>::expanded (util::vector<S,T> mag) const
{
    return *this + mag;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::extent<S,T>
util::extent<S,T>::expanded (T t) const
{
    return *this + util::vector<S,T> {t};
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::extent<S,T>
util::extent<S,T>::contracted (vector<S,T> t) const
{
    return *this - t;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::extent<S,T>
util::extent<S,T>::contracted (T t) const
{
    return *this - vector<S,T> {-t};
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
bool
util::extent<S,T>::empty (void) const
{
    return almost_equal (area(), 0);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
const util::extent<S,T> util::extent<S,T>::MIN { 0 };


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const util::extent<S,T> util::extent<S,T>::MAX {
    std::numeric_limits<T>::max ()
};


///////////////////////////////////////////////////////////////////////////////
namespace debug {
    template <size_t S, typename T>
    struct validator<util::extent,S,T> {
        static bool is_valid (const util::extent<S,T> &e)
        {
            return std::all_of (std::begin (e.data),
                                std::end   (e.data),
                                [] (auto i) { return i >= 0; });
        }
    };
}

template bool debug::valid (const util::extent<2,float>&);
template bool debug::valid (const util::extent<2,double>&);
template bool debug::valid (const util::extent<2,uint16_t>&);
template bool debug::valid (const util::extent<2,uint32_t>&);
template bool debug::valid (const util::extent<2,uint64_t>&);


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, util::extent<S,T> e)
{
    os << "[";
    std::copy (std::begin (e.data),
               std::end   (e.data),
               std::ostream_iterator<T> (os, ", "));
    os << "]";
    return os;
}


//-----------------------------------------------------------------------------
namespace util {
    #define INSTANTIATE_S_T(S,T)                                        \
    template struct extent<S,T>;                                        \
    template std::ostream& operator<< (std::ostream&, extent<S,T>);

    #define INSTANTIATE(T)  \
    INSTANTIATE_S_T(2,T)    \
    INSTANTIATE_S_T(3,T)

    INSTANTIATE(uint32_t)
    INSTANTIATE(uint64_t)
    INSTANTIATE(float)
    INSTANTIATE(double)
}
