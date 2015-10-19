

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
