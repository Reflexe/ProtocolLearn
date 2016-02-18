/**
 * @file convertions.h
 * @author shrek0 (shrek0.tk@gmail.com)
  Convertions
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
 * Contain a class for String-Integer Convertions.
 */

#ifndef CONVERTIONS_H
#define CONVERTIONS_H

#include <string>
#include <sstream>

#include "Debug.h"
#include "Exceptions.h"

namespace ProtocolLearn {

class Convertions
{
private:
    /// Some helpers for the functions toString and toInteger.

    template<typename T>
    static std::string convertIntegerTo(T integer, std::ios_base& (*func)(std::ios_base&)) {
       std::stringstream stringstream;

       stringstream << func << +integer;

       return stringstream.str();
    }

    template<typename T>
    static T convertStringToSignedInteger(const std::string &string, int base)
    {
        return std::stoi(string, nullptr, base);
    }

    template<typename T>
    static T convertStringToUnsignedInteger(const std::string &string, int base)
    {
        return std::stoul(string, nullptr, base);
    }

public:
    Convertions();

    template<typename T>
    static T toInteger(const std::string &string, int base=10) {
        if(std::is_unsigned<T>::value)
            return convertStringToUnsignedInteger<T>(string, base);
        else
            return convertStringToSignedInteger<T>(string, base);
    }


    template<typename T>
    static std::string toString(T integer, int base=10) {
        switch(base) {
        case 10:
            return std::to_string(integer);
            break;
        case 16:
            return convertIntegerTo<T>(integer, std::hex);
            break;
        case 8:
            return convertIntegerTo<T>(integer, std::oct);
            break;
        default:
            return "";
            pl_assert(false && "base isn't a supported base");
        }
    }
};

template<>
inline unsigned long long Convertions::convertStringToUnsignedInteger<unsigned long long>(const std::string &string, int base)
{
    return std::stoull(string, nullptr, base);
}

template<>
inline signed long long Convertions::convertStringToSignedInteger<signed long long>(const std::string &string, int base)
{
    return std::stoll(string, nullptr, base);
}

template<>
inline signed long Convertions::convertStringToSignedInteger<signed long>(const std::string &string, int base)
{
    return std::stol(string, nullptr, base);
}

} // ProtocolLearn

#endif // CONVERTIONS_H
