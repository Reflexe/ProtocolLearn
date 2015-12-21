project(Arp)
add_library(${PROJECT_NAME} ${pl_library_type} ArpDataStream.cpp ArpDataStream.h ArpFilter.cpp ArpFilter.h ArpPacket.cpp ArpPacket.h ArpStream.cpp ArpStream.h)

include ( ../../pl_protocol.cmake )
