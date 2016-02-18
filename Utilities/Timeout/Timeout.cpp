/**
 * @file timeout.cpp
 * @author shrek0 (shrek0.tk@gmail.com)
  Timeout
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
 */

#include "Timeout.h"

namespace ProtocolLearn {

Timeout::Timeout(const PTime &time, bool startMow)
    : mTimeToWait(time), mTimeout(0, 0) {
    if(startMow)
        start();
}

Timeout::Timeout(PTime::Seconds seconds, PTime::MicroSeconds microSeconds, bool startNow)
    : Timeout{PTime{seconds, microSeconds}, startNow}
{
}

bool Timeout::check() const
{
    return isPassed();
}

bool Timeout::isInfinite() const
{
    return mTimeToWait.isInfinty();
}

bool Timeout::isNoTime() const
{
    return mTimeToWait.isZero();
}

void Timeout::setTimeToWait(const PTime &timeToWait)
{
    mTimeToWait = timeToWait;
}

const PTime &Timeout::getTimeToWait() const
{
    return mTimeToWait;
}

bool Timeout::isPassed() const{
    if(isInfinite())
        return false;
    else if(isNoTime())
        return true;
    
    return PTime::now() >= mTimeout;
}

PTime Timeout::howMuchTimeDoWeHave() const{
    if(isInfinite() || isNoTime())
        return mTimeToWait;

    return mTimeout - PTime::now();
}

void Timeout::start() {
    if(isInfinite() || isNoTime())
        return;

    mTimeout = PTime::now();
    mTimeout += mTimeToWait;
}

} // ProtocolLearn
