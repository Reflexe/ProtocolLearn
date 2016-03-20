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

#include "TcpFilter.h"

namespace ProtocolLearn {
namespace Tcp {

TcpFilter::TcpFilter(TcpFilter::TcpState initialState)
{
    mTcpSession.tcpState = initialState;
}

const TcpFilter::TcpSession &TcpFilter::getSession() const
{
    return mTcpSession;
}

TcpFilter::TcpSession &TcpFilter::getSession()
{
    return mTcpSession;
}

void TcpFilter::setTcpState(TcpFilter::TcpState state) {
    mTcpSession.tcpState = state;

    pl_debug("Tcp State changed to: " << state);

    if(state == Listen) {
        auto ourPort = mTcpSession.our.port;
        mTcpSession.our.port = ourPort;
    }
}

bool TcpFilter::isValidSequenceNumber(const TcpFilter::TcpTransmissionControlBlock &TCB, uint32_t sequenceNumber,
                                      OctetVector::SizeType segementLength) {
    /// @see RFC 793, page 26.
    if(segementLength != 0) {
        if(TCB.window.isZeroWindow())
            return false;
        else
            return TCB.window.isInWindow(sequenceNumber) && TCB.window.isInWindow(static_cast<uint32_t>(sequenceNumber+segementLength));
    } else {
        if(TCB.window.isZeroWindow())
            return sequenceNumber == TCB.window.getNextSequence();
        else
            return TCB.window.isInWindow(sequenceNumber);
    }
}

bool TcpFilter::isValidAcknowledmentNumber(const TcpFilter::TcpTransmissionControlBlock &TCB, uint32_t acknowledgmentNumber)
{
    return TCB.window.isInAcknowledgmentWindow(acknowledgmentNumber);
}

void TcpFilter::updateWindowSize(uint16_t packetWindowSize, TcpFilter::TcpTransmissionControlBlock &TCB)
{
    TCB.window.updateWindow(static_cast<uint32_t>(packetWindowSize) << TCB.windowShift);
}

void TcpFilter::updateLastAcknowledgment(uint32_t acknowledgmentNumber, TcpFilter::TcpTransmissionControlBlock &TCB)
{
    TCB.window.onAcknowledgmentReceived(acknowledgmentNumber);
}

void TcpFilter::initTCB(const TcpPacket &packet, TcpFilter::TcpTransmissionControlBlock &TCB,
                        TcpFilter::TcpTransmissionControlBlock &anotherTCB) {
    TCB.port = packet.getSourcePort();

    TCB.window.initSequence(packet.getSequenceNumber(), static_cast<uint32_t>(packet.getSegementLength()));
    anotherTCB.windowShift = 0;
    anotherTCB.window.initWindow(packet.getWindowSize());
}

void TcpFilter::initWindowShift(const TcpPacket &packet, TcpTransmissionControlBlock &TCB, TcpTransmissionControlBlock &anotherTCB,
                                bool isSecond) {
    if((isSecond && anotherTCB.windowShift == 0)
            || packet.getParser().isOptionExist(TcpOptionID::WindowScale) == false
            || packet.getParser().getOption(TcpOptionID::WindowScale).data.size() != sizeof(uint8_t)) {
        TCB.windowShift = 0;

        if(isSecond)
            anotherTCB.windowShift = 0;
    } else {
        // Implomention of the Window Scale option (RFC1323)
        TCB.windowShift = packet.getParser().getOption<uint8_t>(TcpOptionID::WindowScale);
    }

    updateWindowSize(static_cast<uint16_t>(TCB.window.getWindowSize()), TCB);
    if(isSecond)
        updateWindowSize(static_cast<uint16_t>(anotherTCB.window.getWindowSize()), anotherTCB);
}

TcpFilter::DropReasonType TcpFilter::successUpdate(uint32_t sequenceNumber, uint32_t acknowledgmentNumber,
                                                        uint16_t windowSize, uint32_t segementLength) {
    // We'll check if the received sequenceNumber is greather than the previous ones.
    // In order to do that, we'll check if ACK > lastACK (if this is a new ACK).
    // And we'll check if there's a hole in the sequences, if there is,
    // we'll check if our one is greater than the greatest one in sequenceRanges.
    // If there's not, our one is probably a new one.
    auto tcpSequenceNumber = mTcpSession.your.window.createSequenceNumber(sequenceNumber);
    if(acknowledgmentNumber != mTcpSession.our.window.getLastAcknowledgment()
            || mTcpSession.sequenceRanges.empty()
            || tcpSequenceNumber > mTcpSession.sequenceRanges.back()) {
        updateWindowSize(windowSize, mTcpSession.our);
    }

    if(sequenceNumber != mTcpSession.your.window.getNextSequence()) {
        mTcpSession.sequenceRanges.onUnsequencalSequenceReceived(tcpSequenceNumber, segementLength);
    } else {
        mTcpSession.your.window.onPacketReceivedInWindow(segementLength);

        mTcpSession.sequenceRanges.onSequencalSequenceReceived(sequenceNumber, segementLength, mTcpSession.your.window);
    }

    mTcpSession.our.window.onAcknowledgmentReceived(acknowledgmentNumber);

    return success();
}

/**
 * @brief TcpFilter::checkByPreviousPacket
 * @param filteredPacket
 * @return
 *
 * The Tcp Handshake:
 *  > Moshe!
 *  > Did you called me?
 *  > Yes!
 *
 *  > Client: Syn.
 *  > Server: Syn/Ack.
 *  > Client: Ack.
 *
 * R=receive
 * S=send
 *
 * Passive close:
 *  > R: Close it! (Finish) -> (FinishReceived)
 *  > S: Got it. (ACK) -> (CloseWait)]
 *  (Application calls close)
 *  > S: I'm closing! (Finish) -> (LastAck)
 *  > R: Got it. (ACK) -> (Closed)
 *
 *  Active Close:
 *  > S: Close it! (Finish) -> (FinishWait1)
 *  > R: Got it (ACK) -> (FinishWait2)
 *  > R: I'm closing! -> (FinishReceived)
 *  > S: Got it (ACK) -> Timeout -> (Closed)
 */
TcpFilter::DropReasonType TcpFilter::checkByPreviousPacket(const TcpPacket &filteredPacket) {
    if(filteredPacket.getDestinationPort() != mTcpSession.our.port) {
        pl_debug(filteredPacket.getDestinationPort());
        return InvalidDestinationPort;
    }

    switch (mTcpSession.tcpState) {
    case Closed:
        return StreamClosed; // Our session is closed, we don't allow anything to get in.
    case Listen:
        if(filteredPacket.getResetFlag())
            return ResetNotExcpcted;
        if(filteredPacket.getAcknowledgmentFlag())
            return SynRequired;

        // Server received a Syn packet (new connection).
        if(filteredPacket.getSynFlag() == false)
            return SynRequired;

        setTcpState(SynReceived);

        initTCB(filteredPacket, mTcpSession.your, mTcpSession.our);
        initWindowShift(filteredPacket, mTcpSession.our, mTcpSession.your, false);

        return success();
    case SynSent:
        // Client got the Syn\Ack packet.
        if(filteredPacket.getSourcePort() != mTcpSession.your.port)
            return InvalidSourcePort;
        if(filteredPacket.getAcknowledgmentFlag() == false)
            return AcknowledgmentRqeuired;
        if(isValidAcknowledmentNumber(mTcpSession.our, filteredPacket.getAcknowledgmentNumber()) == false)
            return InvalidAcknowledgment;

        if(mTcpSession.our.window.willBeSynced(filteredPacket.getAcknowledgmentNumber()) == false)
            return SyncedAcknowledgmentRequired;

        if(filteredPacket.getResetFlag() == true) {
            setTcpState(Closed);

            return success();
        }

        updateLastAcknowledgment(filteredPacket.getAcknowledgmentNumber(), mTcpSession.our);
        if(filteredPacket.getSynFlag() == false)
            return success();

        initTCB(filteredPacket, mTcpSession.your, mTcpSession.our);
        initWindowShift(filteredPacket, mTcpSession.our, mTcpSession.your, true);

        return success();
    default:
        break;
    }

    // Save some resources and get all the variables here.
    auto acknowledgmentNumber = filteredPacket.getAcknowledgmentNumber();
    auto sequenceNumber = filteredPacket.getSequenceNumber();
    auto segementLength = filteredPacket.getSegementLength();

    if(filteredPacket.getSourcePort() != mTcpSession.your.port)
        return InvalidSourcePort;
    if(filteredPacket.getAcknowledgmentFlag() == false)
        return AcknowledgmentRqeuired;
    if(isValidAcknowledmentNumber(mTcpSession.our, acknowledgmentNumber) == false)
        return InvalidAcknowledgment;

    // This is not actualy what the standrart says about SynReceived (setTcpState(Listen)), but this is a new stream.
    // @see Network Notes:4 for more information about the second part of the condition.
    if(filteredPacket.getResetFlag() && segementLength == 0) {
        setTcpState(Closed);
        return success();
    }

    if(isValidSequenceNumber(mTcpSession.your, sequenceNumber, segementLength) == false)
        return InvalidSequence;

    switch (mTcpSession.tcpState) {
    case SynReceived:
        // Server got the ack from the client (part 3 of the tcp-handshake).
        setTcpState(Established);

        goto SuccessUpdate;
    case Established:   
        // If we are the starting a finish as a passive side.
        if(filteredPacket.getFinishFlag())
            setTcpState(CloseWait);

        goto SuccessUpdate;
    case CloseWait:
        // On CloseWait, we still have the option to send data (and receive acknowledgments)
        if(filteredPacket.isDataPacket() == true)
            return NoDataRequired;
        else
            goto SuccessUpdate;

    case FinishWait1:
        // If this is packet is both finish and ack combined, skip the FinishWait2 (Which waits for FIN).
        if(filteredPacket.getFinishFlag()) {
            if(mTcpSession.our.window.willBeSynced(acknowledgmentNumber))
                setTcpState(TimeWait); // If the other side ACKed our FIN packet and send us a FIN packet.
            else
                setTcpState(Closing); // If the other side send us a FIN packet (without ACKing our FIN).

            goto SuccessUpdate;
        } else if(mTcpSession.our.window.willBeSynced(acknowledgmentNumber)) {
            setTcpState(FinishWait2); // If the other side just ACKed our FIN.
            goto SuccessUpdate;
        } else {
            return FinishRequired;
        }

    case FinishWait2:
        // We got the finish from the passive side. In finish wait 2 we also should receive packet.
        if(filteredPacket.getFinishFlag())
            setTcpState(TimeWait);

        goto SuccessUpdate;

    case LastAck:
        // Client got the last ACK.
        if(mTcpSession.our.window.willBeSynced(acknowledgmentNumber)) {
            setTcpState(Closed);
            goto SuccessUpdate;
        } else {
            return SyncedAcknowledgmentRequired;
        }

    case Closing:
        if(mTcpSession.our.window.willBeSynced(acknowledgmentNumber)) {
            setTcpState(TimeWait);

            goto SuccessUpdate;
        } else {
            return SyncedAcknowledgmentRequired;
        }

    case TimeWait:
        return StreamClosedForRead;

    default:
        pl_debug("** Default reached, Tcp state: " << mTcpSession.tcpState);

        pl_assert(false);
        break;
    }

    return DropReason::NoReason;

SuccessUpdate:
    return successUpdate(sequenceNumber,
                         acknowledgmentNumber,
                         filteredPacket.getWindowSize(),
                         static_cast<uint32_t>(segementLength));
}

TcpFilter::DropReasonType TcpFilter::checkByProtocol(const TcpPacket &filteredPacket) {
    if(filteredPacket.isValidPacket() == false)
        return InvalidOptions;
    if(filteredPacket.isChecksumValid() == false)
        return InvalidChecksum;
    if(filteredPacket.getParser().isValidOptions() == false)
        return InvalidOptions;

    return DropReason::None;
}

void TcpFilter::filterByPacket(const TcpPacket &filteringPacket) {
    // Special Cases that requires a TCB initialization.
    switch (mTcpSession.tcpState) {
    case Closed:
        // Client opens a new connection.
        if(filteringPacket.getSynFlag()) {
            setTcpState(SynSent);

            mTcpSession.your.port = filteringPacket.getDestinationPort();

            initTCB(filteringPacket, mTcpSession.our, mTcpSession.your);
            initWindowShift(filteringPacket, mTcpSession.your, mTcpSession.our, false);

            return;
        }

        return;
    case SynReceived:
        // Server sends the SYN\ACK
        if(filteringPacket.getSynFlag() && filteringPacket.getAcknowledgmentFlag()) {
            updateLastAcknowledgment(filteringPacket.getAcknowledgmentNumber(), mTcpSession.your);

            initTCB(filteringPacket, mTcpSession.our, mTcpSession.your);
            initWindowShift(filteringPacket, mTcpSession.your, mTcpSession.our, true);

            return;
        }

        break;

    default:
        break;
    }

    pl_assert(filteringPacket.getAcknowledgmentFlag() == false
              || isValidAcknowledmentNumber(mTcpSession.your, filteringPacket.getAcknowledgmentNumber()));

    switch(mTcpSession.tcpState) {
    case SynSent:
        // Client sends Ack (step three of the tcp handshake)
        if(filteringPacket.getAcknowledgmentFlag()) {
            setTcpState(Established);
        }

        break;
    case Established:
        // Finish sent, Goodbyte! :)
        if(filteringPacket.getFinishFlag() && mTcpSession.our.window.isSynced()) {
            setTcpState(FinishWait1);
        }

        break;

    case CloseWait:
        // The passive is sending the last FIN.
        if(filteringPacket.getFinishFlag())
            setTcpState(LastAck);

        break;
    case LastAck:
        // Retransmition baby!
        break;

    case FinishWait2:
        // You can't send data on FinishWait2.
        pl_assert(filteringPacket.isDataPacket() == false);
        break;
    case TimeWait:
        // Active side acks the FIN from the passive side.
        pl_assert(filteringPacket.getAcknowledgmentFlag()
                  && mTcpSession.our.window.isSynced());
        break;
    default:
        pl_debug("** Default reached, Tcp state: " << mTcpSession.tcpState);

        pl_assert(false);

        break;
    }

    if(filteringPacket.getSequenceNumber() == mTcpSession.our.window.getNextSequence())
        mTcpSession.our.window.onPacketReceivedInWindow(static_cast<uint32_t>(filteringPacket.getSegementLength()));

    if(filteringPacket.getAcknowledgmentFlag())
        updateLastAcknowledgment(filteringPacket.getAcknowledgmentNumber(), mTcpSession.your);

    updateWindowSize(filteringPacket.getWindowSize(), mTcpSession.your);
}

uint32_t TcpFilter::TcpSession::SequenceRanges::onSequencalSequenceReceived(uint32_t sequenceNumber, OctetVector::SizeType segementSize,
                                                                            TcpSequenceNumber::TcpSequenceWindow &tcpSequenceWindow) {
    uint32_t nextSequenceNumber = sequenceNumber+static_cast<uint32_t>(segementSize);
    auto iterator = mSequenceRangesMap.begin();

    while(iterator != mSequenceRangesMap.end()) {
        if(iterator->first != nextSequenceNumber)
            break;

        tcpSequenceWindow.onPacketReceivedInWindow(static_cast<uint32_t>(iterator->second));
        nextSequenceNumber += iterator->second;
        iterator = mSequenceRangesMap.erase(iterator);
    }

    return nextSequenceNumber;
}

void TcpFilter::TcpSession::SequenceRanges::onUnsequencalSequenceReceived(const TcpSequenceNumber &tcpSequenceNumber, OctetVector::SizeType segmentSize)
{
    mSequenceRangesMap[tcpSequenceNumber] = segmentSize;
}

}
}
