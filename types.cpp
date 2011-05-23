#include "types.hpp"

using namespace std;


#define do_type_to_string(T)                                                    \
template <> std::string type_to_string <T>       (void) { return #T; }          \
template <> std::string type_to_string <const T> (void) { return "const " #T; }

do_type_to_string (float)
do_type_to_string (double)

