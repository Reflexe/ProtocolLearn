project(Arpv4)
add_library(${PROJECT_NAME} ${pl_library_type} ArpEthernetIpv4Filter.cpp ArpEthernetIpv4Filter.h ArpEthernetIpv4Packet.cpp ArpEthernetIpv4Packet.h ArpEthernetIpv4Stream.cpp ArpEthernetIpv4Stream.h)

include ( ../../pl_protocol.cmake )

pl_add_module_auto_caller(Utilities/Ipv4Address)
pl_add_module_auto_caller(Utilities/MacAddress)

add_subdirectory(ArpEthernetIpv4Resolver)
