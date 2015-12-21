project(Tcpv4)
add_library(${PROJECT_NAME} ${pl_library_type} TcpIpv4DataStream.cpp TcpIpv4DataStream.h TcpIpv4Filter.cpp TcpIpv4Filter.h TcpIpv4Packet.cpp TcpIpv4Packet.h TcpIpv4Server.cpp TcpIpv4Server.h TcpIpv4Stream.cpp TcpIpv4Stream.h)

include ( ../../pl_protocol.cmake )

pl_add_module_auto_caller(Layers/Transport/Tcp)
pl_add_module_auto_caller(Layers/Internet/Ipv4 Layers/Internet/Ipv4/SmartIpv4Stream)
