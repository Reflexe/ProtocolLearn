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

#include "TcpDataStream.h"

#include <algorithm> // For std::min.
#include <functional>

namespace ProtocolLearn {
namespace Tcp {

TcpDataStream TcpDataStream::connect(TcpStream &tcpStream, uint16_t destinationPort, uint16_t sourcePort) {
    tcpStream.setTcpState(TcpFilter::Closed);
    TcpDataStream tcpDataStream{tcpStream, destinationPort, sourcePort};

    // Send the syn.
    tcpDataStream.sendPacketInWindow(TcpPacketType::SynPacket, static_cast<uint32_t>(Random::getMediumRandomNumber()));
    tcpDataStream.sync(); // Receive the syn/ack and ack it (maybe retransmit the syn as well).

    // You might send only an ACK, wait for an syn.
    if(tcpDataStream.getTcpState() == TcpFilter::SynSent) {
        if(tcpDataStream.waitForPacket(TcpPacketType::SynPacket) == false)
            throw Timeout::TimeoutException{"TcpDataStream::tcpConnect"};
    }

    tcpDataStream.onStreamConnected();

    pl_debug("Connected!");

    return tcpDataStream;
}

TcpDataStream TcpDataStream::completeAccept(TcpStream &tcpStream, TcpFilter::TcpSession &tcpSession) {
    tcpStream.getFilter().getSession() = tcpSession;
    pl_assert(tcpStream.getTcpSession().tcpState == TcpFilter::SynReceived);

    TcpDataStream tcpDataStream{tcpStream, tcpSession.your.port, tcpSession.our.port};

    tcpDataStream.sendPacketInWindow(TcpPacketType::SynAckPacket, static_cast<uint32_t>(Random::getMediumRandomNumber()));
    tcpDataStream.sync();

    pl_assert(tcpDataStream.getTcpState() == TcpFilter::Established);

    tcpDataStream.onStreamConnected();

    return tcpDataStream;
}

TcpDataStream::TcpDataStream(TcpStream &tcpStream, uint16_t destinationPort, uint16_t sourcePort)
    : DataStreamUnderPacketStream{tcpStream},
      mReceiveQueue{},
      mRetransmitQueue{} {

    updateDropHandler();

    getPacketStream().setTimeout(PTime{0, 70000});
    setTimeout(PTime{2, 0});

    getYourTCB().window.initWindow(TEMP_WINDOW_SIZE);
    getYourTCB().windowShift = 0;
    initPorts(destinationPort, sourcePort);
}

void TcpDataStream::onStreamConnected()
{
    mReceiveQueue.setNextSequenceNumber(getYourTCB().window.getNextSequence());
}

TcpDataStream::TcpDataStream(TcpDataStream &&tcpDataStream)
    : DataStreamUnderPacketStream{std::move(tcpDataStream)},
      mReceiveQueue{std::move(tcpDataStream.mReceiveQueue)},
      mRetransmitQueue{std::move(tcpDataStream.mRetransmitQueue)},
      mRetransmitionTimeout{std::move(tcpDataStream.mRetransmitionTimeout)}
{
    updateDropHandler(); // Yes. that's all.
}

void TcpDataStream::receiveData(OctetVector &data) {
    if(mMinimumReceiveDataSize == 0)
        return _recv(data);

    Timeout timeout{getTimeout(), true};
    data.clear();
    data.reserve(mMinimumReceiveDataSize);

    do {
        if(timeout.isPassed())
            throw Timeout::TimeoutException{"TcpDataStream::receiveData"};

        if(mReceiveQueue.empty()) {
            checkIfStreamReadConnected("TcpDataStream::receiveData");
            waitForPacket(TcpPacketType::DataPacket);
        } else {
            if(data.empty()) {
                mReceiveQueue.pop_front(data);
                getYourTCB().window.expendWindow(data.size());
            } else {
                OctetVector buffer;
                mReceiveQueue.pop_front(buffer);
                getYourTCB().window.expendWindow(buffer.size());

                data.add(buffer);
            }
        }
    } while(data.size() < mMinimumReceiveDataSize);
}

void TcpDataStream::_send(const OctetVector &data){
    if(data.empty())
        return;

    // Handle zero windows by just throwing NotConnectedStream.
    checkIfStreamWriteConnected("TcpDataStream::_send");

    // Split large datas.
    if(data.size() > getMaximumSendDataLength()) {
        auto nextOctetToSend = data.cbegin();
        auto pastLastOctetToSend = nextOctetToSend + getMaximumSendDataLength();

        do {
            sendFragmentInWindow(OctetVector{nextOctetToSend, pastLastOctetToSend});

            // Don't send acknowledgments if this is not the last packet in order to save resources
            // (we'll send the acknowledgment in the data packet).
            if(pastLastOctetToSend != data.cend())
                sync(false);
            else
                sync(true);

            nextOctetToSend = pastLastOctetToSend;
            pastLastOctetToSend += getMaximumSendDataLength();
            if(pastLastOctetToSend > data.cend())
                pastLastOctetToSend = data.cend();
        } while(nextOctetToSend < data.cend());

    } else {
        sendFragmentInWindow(data);
    }
}

void TcpDataStream::_recv(OctetVector &data) {
    if(mReceiveQueue.empty()) {
        Timeout timeout{getTimeout(), true};

        while(waitForPacket(TcpPacketType::DataPacket) == false || mReceiveQueue.empty()) {
            if(timeout.isPassed())
                throw Timeout::TimeoutException{"TcpDataStream::_recv"};

            checkIfStreamReadConnected("TcpDataStream::_recv");
        }
    }

    pl_assert(mReceiveQueue.empty() == false);
    mReceiveQueue.pop_front(data);
    getYourTCB().window.expendWindow(static_cast<uint32_t>(data.size()));
}

void TcpDataStream::setTimeout(const Timeout::TimeType &time)
{
    return DataStreamUnderPacketStream::setTimeout(time);
}

void TcpDataStream::setMinimumReceiveDataSize(OctetVector::SizeType minimumDataSize)
{   // Skip the DataStreamUnderPacketStream's overriden function: we want the packets even if they don't have any data.
    return DataStream::setMinimumReceiveDataSize(minimumDataSize);
}

void TcpDataStream::sync(bool sendACKs) {
    // If we're already synced.
    if(getOurTCB().window.isSynced())
        return;

    Timeout timeout{getTimeout(), true};

    while(waitForPacket(TcpPacketType::SycedAck, sendACKs) == false) {
        if(timeout.isPassed() == true)
            throw Timeout::TimeoutException{"TcpDataStream::sync"};

        pl_assert(mRetransmitQueue.empty() == false);

        retransmitPackets();
    }
}

void TcpDataStream::close() {
    if(getTcpState() == TcpFilter::Closed)
        return;

    // First of all, sync:
    sync(false); // The send queue.

    getPacketStream().setTimeout(PTime::zero());
    waitForPacket(TcpPacketType::DataPacket, false); // The receive queue, we'll ack this packets in the FIN packet.
    getPacketStream().setTimeout(PTime{0, 70000});

    // The state may change after calling sync() and waitForPacket().
    if(getTcpState() == TcpFilter::Closed)
        return;

    /*
     * Anyway, we sould send a finish and receive a packet.
     * If we in CloseWait (Passive CLOSE), we should send fin and receive ACK.
     *
     * If we in Established, we should send finish and go to FinishWait1.
     * Then:
     *     If we received a Finish (Two Active CLOSE), we should reply with ACK and receive an ACK (Closing).
     *     If we received a ACK (We're waiting for the other side to call CLOSE) (FinishWait2).
     *     If we received a Finish+ACK, we in the end and we should reply with ACK (TimeWait).
     */

    pl_crap("Sending finish");
    sendPacketInWindow(TcpPacketType::FinishPacket, getOurTCB().window.getNextSequence());

    pl_crap("receiving something");
    sync();

    switch (getTcpState()) {
    case TcpFilter::FinishWait2:
        // Recive fin and ack it.
        getPacketStream().setTimeout(PTime{2, 0});
        if(waitForPacket(TcpPacketType::FinishPacket) == false)
            throw Timeout::TimeoutException{"TcpDataStream::close"};

        pl_assert(getTcpState() == TcpFilter::TimeWait);

        // waitForPacket sent acknowledment for the packet it received.

        break;
    case TcpFilter::TimeWait:
        // sync already sent an acknowledgment.

        break;

    case TcpFilter::Closing:
        // sync already sent an acknowledgment.

        break;
    case TcpFilter::Closed:
        break;
    default:
        pl_assert(false);
        break;
    }
}

OctetVector::SizeType TcpDataStream::getMaximumSendDataLength() {
    return std::min(getPacketStream().getPacketStream().getDataStream().getMaximumSendDataLength()-TcpPacket::MinimumHeaderLength,
                    OctetVector::SizeType{getOurTCB().window.getCurrentWindowSize()});
}

//OctetVector::SizeType TcpDataStream::getMaximumTransmissionUnit()
//{
//    return getPacketStream().getPacketStream().getDataStream().getMaximumSendDataLength();
//}

//void TcpDataStream::updateSendWindow(OctetVector::SizeType segementSize) {
//    pl_assert(segementSize <= getOurTCB().windowSize);
//    getOurTCB().windowSize -= segementSize;
//}

void TcpDataStream::sendFragment(const OctetVector &octetVector, uint32_t sequenceNumber) {
    updateSendPacket(true, sequenceNumber);
    DataStreamUnderPacketStream::_send(octetVector);
}

void TcpDataStream::sendFragmentInWindow(const OctetVector &octetVector) {
    mRetransmitQueue.push(RetransmitQueue::SavedData{TcpPacketType::DataPacket,
                                                     getOurTCB().window.createSequenceNumber(static_cast<uint32_t>(getOurTCB().window.getNextSequence() + octetVector.size())),
                                                     octetVector,
                                                     Timeout{mRetransmitionTimeout, true}});

    sendFragment(octetVector, getOurTCB().window.getBegin());
}

void TcpDataStream::sendPacket(TcpDataStream::TcpPacketType packetType, uint32_t sequenceNumber) {
    switch (packetType) {
    case TcpPacketType::FinishPacket:
        updateSendPacket(true, sequenceNumber);
        getSendPacket().setFinishFlag(true);
        break;
    case TcpPacketType::SynAckPacket:
        updateSendPacket(true, sequenceNumber);
        getSendPacket().setSynFlag(true);
        break;
    case TcpPacketType::SynPacket:
        updateSendPacket(false, sequenceNumber);
        getSendPacket().setSynFlag(true);
        break;
    default:
        pl_assert(false);
        break;
    }

    getPacketStream().sendPacket(getSendPacket());

    // Clean up!
    switch (packetType) {
    case TcpPacketType::FinishPacket:
        getSendPacket().setFinishFlag(false);
        break;
    case TcpPacketType::SynAckPacket:
    case TcpPacketType::SynPacket:
        getSendPacket().setSynFlag(false);
        break;
    default:
        break;
    }
}

void TcpDataStream::sendPacketInWindow(TcpDataStream::TcpPacketType packetType, uint32_t sequenceNumber) {
    sendPacket(packetType, sequenceNumber);

    mRetransmitQueue.push(RetransmitQueue::SavedData{packetType,
                                                     getOurTCB().window.createSequenceNumber(sequenceNumber+1),
                                                     OctetVector{},
                                                     Timeout{mRetransmitionTimeout, true}});
}

void TcpDataStream::retransmitPackets() {
    // The queue oredred by the send time.
    while(mRetransmitQueue.empty() == false && mRetransmitQueue.front().timeout.isPassed()) {
        auto &savedData = mRetransmitQueue.front();

        if(savedData.tcpPacketType == TcpPacketType::DataPacket)
            sendFragment(savedData.data, static_cast<uint32_t>(savedData.expectedAcknowledgmentNumber.to32BitInteger()-savedData.data.size()));
        else
            sendPacket(savedData.tcpPacketType, savedData.expectedAcknowledgmentNumber.to32BitInteger()-1);

        // We sent it again, restart the timer.
        savedData.timeout.start();
        mRetransmitQueue.move_front_back();
    }
}

void TcpDataStream::updateDropHandler() {
    // I HATE to use this!
    getPacketStream().setDropHandler(std::bind(&TcpDataStream::onPacketDropped, this, std::placeholders::_1, std::placeholders::_2));
}

void TcpDataStream::onPacketDropped(const TcpPacket &packet, TcpFilter::DropReasonType dropReason) {
    typedef TcpFilter::ProtocolDropReason ProtocolDropReason;

    pl_debug("Handeling a drop: " << dropReason);

    if(packet.getResetFlag() == true)
        return;

    switch (dropReason) {
    case ProtocolDropReason::InvalidDestinationPort:
    case ProtocolDropReason::InvalidChecksum:
    case ProtocolDropReason::InvalidPacket:
    case ProtocolDropReason::InvalidOptions:
    case ProtocolDropReason::InvalidSourcePort:
        break; // It's not for us.

    case ProtocolDropReason::StreamClosed:
    case ProtocolDropReason::StreamClosedForRead:
        sendReset(packet);

        break;
    case ProtocolDropReason::SynRequired:
    case ProtocolDropReason::InvalidAcknowledgment:
        if(getTcpState() == TcpFilter::Listen || getTcpState() == TcpFilter::SynReceived || getTcpState() == TcpFilter::SynSent) {
            sendReset(packet);
            break;
        } // else, just continue to the next.

    case ProtocolDropReason::InvalidSequence:
    case ProtocolDropReason::AcknowledgmentRqeuired:
    case ProtocolDropReason::SyncedAcknowledgmentRequired:
    case ProtocolDropReason::FinishRequired:
    case ProtocolDropReason::DataRequired:
    case ProtocolDropReason::NoDataRequired:
        updateSendPacket(true);
        getPacketStream()._send(getSendPacket());

        break;
    default:
        pl_assert(false);
        break;
    }
}

void TcpDataStream::checkIfStreamReadConnected(const char functionName[]) {
    if(isConnected() == false && (getTcpState() != TcpFilter::FinishWait2))
        throw NotConnectedStream{functionName};
}

void TcpDataStream::checkIfStreamWriteConnected(const char functionName[]) {
    if(getOurTCB().window.getCurrentWindowSize() == 0 || (isConnected() == false && getTcpState() != TcpFilter::CloseWait))
        throw NotConnectedStream{functionName};
}

void TcpDataStream::initPorts(uint16_t destinationPort, uint16_t sourcePort) {
    auto &sendPacket = DataStreamUnderPacketStream::mSendPacket;
    sendPacket.setSourcePort(sourcePort);
    sendPacket.setDestinationPort(destinationPort);
}


void TcpDataStream::updateSendPacket(bool acknowledgmentFlag, uint32_t sequenceNumber) {
    mSendPacket.setSequenceNumber(sequenceNumber);

//    auto oldestUnreceivedSeqeunceNumber = mReceiveQueue.getOldestSequenceNumber();
//    if(oldestUnreceivedSeqeunceNumber == 0)
//        mSendPacket.setWindowSize(TEMP_WINDOW_SIZE);
//    else {
//        mSendPacket.setWindowSize((TEMP_WINDOW_SIZE - (getYourTCB().window.getNextSequence() - oldestUnreceivedSeqeunceNumber)) >> getOurTCB().windowShift);
//    }

    mSendPacket.setWindowSize(static_cast<uint16_t>(getYourTCB().window.getCurrentWindowSize() >> getYourTCB().windowShift));

    mSendPacket.setAcknowledgmentFlag(acknowledgmentFlag);
    if(acknowledgmentFlag)
        mSendPacket.setAcknowledgmentNumber(getYourTCB().window.getNextSequence());
}

void TcpDataStream::updateSendPacket(bool acknowledgmentFlag)
{
    updateSendPacket(acknowledgmentFlag, getOurTCB().window.getNextSequence());
}

void TcpDataStream::sendAcknowledment() {
    if(getYourTCB().window.isSynced())
        return;

    updateSendPacket(true);
    getPacketStream().sendPacket(getSendPacket());
}

void TcpDataStream::sendReset(const TcpPacket &tcpPacket) {
    getSendPacket().setResetFlag(true);

    if(tcpPacket.getAcknowledgmentFlag()) {
        getSendPacket().setSequenceNumber(tcpPacket.getAcknowledgmentNumber());
        getSendPacket().setAcknowledgmentFlag(false);
    } else {
        getSendPacket().setAcknowledgmentFlag(true);
        getSendPacket().setSequenceNumber(0);
        getSendPacket().setAcknowledgmentNumber(tcpPacket.getSequenceNumber()+tcpPacket.getSegementLength());
    }

    getPacketStream()._send(getSendPacket());
    getSendPacket().setResetFlag(false);
}

bool TcpDataStream::waitForPacket(TcpPacketType packetToReceive, bool sendACKs) {
    TcpPacket receivedPacket;
    auto currentTcpState = getTcpState();

    while(true) {
        auto ourLastAcknowledgment = getOurTCB().window.getLastAcknowledgment();

        try
        {
            getPacketStream().receivePacket(receivedPacket);
        } catch(Timeout::TimeoutException &)
        {
            return false;
        }

        if(sendACKs && receivedPacket.getSegementLength() != 0)
            sendAcknowledment();

        if(receivedPacket.getAcknowledgmentFlag() && ourLastAcknowledgment < receivedPacket.getAcknowledgmentNumber())
            mRetransmitQueue.deleteAcknowledgmentNumber(getOurTCB().window.createSequenceNumber(receivedPacket.getAcknowledgmentNumber()));

        // If this is a data packet.
        if(receivedPacket.isDataPacket() && receivedPacket.getSegementLength() == receivedPacket.getDataLength()) {
            mReceiveQueue.push(std::move(receivedPacket.getVectorData()), getYourTCB().window.createSequenceNumber(receivedPacket.getSequenceNumber()));

            if(packetToReceive == TcpPacketType::DataPacket)
                break;
        }

        if(packetToReceive == TcpPacketType::FinishPacket && receivedPacket.getFinishFlag())
            break;

        if(packetToReceive == TcpPacketType::SycedAck && getOurTCB().window.isSynced())
            break; // We're received the requested ack.

        if(packetToReceive == TcpPacketType::SynPacket && receivedPacket.getSynFlag())
            break;

        // Check if the state has been changed.
        if(getTcpState() != currentTcpState)
            return false;
    }

    return true;
}

TcpFilter::TcpTransmissionControlBlock &TcpDataStream::getOurTCB()
{
    return getPacketStream().getFilter().getSession().our;
}

TcpFilter::TcpTransmissionControlBlock &TcpDataStream::getYourTCB()
{
    return getPacketStream().getFilter().getSession().your;
}

TcpFilter::TcpState TcpDataStream::getTcpState() const
{
    return getPacketStream().getFilter().getSession().tcpState;
}

TcpDataStream::RetransmitQueue::RetransmitQueue()
{
}

void TcpDataStream::RetransmitQueue::push(TcpDataStream::RetransmitQueue::SavedData &&savedData) {
    pl_assert(mAcknowledgmentsMap.count(savedData.expectedAcknowledgmentNumber) == 0);
    mRetransmitQueue.push_back(std::move(savedData));
    mAcknowledgmentsMap[savedData.expectedAcknowledgmentNumber] = --mRetransmitQueue.end();
}

TcpDataStream::RetransmitQueue::SavedData &TcpDataStream::RetransmitQueue::front()
{
    return mRetransmitQueue.front();
}

bool TcpDataStream::RetransmitQueue::empty() const
{
    return mRetransmitQueue.empty();
}

void TcpDataStream::RetransmitQueue::pop() {
    mAcknowledgmentsMap.erase(mRetransmitQueue.front().expectedAcknowledgmentNumber);
    mRetransmitQueue.pop_front();
}

void TcpDataStream::RetransmitQueue::move_front_back() {
    mRetransmitQueue.push_back(std::move(mRetransmitQueue.front()));
    mRetransmitQueue.pop_front();

    mAcknowledgmentsMap[mRetransmitQueue.back().expectedAcknowledgmentNumber] = --mRetransmitQueue.end();
}

void TcpDataStream::RetransmitQueue::deleteAcknowledgmentNumber(const TcpSequenceNumber &tcpAcknowlegmentNumber) {
    if(mAcknowledgmentsMap.empty())
        return;

    auto endIterator = mAcknowledgmentsMap.find(tcpAcknowlegmentNumber);
    if(endIterator == mAcknowledgmentsMap.end())
        return;

    ++endIterator;

    auto iterator = mAcknowledgmentsMap.begin();

    while(iterator != endIterator) {
        mRetransmitQueue.erase(iterator->second);

        iterator = mAcknowledgmentsMap.erase(iterator);
    }
}

TcpDataStream::ReceiveQueue::ReceiveQueue()
{
}

void TcpDataStream::ReceiveQueue::push(OctetVector &&octetVector, const TcpSequenceNumber &tcpSequenceNumber) {
    pl_assert(octetVector.empty() == false);
//    mReceiveMap[tcpSequenceNumber] = std::move(octetVector);

    if(tcpSequenceNumber != mNextSequenceNumber) {
        mReceiveMap[tcpSequenceNumber] = std::move(octetVector);
    } else if(mReceiveMap.empty()) {
        pl_assert(tcpSequenceNumber == mNextSequenceNumber);
        addToReceiveBuffer(std::move(octetVector));
    } else {
        pl_assert(tcpSequenceNumber == mNextSequenceNumber);
        addToReceiveBuffer(std::move(octetVector));
        mergeReceiveMapToReceiveBuffer();
    }
}

void TcpDataStream::ReceiveQueue::pop_front(OctetVector &octetVector) {
//    pl_assert(empty() == false);
//    octetVector.clear();

//    auto iterator = mReceiveMap.begin();
//    uint32_t nextSequenceNumber = mOldestSequenceNumber;

//    while(iterator != mReceiveMap.end()) {
//        if(iterator->first != nextSequenceNumber)
//            break;

//        auto &savedOctetVector = iterator->second;
//        nextSequenceNumber += savedOctetVector.size();

//        if(octetVector.empty())
//            octetVector = std::move(savedOctetVector);
//        else
//            octetVector.insert(octetVector.end(), savedOctetVector.begin(), savedOctetVector.end());

//        iterator = mReceiveMap.erase(iterator);
//    }

//    mOldestSequenceNumber = nextSequenceNumber;

    pl_assert(empty() == false);
    octetVector.clear();
    octetVector = std::move(mReceiveBuffer);
}

bool TcpDataStream::ReceiveQueue::empty() const{
//    return mReceiveMap.empty() || mReceiveMap.begin()->first != mOldestSequenceNumber;

    return mReceiveBuffer.empty();
}

void TcpDataStream::ReceiveQueue::mergeReceiveMapToReceiveBuffer() {
    auto iterator = mReceiveMap.begin();

    while(iterator != mReceiveMap.end()) {
        if(iterator->first != mNextSequenceNumber)
            break;

        addToReceiveBuffer(std::move(iterator->second));

        iterator = mReceiveMap.erase(iterator);
    }
}

void TcpDataStream::ReceiveQueue::setNextSequenceNumber(const uint32_t &nextSequenceNumber)
{
    mNextSequenceNumber = nextSequenceNumber;
}

uint32_t TcpDataStream::ReceiveQueue::getNextSequenceNumber()
{
    return mNextSequenceNumber;
}

void TcpDataStream::ReceiveQueue::addToReceiveBuffer(OctetVector &&octetVector) {
    mNextSequenceNumber += octetVector.size();
    mReceiveBuffer.add(std::move(octetVector));
}

} // Tcp
} // ProtocolLearn
