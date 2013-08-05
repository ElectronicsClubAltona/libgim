#include "backtrace.hpp"

#include <iostream>

using namespace std;


debug::backtrace::backtrace (void):
    m_frames (DEFAULT_DEPTH) 
{ ; }


ostream&
debug::operator <<(ostream &os, const debug::backtrace&) {
    os << "null backtrace";
    return os;
}


