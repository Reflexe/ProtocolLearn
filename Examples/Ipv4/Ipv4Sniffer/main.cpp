#include <iostream>

#include "SmartIpv4Stream.h"
#include "Ipv4FragmentReassemblyManager.h"

using namespace ProtocolLearn;

std::string ethernetTypeToString(uint16_t ethernetType);
ProtocolLearn::Ethernet::EthernetFilter::DropReasonType ethernetFilterCallback(const Ethernet::EthernetPacket &ethernetPacket);
std::string ipv4TypeToString(uint8_t protocol);
std::string optionTypeToString(Ipv4::Ipv4OptionIDType optionID);
std::string ipv4PacketParsingErrorToString(Ipv4::Ipv4Packet::ParsingError parsingError);
ProtocolLearn::Ipv4::Ipv4Filter::DropReasonType filterCallback(const ProtocolLearn::Ipv4::Ipv4Packet &ipv4Packet);
int usage(const char *programName);


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

ProtocolLearn::Ethernet::EthernetFilter::DropReasonType ethernetFilterCallback(const Ethernet::EthernetPacket &ethernetPacket) {
    std::cout << "== Ethernet header: " << std::endl;

    using std::cout;
    using std::endl;
    using std::hex;
    using std::dec;

    cout << "Packet captured:\t" << endl;
    cout << "\tPacket Length:\t" << dec << ethernetPacket.getPacketLength() << endl;
    cout << "\tSource:\t\t" << ethernetPacket.getSource().toString() << endl;
    cout << "\tDestination:\t" << ethernetPacket.getDestination().toString() << endl;
    cout << "\tProtocol:\t0x" << hex << ethernetPacket.getProtocol();
    cout << " (" << ethernetTypeToString(ethernetPacket.getProtocol()) << ")" << endl;

    return ProtocolLearn::Ethernet::EthernetFilter::None;
}

std::string ipv4TypeToString(uint8_t protocol) {
    std::string protocolName = "";

    switch (protocol) {
    case IPPROTO_ICMP:
        protocolName = "ICMP";
        break;
    case IPPROTO_TCP:
        protocolName = "TCP";
        break;
    case IPPROTO_UDP:
        protocolName = "UDP";
        break;
    case IPPROTO_MTP:
        protocolName = "MTP";
        break;
    default:
        protocolName = "Unknown";
    }

    return protocolName;
}

std::string optionTypeToString(Ipv4::Ipv4OptionIDType optionID) {
    typedef Ipv4::Ipv4OptionIDType::OptionNumber OptionNumber;
    typedef Ipv4::Ipv4OptionIDType::OptionClass OptionClass;

    switch (optionID.optionClass) {
    case OptionClass::Control:
        switch (optionID.optionNumber) {
        case OptionNumber::End:
            return "End";
        case OptionNumber::LooseRoute:
            return "Loose route";
        case OptionNumber::NoOperation:
            return "No Operation";
        case OptionNumber::RecordRoute:
            return "Record route";
        case OptionNumber::Security:
            return "Security";
        case OptionNumber::StreamID:
            return "Stream id";
        case OptionNumber::StrictRoute:
            return "Strict route";
        default:
            return "Unknown Option Number";
        }

    case OptionClass::Debug:
        switch (optionID.optionNumber) {
        case OptionNumber::Timestamp:
            return "Timestamp";
        default:
            return "Unknown Option Number";
        }
    default:
        return "Unknown Option Class";
    }
}

std::string ipv4PacketParsingErrorToString(Ipv4::Ipv4Packet::ParsingError parsingError) {
    typedef Ipv4::Ipv4Packet::ParsingError ParsingError;

    switch (parsingError) {
    case ParsingError::None:
        return "None";
    case ParsingError::InvalidVersion:
        return "Invalid version";
    case ParsingError::InvalidInternetHeaderLength:
        return "Invalid internet header length";
    case ParsingError::NoPlaceForOptions:
        return "There's no place for the options";
    default:
        return "Unknown";
    }
}

