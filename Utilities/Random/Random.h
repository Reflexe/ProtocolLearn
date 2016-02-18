/**
 * @file Random.h
 * @author shrek0 (shrek0.tk@gmail.com)
  Random
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

#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>

#include <functional>

namespace ProtocolLearn {

class Random
{

public:
    typedef std::function<uint32_t()> RandomFunction;

    Random();

    static uint32_t getLowRandomNumber();
    static uint32_t getMediumRandomNumber();
    static uint32_t getHighRandomNumber();

//    enum class RandomnessLevel{
//        High,
//        Medium,
//        Low
//    };

//    template<RandomnessLevel randomnessLevel>
//    static uint32_t getRandomNumber() {
//        /// This algorithm seems to be buggy, we should replace it.
//        /// @see https://www.xkcd.com/221/
//        return 4; // chosen by fair dice roll
//                  // guaranteed to be random.
//    }

    /**
     * @brief getRandomNumberInRange  Return a random number in the range of @arg start to @arg end.
     * @param start
     * @param end
     * @return
     */
    static uint32_t getRandomNumberInRange(uint32_t start, uint32_t end, const RandomFunction &randomFunction = getMediumRandomNumber);

    static bool getRandomBoolean(const RandomFunction &randomFunction = getMediumRandomNumber);

};

} // namespace ProtocolLearn

#endif // RANDOM_H
