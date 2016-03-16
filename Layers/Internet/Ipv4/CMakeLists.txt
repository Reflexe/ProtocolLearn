pl_add_library(Ipv4 Ipv4DataStream Ipv4Filter Ipv4FragmentReassembler Ipv4Fragmenter Ipv4Packet Ipv4OptionsParser
    Ipv4Stream Ipv4FragmentReassemblyManager IPProtocolIpv4
    Icmp/IcmpPacket Icmp/IcmpFilter Icmp/IcmpStream Icmp/IcmpDataStream)

target_include_directories(Ipv4 PUBLIC "Icmp/")
target_include_directories(Ipv4 PUBLIC "./")

include ( ../../pl_protocol.cmake )

pl_add_module_auto_caller(Utilities/Ipv4Address)

pl_add_module_auto_caller(Utilities/TcpIpv4OptionsParser)
pl_add_module_auto_caller(Utilities/BitField)
pl_add_module_auto_caller(Utilities/Timeout)
pl_add_module_auto_caller(Utilities/Random)
pl_add_module_auto_caller(Utilities/Debug)

pl_add_module_auto_caller(Utilities/InternetChecksum)

pl_add_module_auto_caller(Layers/Internet/InternetProtocol)
pl_add_module_auto_caller(Layers/Internet/IPProtocol)

pl_add_module_auto_caller(Layers/Link/LinkProtocol)

add_subdirectory(SmartIpv4Stream)
