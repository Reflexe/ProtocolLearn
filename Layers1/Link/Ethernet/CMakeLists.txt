project(Ethernet)
add_library(${PROJECT_NAME} ${pl_library_type} EthernetDataStream.cpp EthernetDataStream.h EthernetFilter.cpp EthernetFilter.h EthernetPacket.cpp EthernetPacket.h EthernetStream.cpp EthernetStream.h)

include ( ../../pl_protocol.cmake )

pl_add_module_auto_caller(Utilities/MacAddress)
pl_add_module_auto_caller(Layers/Link/LinkProtocol)
