/**
 * @file Random.cpp
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

#include "Random.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "PTime.h"
#include "Exceptions.h"

namespace ProtocolLearn {

Random::Random()
{
}

uint32_t Random::getLowRandomNumber()
{
    return PTime::now().getMicroSeconds();
}

uint32_t Random::getMediumRandomNumber() {
#ifdef __linux__
    uint32_t randomNumber = 0;

    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, reinterpret_cast<void *>(&randomNumber), sizeof(randomNumber));
    close(fd);

    return randomNumber;
#endif
}

uint32_t Random::getHighRandomNumber() {
#ifdef __linux__
    uint32_t randomNumber = 0;

    int fd = open("/dev/random", O_RDONLY);
    read(fd, reinterpret_cast<void *>(&randomNumber), sizeof(randomNumber));
    close(fd);

    return randomNumber;
#endif
}

uint32_t Random::getRandomNumberInRange(uint32_t start, uint32_t end, const RandomFunction &randomFunction) {
    if(start == end || end == 0 || start > end)
        throw InvalidArgument("Random::getRandomNumberInRange");

    return (randomFunction() % ((end-start)+1)) + start;
}

bool Random::getRandomBoolean(const RandomFunction &randomFunction)
{
    return (randomFunction() % 2) == 0;
}

} // namespace ProtocolLearn
