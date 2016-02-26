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
 * Copyright:
 *      2015, Danny Robson <danny@nerdcruft.net>
 */

#include "adapter.hpp"

//#include <vector>
//#include <tuple>
//#include <iostream>
//
//
//using util::adapter::scalar;
//
//
//void
//foo (void) {
//    std::vector<std::tuple<int,int,int>> vals;
//
//    using iterator_t = util::adapter::scalar<0, decltype(vals.begin ())>;
//
//    static_assert (
//        std::is_same<
//            typename std::iterator_traits<
//                decltype(vals.begin ())
//            >::value_type,
//            std::tuple<int,int,int>
//        >::value
//    );
//
//    static_assert (
//        std::is_same<
//            typename std::tuple_element<0, std::tuple<int,int,int>>::type,
//            int
//        >::value
//    );
//
//    iterator_t end (vals.end ());
//
//    //static_assert (
//    //    std::is_same<
//    //        typename iterator_t::reference,
//    //        //decltype(*std::declval <scalar<0,std::tuple<int,int,int>>> ()),
//    //        //typename scalar<0,decltype(vals.begin ())>::value_type,
//    //        int&
//    //    >::value
//    //);
//
//    for (auto p = iterator_t (vals.begin ()), last = iterator_t (vals.end ()); p != last; ++p)
//        int b = *p;
//}
