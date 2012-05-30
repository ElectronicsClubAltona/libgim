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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COORD_HPP
#define __UTIL_COORD_HPP

namespace util {
    namespace detail {
        // Disable GCC warnings about validity of anonyous structures in
        // unions. Push comes to shove I'll manually redsign everything to
        // keep this syntax anyway.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
        template <size_t S>
        struct coord_data {
            coord_data () { ; }

            template <typename ...T>
            coord_data (T ...t): data{t...}
            { ; }

            double data[S];
        };

        template <>
        struct coord_data<1> {
            coord_data () { ; }

            template <typename ...T>
            coord_data (T ...t): data{t...}
            { ; }

            union {
                double data[1];
                double x;
            };
        };

        template <>
        struct coord_data<2> {
            coord_data () { ; }

            template <typename ...T>
            coord_data (T ...t): data{t...}
            { ; }

            union {
                double data[2];
                struct {
                    double x;
                    double y;
                };
            };
        };

        template <>
        struct coord_data<3> {
            union {
                double data[3];
                struct {
                    double x;
                    double y;
                    double z;
                };
            };

            coord_data () { ; }

            template <typename... T>
            coord_data (T... t): data{t...}
            { ; }
        };
#pragma GCC diagnostic pop
    }
}

#endif
