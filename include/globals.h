/**
 * @file
 * @author shrek0 (shrek0.tk@gmail.com)
 * @class
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) 2015 shrek0
 *
 * ProtocolLearn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProtocolLearn is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
 */

#ifndef GLOBALS_H
#define GLOBALS_H

// We are not debugging!
#define DEBUG

#ifdef DEBUG
# include <assert.h>
# include <iostream>
# define debug std::cerr << "DEBUG: " << __PRETTY_FUNCTION__ << ":" << __LINE__ << " Says: "
#else
# define assert(expr)
# define debug //
#endif

namespace ProtocolLearn {

template<class T>
inline void IgnoreUnusedParameterWarning(const T &)
{
}

}

//#define IgnoreUnusedParameterWarning(arg) (void)arg

// Check the compiler's version.
#if __cplusplus < 201103L
# error "Your compiler's C++ Version is not supported by ProtocolLearn, try to add the option -std=c++11 to your GCC Compiler."
#endif

#include <Exception>

#include <cstdint>

#endif // GLOBALS_H

