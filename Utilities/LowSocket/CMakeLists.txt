project(LowSocket)
add_library(${PROJECT_NAME} ${pl_library_type} LowSocket.cpp LowSocket.h)

pl_add_module_auto_caller(Utilities/MacAddress)
pl_add_module_auto_caller(Utilities/SocketDataStream)
pl_add_module_auto_caller(Utilities/Interface)
