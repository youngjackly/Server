#pragma once

#include <sstream>
//just make it built through on android
#ifdef ANDROID

namespace std {
template < typename T >
std::string to_string( const T& n )
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

}
#endif
