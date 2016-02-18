/**
 * @file timeout.h
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

#ifndef TIMEOUT_H
#define TIMEOUT_H

#include "PTime.h"
#include "Exceptions.h"

namespace ProtocolLearn {

/**
 * @brief The Timeout class  A timer that used by PacketStream and DataStream.
 */
class Timeout
{
public:
    typedef PTime TimeType;

    class TimeoutException : public RuntimeError
    {
    public:
        using RuntimeError::RuntimeError;
    };

    Timeout(const PTime &time, bool startMow=true);
    Timeout(PTime::Seconds seconds=0, PTime::MicroSeconds microSeconds=0, bool startNow=true);

    /**
     * @brief start  Start the timer, restarting if the timer is already running.
     */
    void start();

    /**
     * @brief isPassed  Check if the time passed.
     * @return isPassed.
     */
    bool isPassed() const;

    /**
     * @brief check  A clone for isPassed.
     * @return isPassed();
     */
    bool check() const;

    /**
     * @brief howMuchTimeWeHave
     * @return
     */
    PTime howMuchTimeDoWeHave() const;

    /**
     * @brief Is this timeout will never end?
     * @return
     */
    bool isInfinite() const;

    /**
     * @brief Is this timeout will always pass?
     * @return
     */
    bool isNoTime() const;

    void setTimeToWait(const PTime &timeToWait);

    const PTime &getTimeToWait() const;

private:
    /**
     * @brief mTimeVal  Time to wait.
     */
    PTime mTimeToWait;

    /**
     * @brief mTimeout  The time the timer will passed in.
     */
    PTime mTimeout;

};

} // ProtocolLearn

#endif // TIMEOUT_H
