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

#include "PTime.h"

#include <limits>

#include "Debug.h"

namespace ProtocolLearn {

const PTime::MicroSeconds PTime::MicroSecondsMaxValue = 1000000-1;
const PTime::MicroSeconds PTime::MicroSecondsInfinty = MicroSecondsMaxValue+1;

PTime::PTime(const TimeVal &timeVal)
    : time{timeVal}
{
    pl_assert(timeVal.microSeconds <= MicroSecondsInfinty);
}

PTime::PTime(const timeval &timeVal)
    : PTime{timeVal.tv_sec, static_cast<MicroSeconds>(timeVal.tv_usec)}
{
    pl_assert(timeVal.tv_usec <= MicroSecondsInfinty);
}

PTime::PTime(const Seconds &seconds, const MicroSeconds &microSeconds)
    : time{seconds, microSeconds}
{
    pl_assert(microSeconds <= MicroSecondsInfinty);
}

PTime PTime::now() {
    timeval timeVal;

    ::gettimeofday(&timeVal, NULL);
    pl_assert(timeVal.tv_usec <= MicroSecondsInfinty);

    return PTime{timeVal};
}

PTime PTime::max()
{
    return PTime{std::numeric_limits<Seconds>::max(), MicroSecondsInfinty};
}

PTime PTime::infinity()
{
    return PTime{0, MicroSecondsInfinty};
}

PTime PTime::zero()
{
    return PTime{0, 0};
}

bool PTime::equal(const PTime &time, const PTime &anotherTime) {
    return anotherTime.getMicroSeconds() == time.getMicroSeconds()
            && anotherTime.getSeconds() == time.getSeconds();
}

bool PTime::lesserThan(const PTime &time, const PTime &anotherTime)
{
    return greaterThan(anotherTime, time); // if X is greater than me, then I'm lesser than X.
}

bool PTime::isInfinty() const
{
    return getMicroSeconds() == MicroSecondsInfinty;
}

bool PTime::isZero() const
{
    return getMicroSeconds() == 0 && getSeconds() == 0;
}

PTime PTime::operator +(const PTime &anotherTime) const{
    if(isInfinty() || anotherTime.isInfinty())
        return anotherTime;

    pl_assert(anotherTime.getMicroSeconds() <= MicroSecondsMaxValue);
    pl_assert(getMicroSeconds() <= MicroSecondsMaxValue);

    if(anotherTime.getMicroSeconds() + getMicroSeconds() > MicroSecondsMaxValue)
        return PTime(getSeconds()+anotherTime.getSeconds()+1, (getMicroSeconds()+anotherTime.getMicroSeconds())-(MicroSecondsMaxValue+1));
    else
        return PTime(getSeconds()+anotherTime.getSeconds(), getMicroSeconds()+anotherTime.getMicroSeconds());

    pl_assert(getMicroSeconds() <= MicroSecondsMaxValue);
}

PTime PTime::operator -(const PTime &anotherTime) const{
    if(isInfinty() || anotherTime.isInfinty())
        return anotherTime;

    pl_assert(anotherTime.getMicroSeconds() <= MicroSecondsMaxValue);
    pl_assert(getMicroSeconds() <= MicroSecondsMaxValue);

    if(anotherTime.getMicroSeconds() > getMicroSeconds())
        return PTime(getSeconds()-anotherTime.getSeconds()-1, (MicroSecondsMaxValue+1) - (anotherTime.getMicroSeconds()-getMicroSeconds()));
    else
        return PTime(getSeconds()-anotherTime.getSeconds(), getMicroSeconds()-anotherTime.getMicroSeconds());

    pl_assert(getMicroSeconds() <= MicroSecondsMaxValue);
}

bool PTime::greaterThan(const PTime &time, const PTime &anotherTime) {
    if(time.isInfinty() && anotherTime.isInfinty() == false)
        return true;
    else if(anotherTime.isInfinty())
        return false;

    return time.getSeconds() > anotherTime.getSeconds()
            || (time.getSeconds() == anotherTime.getSeconds()
                && time.getMicroSeconds() > anotherTime.getMicroSeconds());
}

}
