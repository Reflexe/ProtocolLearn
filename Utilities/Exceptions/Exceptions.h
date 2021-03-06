/**
 * @file Exceptions.h
 * @author shrek0 (shrek0.tk@gmail.com)
  Exception
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
 * Contains commoon excptions classes.
 */

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <stdexcept>

namespace ProtocolLearn {

typedef ::std::exception Exception;
typedef ::std::runtime_error RuntimeError;
typedef ::std::logic_error LogicError;
typedef ::std::out_of_range OutOfRange;
typedef ::std::invalid_argument InvalidArgument;


} // ProtocolLearn

#endif // EXCEPTIONS_H
