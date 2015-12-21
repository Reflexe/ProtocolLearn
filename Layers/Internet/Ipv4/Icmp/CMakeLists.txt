project(Icmp)
add_library(${PROJECT_NAME} ${pl_library_type} IcmpDataStream.cpp IcmpDataStream.h IcmpFilter.cpp IcmpFilter.h IcmpPacket.cpp IcmpPacket.h IcmpStream.cpp IcmpStream.h)

include ( ../../pl_protocol.cmake )

pl_add_module_auto_caller(Utilities/Random)
pl_add_module_auto_caller(Utilities/NetworkFunctions)
