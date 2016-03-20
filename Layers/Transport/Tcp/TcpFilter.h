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

#ifndef TCPFILTER_H
#define TCPFILTER_H

#include <map>

#include "ProtocolFilter.h"

#include "TcpPacket.h"
#include "TcpSequenceNumber.h"

namespace ProtocolLearn {
namespace Tcp {

class TcpFilter : public ProtocolFilter<TcpPacket>
{
public:
    enum TcpState {
        Listen,
        SynSent,
        SynReceived,
        Established,
        FinishWait1,
        FinishWait2,
        CloseWait,
        Closing,
        LastAck,
        TimeWait,
        Closed
    };

    /**
     * @brief The reason for packet drop.
     */
    enum ProtocolDropReason{
        // The Protocol Filter reasons.
        InvalidChecksum,
        InvalidPacket,
        InvalidOptions,

        // The Previous Packet Filter reasons.
        /// Invalid source port.
        InvalidSourcePort,
        /// Invalid destination port.
        InvalidDestinationPort,
        /// The stream is closed.
        StreamClosed,
        /// Stream closed for reading.
        StreamClosedForRead,
        /// Invalid sequence field.
        InvalidSequence,
        /// invalid acknowledgment number field.
        InvalidAcknowledgment,
        /// the packet isn't acknowledgment.
        AcknowledgmentRqeuired,
        /// Only full acknowledgment accepted.
        SyncedAcknowledgmentRequired,
        /// The syn flag isn't on.
        SynRequired,
        /// The finish flag isn't on.
        FinishRequired,
        /// Only data packet required.
        DataRequired,
        /// Only non-data packet required.
        NoDataRequired,
        ResetNotExcpcted
    };


    explicit TcpFilter(TcpState initialState=Closed);

    struct TcpTransmissionControlBlock {
        /**
         * @brief The source port in the packets.
         */
        uint16_t port = 0;

        TcpSequenceNumber::TcpSequenceWindow window;
        uint8_t windowShift = 0;
    };

    struct TcpSession{
        TcpState tcpState = Closed;

        class SequenceRanges {
        public:
            const TcpSequenceNumber &front() { return mSequenceRangesMap.begin()->first; }
            const TcpSequenceNumber &back() { return (--mSequenceRangesMap.end())->first; }

            bool empty() { return mSequenceRangesMap.empty(); }

            uint32_t onSequencalSequenceReceived(uint32_t sequenceNumber, OctetVector::SizeType segementSize, TcpSequenceNumber::TcpSequenceWindow &tcpSequenceWindow);
            void onUnsequencalSequenceReceived(const TcpSequenceNumber &tcpSequenceNumber, OctetVector::SizeType segmentSize);
        private:
            std::map<TcpSequenceNumber, OctetVector::SizeType> mSequenceRangesMap;
        } sequenceRanges;

        TcpTransmissionControlBlock our;
        TcpTransmissionControlBlock your;
    };

    const TcpSession &getSession() const;
    TcpSession &getSession();
    void setTcpState(TcpState state);

    virtual void filterByPacket(const TcpPacket &filteringPacket) override final;

private:
    static bool isValidSequenceNumber(const TcpTransmissionControlBlock &TCB, uint32_t sequenceNumber, OctetVector::SizeType segementLength);
    static bool isValidAcknowledmentNumber(const TcpTransmissionControlBlock &TCB, uint32_t acknowledgmentNumber);
    static void updateWindowSize(uint16_t packetWindowSize, TcpTransmissionControlBlock &TCB);

    void updateLastAcknowledgment(uint32_t acknowledgmentNumber, TcpTransmissionControlBlock &TCB);
    void initTCB(const TcpPacket &packet, TcpTransmissionControlBlock &TCB, TcpTransmissionControlBlock &anotherTCB);
    static void initWindowShift(const TcpPacket &packet, TcpTransmissionControlBlock &TCB, TcpTransmissionControlBlock &anotherTCB,
                                bool isSecond);
    DropReasonType success() const
    {
        return DropReason::None;
    }

    DropReasonType successUpdate(uint32_t sequenceNumber, uint32_t acknowledgmentNumber,
                                 uint16_t windowSize, uint32_t segementLength);

    TcpSession mTcpSession;

protected:
    virtual DropReasonType checkByPreviousPacket(const TcpPacket &filteredPacket) override final;

    virtual DropReasonType checkByProtocol(const TcpPacket &filteredPacket) override final;
};

} // Tcp
} // ProtocolLearn

#endif // TCPFILTER_H
