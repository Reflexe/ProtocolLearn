project(PacketStreamUnderDataStream)
add_library(${PROJECT_NAME} ${pl_library_type} PacketStreamUnderDataStream.cpp PacketStreamUnderDataStream.h)

pl_add_module_auto_caller(Utilities/PacketStream)
pl_add_module_auto_caller(Utilities/DataStream)
