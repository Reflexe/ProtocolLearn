project(RtNetlink)
add_library(${PROJECT_NAME} ${pl_library_type} RtNetlinkFilter.cpp RtNetlinkFilter.h RtNetlinkPacket.cpp RtNetlinkPacket.h RtNetlinkStream.cpp RtNetlinkStream.h RtAttrParser.h RtAttrParser.cpp)

include ( ../../pl_protocol.cmake )

pl_add_module_auto_caller(Utilities/OptionsParser)

add_subdirectory(Route)
