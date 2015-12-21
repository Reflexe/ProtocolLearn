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

#ifndef TCPDATASTREAM_H
#define TCPDATASTREAM_H

#include <limits>
#include <list>
#include <map>

#include "DataStreamUnderPacketStream.h"
#include "Random.h"

#include "TransportProtocol.h"
#include "TcpStream.h"

namespace ProtocolLearn {
namespace Tcp {

/**
 * @brief An implomation of the Transmission Constrol Protocol (TCP).
 * @see [RFC 793](https://tools.ietf.org/html/rfc793).
 *
 * Implomention Progress (v: implomented; ?: partical or unknown; x: not implometed).
 *   Connection Establishing and closing:
 *     v connect(): Create a new connection.
 *     v completeAccept(): Answer a creation request (SYN).
 *     v close(): Close the stream.
 *   Sending and receiving of data:
 *     ? Window Size: Updated but static number; the newest packet will update the window; use ipv4 path MTU discovery for that.
 *     v Fragmention (sendDataPacket).
 *     v Reassembling (ReceiveQueue and _recv(), receivePacket()).
 *     v Retransmition (RetrasmitionQueue and sync(), retransmitPackets()).
 *     v Receiving of data sending ACKSs (waitForPacket()).
 *     v Receive Size Filter (receiveData()).
 *     v Handling no place in the Window (sendDataPacket()).
 *     x Urgent and all that shit.
 *   Other:
 *     v Drop Handler (onPacketDropped()).
 *     ? The Window Scale option (RFC1323): still need to imploment this in TcpDataStream.
 *     x MSS and all RFC1323 things.
 *     x Syn Cookies.
 */
class TcpDataStream : public DataStreamUnderPacketStream<TcpStream, TransportProtocol>
{
public:
    struct NotConnectedStream : RuntimeError
    {
        using RuntimeError::RuntimeError;
    };

    static const uint32_t TEMP_WINDOW_SIZE = 0xffff;

private:
    TcpDataStream(TcpStream &tcpStream, uint16_t destinationPort, uint16_t sourcePort);

    void onStreamConnected();

public:
    TcpDataStream(TcpDataStream &&tcpDataStream);
    TcpDataStream &operator =(TcpDataStream &&tcpDataStream) = delete;

    static TcpDataStream connect(TcpStream &tcpStream,
                                 uint16_t destinationPort,
                                 uint16_t sourcePort=Random::getMediumRandomNumber());
    /**
     * @brief Takes a TcpStream in SynRecieved and return a TcpDataStream when it in Established.
     * @param tcpStream
     * @return
     */
    static TcpDataStream completeAccept(TcpStream &tcpStream, TcpFilter::TcpSession &tcpSession);

    /**
     * @brief Receive data with the data length filter.
     * @param data
     *
     * By default, we're dropping packet without the required length.
     * With TCP, we should wait until we receive enough data.
     */
    virtual void receiveData(OctetVector &data) override;

    /**
     * @brief Send data in the TCP.
     * @param data
     * @throw NotConnectedStream when the stream is not open for writing.
     *
     * This function checks if the window is okay, if not, it sync() it.
     * After that, the function sends the data and put it in the the
     * retransmit queue.
     */
    virtual void _send(const OctetVector &data) override;

    /**
     * @brief Receive a data in the TCP protocol.
     * @param data
     * @throw NotConnectedStream when the stream is not open for reading.
     *
     * This function will block and wait until a data packet will that the stream will close for reading
     * (by receiving of FIN or RST packet). The function will send ack for the packets it receives, even
     * if they're FIN or RST packets).
     */
    virtual void _recv(OctetVector &data) override;

    /**
     * @brief Set a timeout for the general operations like _recv(), _send() and sync().
     * @param seconds
     * @param useconds
     */
    virtual void setTimeout(const Timeout::TimeType &time) override;

    virtual void setMinimumReceiveDataSize(OctetVector::SizeType minimumDataSize) override;

    /**
     * @brief Sync the send window.
     * @param sendAcks  Send acknowledgments?
     * @throw Timeout::TimeoutException when we didn't got a response in the time.
     *
     * Receive ACKs for all the packet sent. If no ACK was received, send the packet again.
     * After calling this function you can be sure that your packet sent (as long as you don't
     * get an exception)
     */
    void sync(bool sendACKs=true);

    /**
     * @brief Sync and close the TCP stream.
     * @note Some data may received in this function.
     *
     * Sync the receive and the send queues, send a FIN packet and wait for response.
     */
    void close();

    /**
     * @brief Checking if the stream is in Established mode.
     * @return
     *
     * This function just checks if the stream is in Established
     * state. The stream may be in CloseWait (which allowing sending)
     * and this function will return false.
     */
    bool isConnected() const{ return getTcpState() == TcpFilter::Established; }

    virtual OctetVector::SizeType getMaximumSendDataLength() override;

private:
    enum class TcpPacketType{
        DataPacket,
        FinishPacket,
        SycedAck, // receive only
        SynAckPacket, // send only
        SynPacket
    };

//    OctetVector::SizeType getMaximumTransmissionUnit();

    void retransmitPackets();

    void updateDropHandler();
    void onPacketDropped(const TcpPacket &packet, TcpFilter::DropReasonType dropReason);

