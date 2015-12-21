/**
 * @file %{FILENAME}
 * @author shrek0 (shrek0.tk@gmail.com)
 * @section LICENSE
 *
 * ProtocolLearn copyright (C) %YEAR% shrek0
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

#ifndef PROTOCOLLEARN_TCP_TCPSEQUENCENUMBER_H
#define PROTOCOLLEARN_TCP_TCPSEQUENCENUMBER_H

#include <cstdint>

namespace ProtocolLearn {
namespace Tcp {

class TcpSequenceNumber
{
public:
    class TcpSequenceWindow
    {
    public:
        TcpSequenceWindow();

        void initSequence(uint32_t begin, uint32_t segmentSize);
        void initWindow(uint32_t windowSize);

        bool isInWindow(uint32_t sequenceNumber) const;
        bool isInAcknowledgmentWindow(uint32_t acknowledgment) const;

        void updateWindow(uint32_t newWindowSize);
        void onPacketReceivedInWindow(uint32_t segementLength);
        void onAcknowledgmentReceived(uint32_t acknowledgment);

        void expendWindow(uint32_t size);

        bool isZeroWindow() const;
        bool isSynced() const;
        bool willBeSynced(uint32_t acknowledgmentNumber) const;

        uint32_t getEnd() const;
        uint32_t getBegin() const;
        uint32_t getNextSequence() const;
        uint32_t getWindowSize() const;
        uint32_t getCurrentWindowSize() const;
        uint32_t getLastAcknowledgment() const;

        void resetWindow();

        TcpSequenceNumber createSequenceNumber() const;
        TcpSequenceNumber createSequenceNumber(uint32_t sequenceNumber) const;

        bool operator ==(const TcpSequenceWindow &tcpSequenceWindow);

    private:
        static bool isInRange(uint32_t start, uint32_t end, uint32_t toTest);
        uint32_t getTrueWindowBegin() const;

        uint32_t mBegin;
        uint32_t mCurrentWindowSize;
        uint32_t mWindowSize;
        uint32_t mLastAcknowledgment;
    };

    TcpSequenceNumber(uint32_t sequenceNumber, const TcpSequenceWindow &window);
    TcpSequenceNumber(uint32_t sequenceNumber, uint32_t begin, uint32_t end);

    uint32_t to32BitInteger() const{ return mSequenceNumber; }

    bool operator ==(uint32_t sequenceNumber) const;
    bool operator !=(uint32_t sequenceNumber) const;

    bool operator >(uint32_t sequenceNumber) const;
    bool operator <(uint32_t sequenceNumber) const;
    bool operator <=(uint32_t sequenceNumber) const;
    bool operator >=(uint32_t sequenceNumber) const;

    bool operator ==(const TcpSequenceNumber &tcpSequenceNumber) const;
    bool operator !=(const TcpSequenceNumber &tcpSequenceNumber) const;

    bool operator >(const TcpSequenceNumber &tcpSequenceNumber) const;
    bool operator <(const TcpSequenceNumber &tcpSequenceNumber) const;
    bool operator <=(const TcpSequenceNumber &tcpSequenceNumber) const;
    bool operator >=(const TcpSequenceNumber &tcpSequenceNumber) const;

private:
    static bool isBefore(uint32_t sequenceNumber, uint32_t anotherSequenceNumber)
    {
        return static_cast<int32_t>(sequenceNumber-anotherSequenceNumber) < 0;
    }

    uint32_t mSequenceNumber;
    uint32_t mBegin;
    uint32_t mEnd;
};

} // namespace Tcp
} // namespace ProtocolLearn

#endif // PROTOCOLLEARN_TCP_TCPSEQUENCENUMBER_H
