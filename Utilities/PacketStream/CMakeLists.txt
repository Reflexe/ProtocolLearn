project(PacketStream)
add_library(${PROJECT_NAME} ${pl_library_type} PacketStream.cpp PacketStream.h)

pl_add_module_auto_caller(Utilities/Timeout)
pl_add_module_auto_caller(Utilities/ProtocolFilter)
