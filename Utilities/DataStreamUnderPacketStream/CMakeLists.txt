project(DataStreamUnderPacketStream)
add_library(${PROJECT_NAME} ${pl_library_type} DataStreamUnderPacketStream.cpp DataStreamUnderPacketStream.h)

pl_add_module_auto_caller(Utilities/PacketStream)
pl_add_module_auto_caller(Utilities/DataStream)
