project(SocketDataStream)
add_library(${PROJECT_NAME} ${pl_library_type} SocketDataStream.cpp SocketDataStream.h)

pl_add_module_auto_caller(Utilities/Socket)
pl_add_module_auto_caller(Utilities/DataStream)
