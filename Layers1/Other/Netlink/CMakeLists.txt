project(Netlink)
add_library(${PROJECT_NAME} ${pl_library_type} NetlinkDataStream.cpp NetlinkDataStream.h NetlinkFilter.cpp NetlinkFilter.h NetlinkPacket.cpp NetlinkPacket.h NetlinkStream.cpp NetlinkStream.h)

include ( ../../pl_protocol.cmake )

pl_add_module_auto_caller(Utilities/MessageSocket)
