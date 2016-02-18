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

#ifndef PTIME_H
#define PTIME_H

#include <ctime>

#include <sys/time.h>

#include <cstdint>
#include <unistd.h>

namespace ProtocolLearn {

class PTime
{
public:
    typedef time_t Seconds;
    typedef useconds_t MicroSeconds;

    //    static const MicroSeconds MicroSecondsMaxValue = 1000000-1;
    static const MicroSeconds MicroSecondsMaxValue;
    static const MicroSeconds MicroSecondsInfinty;

    struct TimeVal{
        TimeVal(Seconds seconds, MicroSeconds microSeconds)
            : seconds(seconds), microSeconds(microSeconds)
        { }

        TimeVal()
            : seconds(0), microSeconds(0)
        { }

        Seconds seconds;
        MicroSeconds microSeconds;
    };

    explicit PTime(const TimeVal &timeVal);
    explicit PTime(const timeval &timeVal);
    explicit PTime(const Seconds &seconds, const MicroSeconds &microSeconds);

    PTime(PTime &&) = default;
    PTime(const PTime &) = default;

    Seconds getSeconds() const{ return time.seconds; }
    MicroSeconds getMicroSeconds() const{ return time.microSeconds; }

    void setSeconds(const Seconds &seconds) { time.seconds = seconds; }
    void setMicroSeconds(const MicroSeconds &microSeconds) { time.microSeconds = microSeconds; }

    static PTime now();
    static PTime max();
    static PTime infinity();
    static PTime zero();

    static bool equal(const PTime &time, const PTime &anotherTime);
    static bool greaterThan(const PTime &time, const PTime &anotherTime);
    static bool lesserThan(const PTime &time, const PTime &anotherTime);

    bool isInfinty() const;
    bool isZero() const;

    void addSeconds(const Seconds &seconds) { *this += PTime{seconds, 0}; }
    void addMicroSeconds(const MicroSeconds &microSeconds) { *this += PTime{0, microSeconds}; }

    bool operator ==(const PTime &anotherTime) const{ return equal(*this, anotherTime); }
    bool operator !=(const PTime &anotherTime) const{ return !equal(*this, anotherTime); }

    bool operator >(const PTime &anotherTime) const{ return greaterThan(*this, anotherTime); }
    bool operator <(const PTime &anotherTime) const{ return lesserThan(*this, anotherTime); }

    bool operator >=(const PTime &anotherTime) const{ return equal(*this, anotherTime) || greaterThan(*this, anotherTime); }
    bool operator <=(const PTime &anotherTime) const{ return equal(*this, anotherTime) || lesserThan(*this, anotherTime); }

    PTime &operator =(const PTime &anotherTime) = default;
    PTime &operator =(PTime &&anotherTime) = default;

    PTime operator +(const PTime &anotherTime) const;
    PTime operator -(const PTime &anotherTime) const;

    PTime &operator -=(const PTime &anotherTime) { *this = *this - anotherTime; return *this; }
    PTime &operator +=(const PTime &anotherTime) { *this = *this + anotherTime; return *this; }

private:
    TimeVal time;

};

}

#endif // PTIME_H
