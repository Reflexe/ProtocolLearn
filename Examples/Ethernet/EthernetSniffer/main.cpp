#include <iostream>

#include "SocketInterface.h"

#include "EthernetStream.h"

int usage(char *programName);
std::string ethernetTypeToString(uint16_t ethernetType);

int usage(char *programName) {
    std::cout << "Usage: " << programName << " <Interface>" << std::endl;
    return -1;
}

std::string ethernetTypeToString(uint16_t ethernetType) {
    std::string ethernetStringType;

    switch (ethernetType) {
    case ETH_P_IP:
        ethernetStringType = "IPv4";
        break;
    case ETH_P_ARP:
        ethernetStringType = "ARP";
        break;
    case ETH_P_IPV6:
        ethernetStringType = "IPv6";
        break;
    case ETH_P_RARP:
        ethernetStringType = "RARP";
        break;
    default:
        ethernetStringType = "Unknown";
        break;
    }

    return ethernetStringType;
}

//void configLowSocket(ProtocolLearn::LowSocket &lowSocket, const std::string &interfaceName)
//{
//    lowSocket.configEthernetSocket(ProtocolLearn::Interface(interfaceName), ProtocolLearn::MacAddress());
//}

int main(int argc, char *argv[]) {
    using namespace ProtocolLearn::Ethernet;
    using ProtocolLearn::SocketInterface;
    using ProtocolLearn::Interface;

    if(argc != 2)
        return usage(argv[0]);

    SocketInterface socketInterface{Interface{std::string{argv[1]}}};
    EthernetStream ethernetStream(socketInterface);
    EthernetPacket ethernetPacket;

    while(true) {
        using std::cout;
        using std::endl;
        using std::hex;
        using std::dec;

        ethernetStream._recv(ethernetPacket, ProtocolLearn::PTime::infinity());

        cout << "Packet captured:\t" << endl;
        cout << "\tPacket Length:\t" << dec << ethernetPacket.getPacketLength() << endl;
        cout << "\tSource:\t\t" << ethernetPacket.getSource().toString() << endl;
        cout << "\tDestination:\t" << ethernetPacket.getDestination().toString() << endl;
        cout << "\tProtocol:\t0x" << hex << ethernetPacket.getProtocol();
        cout << " (" << ethernetTypeToString(ethernetPacket.getProtocol()) << ")" << endl;
    }
}

