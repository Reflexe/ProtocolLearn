project(Udp)
add_library(${PROJECT_NAME} ${pl_library_type} UdpPacket.h UdpPacket.cpp UdpStream.cpp UdpStream.h UdpFilter.h UdpFilter.cpp UdpDataStream.h UdpDataStream.cpp)

include ( ../../pl_protocol.cmake )

pl_add_module_auto_caller(Utilities/NetworkFunctions)
pl_add_module_auto_caller(Utilities/Random)
