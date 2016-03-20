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

#include <Random.h>
#include <Convertions.h>

#include <iostream>


int usage(const char *programName);
ProtocolLearn::Random::RandomFunction getRandomnessByNumber(uint32_t n);


int usage(const char *programName) {
    std::cout << "Usage: " << programName << " <Minimum Number> <Maximum Number> <Randomness number>" << std::endl;

    return -1;
}

ProtocolLearn::Random::RandomFunction getRandomnessByNumber(uint32_t n) {
    using ProtocolLearn::Random;

    switch (n) {
    case 0:
        return Random::getLowRandomNumber;
    case 1:
        return Random::getMediumRandomNumber;
    case 2:
        return Random::getHighRandomNumber;
    default:
        pl_assert(false);
        break;
    }

    return ProtocolLearn::Random::RandomFunction{};
}

int main(int argc, char *argv[]) {
    using ProtocolLearn::Convertions;
    using ProtocolLearn::Random;

    if(argc < 3)
        return usage(argv[0]);

    if(argc == 4) {
        std::cout << Random::getRandomNumberInRange(
                         Convertions::toInteger<uint32_t>(argv[1]),
                         Convertions::toInteger<uint32_t>(argv[2]),
                         getRandomnessByNumber(Convertions::toInteger<uint32_t>(argv[3]))
                )
                << std::endl;
    }
    else
        std::cout << Random::getRandomNumberInRange(Convertions::toInteger<uint32_t>(argv[1]), Convertions::toInteger<uint32_t>(argv[2])) << std::endl;

    return 0;
}