    void checkIfStreamWriteConnected(const char functionName[]);
    void checkIfStreamReadConnected(const char functionName[]);

    void initPorts(uint16_t destinationPort, uint16_t sourcePort);
    void updateSendPacket(bool acknowledgmentFlag, uint32_t sequenceNumber);
    void updateSendPacket(bool acknowledgmentFlag);

    void sendAcknowledment();
    void sendReset(const TcpPacket &tcpPacket);

    void sendFragment(const OctetVector &octetVector, uint32_t sequenceNumber);
    void sendFragmentInWindow(const OctetVector &octetVector);

    void sendPacket(TcpPacketType packetType, uint32_t sequenceNumber);
    void sendPacketInWindow(TcpPacketType packetType, uint32_t sequenceNumber);

    bool waitForPacket(TcpPacketType packetToReceive, bool sendACKs=true);

    TcpFilter::TcpTransmissionControlBlock &getOurTCB();
    TcpFilter::TcpTransmissionControlBlock &getYourTCB();
    TcpFilter::TcpState getTcpState() const;

    /**
     * @brief Input data and output it.
     *
     * @note You *must* make sure that difference between the oldest sequence number and the newest sequence number
     *       isn't greather than 2**32 (i.e. our window is not greather than that).
     */
    class ReceiveQueue
    {
    public:
        ReceiveQueue();

        /**
         * @brief Push a data to the receive queue.
         * @param octetVector
         * @param sequenceNumber
         *
         * Runtime efficiency: O(log n).
         */
        void push(OctetVector &&octetVector, const TcpSequenceNumber &tcpSequenceNumber);

        /**
         * @brief Fill @arg octetVector with the data space that recived (ordered by sequence number).
         * @param octetVector
         * @note Unexcpected results if the queue is empty().
         */
        void pop_front(OctetVector &octetVector);

        /**
         * @brief Check if the queue is empty.
         * @return
         *
         * This function checks if we didn't got a sequential data.
         */
        bool empty() const;

        void setNextSequenceNumber(const uint32_t &nextSequenceNumber);
        uint32_t getNextSequenceNumber();

    private:
        typedef std::map<TcpSequenceNumber, OctetVector> MapType;

        void addToReceiveBuffer(OctetVector &&octetVector);
        void mergeReceiveMapToReceiveBuffer();

        uint32_t mNextSequenceNumber = 0;

        MapType mReceiveMap;
        OctetVector mReceiveBuffer;
    };

    /**
     * @brief The received datas, filled in waitForPacket() and cleared in _recv() and receivePacket().
     */
    ReceiveQueue mReceiveQueue;

    /**
     * @brief A class for the retransmit queue.
     *
     * The retransmit queue is a s case: we need it to be ordered by the send time to resend them
     * and remove elements by the acknowledgment number we received.
     *
     * This class solves this problem by a map that contains iterators into the queue and ordered by
     * acknowledgment number. When we get a acknowledgment, we just look it up on the map and remove
     * it from the retransmit queue (and the lower acknowlegments as well).
     */
    class RetransmitQueue
    {
    public:
        struct SavedData{
            /**
             * @brief The type of the packet.
             */
            TcpPacketType tcpPacketType;

            /**
             * @brief The excpected acknowledgment number for this packet.
             */
            TcpSequenceNumber expectedAcknowledgmentNumber;

            /**
             * @brief The packet's data, may be empty.
             */
            OctetVector data;

            /**
             * @brief The time the retransmit timeout will pass.
             */
            Timeout timeout;
        };

        RetransmitQueue();

        void push(SavedData &&savedData);

        /**
         * @brief Get the oldest data in the queue.
         * @return
         *
         * Runtime efficiency: O(1).
         */
        SavedData &front();

        /**
         * @brief Check if the queue is empty.
         * @return
         *
         * Runtime efficiency: O(1).
         */
        bool empty() const;

        /**
         * @brief Pop the oldest data in the queue.
         *
         * Runtime efficiency: O(log n) (red-black tree delete).
         */
        void pop();

        /**
         * @brief Move the front() data to the end.
         *
         * Instead pop() -ing and push() -ing again we save some resources and just moving the datas back.
         */
        void move_front_back();

        /**
         * @brief Delete all the acknowledgment numbers that lower than @arg acknowlegmentNumber.
         * @param acknowlegmentNumber
         *
         * Runtime efficiency: O(log(mAcknowledgmentsMap.size()) + deletedACKs) (red-black tree search).
         */
        void deleteAcknowledgmentNumber(const TcpSequenceNumber &tcpAcknowlegmentNumber);

    private:
        typedef std::list<SavedData> QueueType;
        typedef std::map<TcpSequenceNumber, QueueType::const_iterator> MapType;

        /// Map the acknowlegments to elemets in mRetransmitQueue.
        MapType mAcknowledgmentsMap;

        /// Ordred by the timeout.
        QueueType mRetransmitQueue;
    };

    /**
     * @brief The sent datas which still don't ACKed.
     */
    RetransmitQueue mRetransmitQueue;

    Timeout::TimeType mRetransmitionTimeout{1, 0};
};

}
}

#endif // TCPDATASTREAM_H
