/**
 * @file
 * @author shrek0 (shrek0.tk@gmail.com)
 
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

#ifndef PROTOCOLLEARN_DEBUG_H
#define PROTOCOLLEARN_DEBUG_H

#include <iostream>
#include <unordered_set>

#include "Exceptions.h"

#include "DebugConfig.h"

namespace ProtocolLearn {

/**
 * @brief The Debug class  My Debugging Toolbox.
 */
class Debug
{
public:
    constexpr Debug();

#ifdef PL_DEBUG
    class AssertException : public RuntimeError
    {
        using RuntimeError::RuntimeError;
    };

    static void assert(bool result, const char *expr, const char *functionName, int line, const char *);

    constexpr static auto &debug = std::cerr;
    constexpr static auto &crap  = debug;

    static void addDebbugedFile(const std::string &string);
    static void addDebbugedClass(const std::string &string);
    static bool isDebbugedFile(const std::string &string);

private:
    static std::unordered_set<std::string> mDebbugedFiles;
#else
public:
    static void addDebbugedClass(const std::string &string)
    {}
#endif
};

//class DebugColors {
//#define pl__define_color(ColorName, Number) constexpr static char ColorName[] = "\033["#Number"m";
//#define pl__define_colorb(ColorName, BackgroundNumber, Number) constexpr static char ColorName[] = "\033["#Number"m";


//    pl__define_color(Regular, 0);
//    pl__define_color(Black, 30);
//    pl__define_color(Red, 31);
//    pl__define_color(Green, 32);
//    pl__define_color(Brown, 33);
//    pl__define_color(Blue, 34);
//    pl__define_color(purple, 35);
//    pl__define_color(Cyan, 36);
//    pl__define_color(Gray, 37);

//#undef pl__define_colorb
//#undef pl__define_color
//};

} // namespace ProtocolLearn

#ifdef PL_DEBUG
# define pl_assert(expr) ::ProtocolLearn::Debug::assert((expr), #expr, __PRETTY_FUNCTION__, __LINE__, __FILE__)
# define pl_debug(debug_string) if(::ProtocolLearn::Debug::isDebbugedFile(__FILE__)) (::ProtocolLearn::Debug::crap << "[\033[1;31m" << \
    __PRETTY_FUNCTION__ << "\033[0m]: \033[1;35m" << debug_string << "\033[0m" << ::std::endl)
# define pl_crap(debug_string) pl_debug(debug_string)
# define pl_strange(debug_string) (::ProtocolLearn::Debug::debug << "[\033[1;36m" << __PRETTY_FUNCTION__ << "\033[0m]: \033[1;35m" << \
    debug_string << "\033[0m" << ::std::endl)
#else
# define pl_assert(expr) ((void)0)
# define pl_debug(debug_string) ((void)0)
# define pl_crap(debug_string) ((void)0)
# define pl_strange(debug_string) ((void)0)
#endif

#endif // PROTOCOLLEARN_DEBUG_H
