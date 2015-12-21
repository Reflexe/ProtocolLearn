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

#include "TcpSequenceNumber.h"
#include "Debug.h"

namespace ProtocolLearn {
namespace Tcp {

bool TcpSequenceNumber::TcpSequenceWindow::isInRange(uint32_t start, uint32_t end, uint32_t toTest)
{
    return (toTest == start || isBefore(start, end)) && isBefore(start, end);
}

uint32_t TcpSequenceNumber::TcpSequenceWindow::getTrueWindowBegin() const
{
    return mBegin - (mWindowSize - mCurrentWindowSize);
}

uint32_t TcpSequenceNumber::TcpSequenceWindow::getCurrentWindowSize() const
{
    return mCurrentWindowSize;
}

uint32_t TcpSequenceNumber::TcpSequenceWindow::getLastAcknowledgment() const
{
    return mLastAcknowledgment;
}

TcpSequenceNumber::TcpSequenceWindow::TcpSequenceWindow()
{
}

void TcpSequenceNumber::TcpSequenceWindow::initSequence(uint32_t begin, uint32_t segmentSize) {
    mBegin = begin+segmentSize;
    mLastAcknowledgment = begin;
//    mWindowSize = mCurrentWindowSize = 0;
}

void TcpSequenceNumber::TcpSequenceWindow::initWindow(uint32_t windowSize)
{
    mWindowSize = mCurrentWindowSize = windowSize;
}

bool TcpSequenceNumber::TcpSequenceWindow::isInWindow(uint32_t sequenceNumber) const
{
    return isInRange(mBegin, getEnd(), sequenceNumber);
}

bool TcpSequenceNumber::TcpSequenceWindow::isInAcknowledgmentWindow(uint32_t acknowledgment) const
{
    return isInRange(mLastAcknowledgment-1, mBegin+1, acknowledgment);
}

void TcpSequenceNumber::TcpSequenceWindow::updateWindow(uint32_t newWindowSize) {
    if(newWindowSize == mCurrentWindowSize)
        return;
    else if(newWindowSize == mWindowSize)
        return resetWindow();

    pl_debug("New window size used: " << newWindowSize << " VS " << mWindowSize);

    mCurrentWindowSize = newWindowSize;
    mWindowSize = newWindowSize;
}

void TcpSequenceNumber::TcpSequenceWindow::onPacketReceivedInWindow(uint32_t segementLength) {
    mBegin += segementLength;
    mCurrentWindowSize -= segementLength;
}

void TcpSequenceNumber::TcpSequenceWindow::onAcknowledgmentReceived(uint32_t acknowledgment)
{
    mLastAcknowledgment = acknowledgment;
}

void TcpSequenceNumber::TcpSequenceWindow::expendWindow(uint32_t size) {
    pl_assert(size + getCurrentWindowSize() <= getWindowSize());

    if(getCurrentWindowSize()+size == getWindowSize())
        return resetWindow();
    else
        mCurrentWindowSize += size;
}

bool TcpSequenceNumber::TcpSequenceWindow::isZeroWindow() const
{
    return mCurrentWindowSize == 0;
}

bool TcpSequenceNumber::TcpSequenceWindow::isSynced() const
{
    return mLastAcknowledgment == mBegin;
}

bool TcpSequenceNumber::TcpSequenceWindow::willBeSynced(uint32_t acknowledgmentNumber) const
{
    return mBegin == acknowledgmentNumber;
}

uint32_t TcpSequenceNumber::TcpSequenceWindow::getEnd() const
{
    return mBegin + mCurrentWindowSize;
}

uint32_t TcpSequenceNumber::TcpSequenceWindow::getBegin() const
{
    return mBegin;
}

uint32_t TcpSequenceNumber::TcpSequenceWindow::getNextSequence() const
{
    return mBegin;
}

uint32_t TcpSequenceNumber::TcpSequenceWindow::getWindowSize() const
{
    return mWindowSize;
}

void TcpSequenceNumber::TcpSequenceWindow::resetWindow()
{
    mCurrentWindowSize = mWindowSize;
}

ProtocolLearn::Tcp::TcpSequenceNumber ProtocolLearn::Tcp::TcpSequenceNumber::TcpSequenceWindow::createSequenceNumber() const
{
    return createSequenceNumber(mBegin);
}

TcpSequenceNumber TcpSequenceNumber::TcpSequenceWindow::createSequenceNumber(uint32_t sequenceNumber) const
{
    return TcpSequenceNumber{sequenceNumber, getTrueWindowBegin(), getEnd()};
}

bool TcpSequenceNumber::TcpSequenceWindow::operator ==(const TcpSequenceNumber::TcpSequenceWindow &tcpSequenceWindow)
{
    return mBegin == tcpSequenceWindow.mBegin && mWindowSize == tcpSequenceWindow.mWindowSize;
}

TcpSequenceNumber::TcpSequenceNumber(uint32_t sequenceNumber, const TcpSequenceNumber::TcpSequenceWindow &window)
    : mSequenceNumber{sequenceNumber}, mBegin{window.getBegin()}, mEnd{window.getEnd()}
{
}

TcpSequenceNumber::TcpSequenceNumber(uint32_t sequenceNumber, uint32_t begin, uint32_t end)
    : mSequenceNumber{sequenceNumber}, mBegin{begin}, mEnd{end}
{
}

bool TcpSequenceNumber::operator ==(uint32_t sequenceNumber) const
{
    return mSequenceNumber == sequenceNumber;
}

bool TcpSequenceNumber::operator !=(uint32_t sequenceNumber) const
{
    return mSequenceNumber != sequenceNumber;
}

bool TcpSequenceNumber::operator >(uint32_t sequenceNumber) const
{
    return isBefore(sequenceNumber, mSequenceNumber);
}

bool TcpSequenceNumber::operator <(uint32_t sequenceNumber) const
{
    return isBefore(mSequenceNumber, sequenceNumber);
}

bool TcpSequenceNumber::operator <=(uint32_t sequenceNumber) const
{
    return *this == sequenceNumber || *this < sequenceNumber;
}

bool TcpSequenceNumber::operator >=(uint32_t sequenceNumber) const
{
    return *this == sequenceNumber || *this > sequenceNumber;
}

bool TcpSequenceNumber::operator ==(const TcpSequenceNumber &tcpSequenceNumber) const
{
    return *this == tcpSequenceNumber.mSequenceNumber;
}

bool TcpSequenceNumber::operator !=(const TcpSequenceNumber &tcpSequenceNumber) const
{
    return *this != tcpSequenceNumber.mSequenceNumber;
}

bool TcpSequenceNumber::operator >(const TcpSequenceNumber &tcpSequenceNumber) const{
//    pl_assert(tcpSequenceNumber.mBegin == mBegin);
//    pl_assert(tcpSequenceNumber.mEnd == mEnd);

    // signed(a-b) < 0
    // if (a - b) > std::numeric_limits<int32_t>::max();
    // if a > (range/2) and b < range/2: return true;
    // if a > b and b < range/2.
    // a-b > 2**31-1
    // Options: (when the 31th digit will be 1.) (when result is in {2**31..2**32-1})
    //   1. b > a and (b < (2**31 + a))     // If
    //   2. a > 2**31-1 and (a-2**31-1) > b.
    //   3.

    return *this > tcpSequenceNumber.mSequenceNumber;
}

bool TcpSequenceNumber::operator <(const TcpSequenceNumber &tcpSequenceNumber) const{
//    pl_assert(tcpSequenceNumber.mBegin == mBegin);
//    pl_assert(tcpSequenceNumber.mEnd == mEnd);

    return *this < tcpSequenceNumber.mSequenceNumber;
//    return (int32_t)(mSequenceNumber-tcpSequenceNumber.mSequenceNumber) < 0;;
}

bool TcpSequenceNumber::operator <=(const TcpSequenceNumber &tcpSequenceNumber) const
{
    return *this <= tcpSequenceNumber.mSequenceNumber;
}

bool TcpSequenceNumber::operator >=(const TcpSequenceNumber &tcpSequenceNumber) const
{
    return *this >= tcpSequenceNumber.mSequenceNumber;
}

//bool TcpSequenceNumber::isBefore(uint32_t sequenceNumber, uint32_t anotherSequenceNumber) {
////    if(start > end)
////        return ((sequenceNumber >= end && anotherSequenceNumber < end) // if only one before zero.
////                || (((sequenceNumber >= end && anotherSequenceNumber >= end)  // If they both before 0.
////                    || (sequenceNumber < end && anotherSequenceNumber < end)) // if they both after or equal to 0.
////                    && sequenceNumber < anotherSequenceNumber)); // Check if it really before.
////    else
////        return sequenceNumber < anotherSequenceNumber;

////    return ((sequenceNumber-anotherSequenceNumber) & (1u << 31u)) != 0;


//}

} // namespace Tcp
} // namespace ProtocolLearn