ProtocolLearn::Ipv4::Ipv4Filter::DropReasonType filterCallback(const ProtocolLearn::Ipv4::Ipv4Packet &ipv4Packet) {
    using std::cout;
    using std::endl;
    using std::dec;
    using std::hex;

    typedef ProtocolLearn::Ipv4::Ipv4Filter::DropReasonType DropReasonType;

    cout << "Packet captured:\t" << endl;
    cout << "\tPacket Length:\t" << dec << ipv4Packet.getPacketLength() << endl;
    cout << "\tSource:\t\t" << ipv4Packet.getSource().toString() << endl;
    cout << "\tDestination:\t" << ipv4Packet.getDestination().toString() << endl;
    cout << "\tProtocol:\t0x" << hex << +ipv4Packet.getProtocol();
    cout << " (" << ipv4TypeToString(ipv4Packet.getProtocol()) << ")" << endl;
    cout << "\tChecksum:\t0x" << ipv4Packet.getHeaderChecksum();

    if(ipv4Packet.isChecksumValid())
        cout << " (Valid)" << endl;
    else
        cout << " (Not Valid)" << endl;

    if(ipv4Packet.isValidPacket() == false)
        cout << "\t<!> Invalid Packet:\t" << ipv4PacketParsingErrorToString(ipv4Packet.getParsingError()) << endl;
    if(ipv4Packet.getParser().isValidOptions() == false)
        cout << "\t<!> Invalid Options: ***" << endl;

    for(const auto &option : ipv4Packet.getParser().getMap()) {
        cout << "\tOption name: " << optionTypeToString(option.second.optionID) << endl;
        cout << "\tOption Length: 0x" << hex << option.second.data.size() << endl;
    }

    return ipv4Packet.isValidPacket() ? static_cast<DropReasonType>(ProtocolLearn::Ipv4::Ipv4Filter::None) : static_cast<DropReasonType>(ProtocolLearn::Ipv4::Ipv4Filter::InvalidPacket);
}

int usage(const char *programName) {
    std::cout << "Usage: " << programName << " <Interface Name>" << std::endl;
    return -1;
}

int main(int argc, char *argv[]) {
    if(argc != 2)
        return usage(argv[0]);

    using namespace ProtocolLearn::Ipv4;

    Libraries::SmartIpv4Stream smartIpv4Stream{{Interface{argv[1]}, MacAddress{}}};

    smartIpv4Stream.getEthernetStream().getFilter().setProtocolFilterStatus(false);
    smartIpv4Stream.getEthernetStream().getFilter().setPreviousPacketFilterStatus(false);
    smartIpv4Stream.getEthernetStream().getFilter().setUserFilterStatus(true, ethernetFilterCallback);

    Ipv4Packet ipv4Packet;

    smartIpv4Stream.getIpv4Stream().getFilter().setProtocolFilterStatus(false);
    smartIpv4Stream.getIpv4Stream().getFilter().setPreviousPacketFilterStatus(false);
    smartIpv4Stream.getIpv4Stream().getFilter().setUserFilterStatus(true, filterCallback);

    Ipv4FragmentReassemblyManager fragmentReassemblers;

    while(true) {
        smartIpv4Stream.getIpv4Stream().receivePacket(ipv4Packet, PTime::infinity());

        if (Ipv4FragmentReassembler::isFragment(ipv4Packet)) {
            pl_crap("Found a fragmented packet");

            if(fragmentReassemblers.enterPacket(std::move(ipv4Packet)) == true) {
                std::cout << "Fragmented packet reassembled, details: " << std::endl;
                filterCallback(ipv4Packet);
            }
        }

        if(Ipv4FragmentReassembler::isFirstFragment(ipv4Packet))
            std::cout << "\tFirst Fragment" << std::endl;
        if(Ipv4FragmentReassembler::isFragment(ipv4Packet))
            std::cout << "\tFragment" << std::endl;
    }
}
