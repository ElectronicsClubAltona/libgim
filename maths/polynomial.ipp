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
 * Copyright 2010 Danny Robson <danny@nerdcruft.net>
 */

#ifdef __UTIL_MATHS_POLYNOMIAL_IPP
#error Double includion of util/maths/polynomial.hpp
#endif

#define __UTIL_MATHS_POLYNOMIAL_IPP


//-----------------------------------------------------------------------------
template <typename T, size_t N>
T
maths::polynomial<T,N>::eval (T x) const {
    T val = 0;
    T exp = 1;

    for (const T &i: coefficients) {
        val += i * exp;
        exp *= x;
    }

    return val;
}


//-----------------------------------------------------------------------------
#include "../json.hpp"
#include "../types.hpp"

namespace json {
    template <typename T, size_t N>
    struct io<maths::polynomial<T,N>> {
        typedef maths::polynomial<T,N> type;

        static type
        deserialise (const json::node &node) {
            const json::array &source = node.as_array ();
            type dest;

            if (source.size () != N)
                throw std::runtime_error ("Polynomial size mismatch in json serialisation");

            for (size_t i = 0; i < N; ++i)
                dest.coefficients[i] = source[i].as_number ();
            return dest;
        }

        static std::unique_ptr<json::node>
        serialise (type src) {
            auto dst = make_unique<json::array> ();

            for (const auto &i: src.coefficients)
                dst->insert (i);

            return std::unique_ptr<json::node> (dst.release ());
        }
    };
}


namespace json {
    //template <typename T, size_t N>
    //std::unique_ptr<json::node>
    //io<maths::polynomial<T,N>>::serialise (const maths::polynomial<T,N>&);

    //template <typename T, size_t N>
    //maths::polynomial<T,N>
    //io<maths::polynomial<T,N>>::deserialise(const json::node &node);
    /*{
        typedef maths::polynomial<T,N> type;

        json::array &source = node.as_array ();
        type dest;

        if (source.size () != N)
            throw std::runtime_error ("Polynomial size mismatch in json serialisation");

        for (size_t i = 0; i < N; ++i)
            dest.coefficients[i] = source[i].as_number ();
        return dest;
    }*/
}


/*namespace json {
    template <typename T, size_t N>
    struct io <maths::polynomial<T,N>> {
        static std::unique_ptr<json::node> serialise (const maths::polynomial<T,N>&);
        static maths::polynomial<T,N> deserialise (const json::node&);
    };

    template <typename T, size_t N>
    std::unique_ptr<node>
    node::serialise<maths::polynomial<T,N>> (const maths::polynomial<T,N> &poly) {
        std::unique_ptr<array> val (new array);

        for (const double &i: poly.coefficients)
            val->insert (node::serialise<double> (i));

        std::unique_ptr<node> ret (val.release ());
        return ret;
    }
}*/


/*from_json<maths::polynomial<T,N>> (const json::node &node) {
    maths::polynomial<T,N> val;

    const json::array &source = node.as_array ();
    for (size_t i = 0; i < N; ++i)
        val.coefficients[i] = source[i].as_number ();

    return val;
}*/
