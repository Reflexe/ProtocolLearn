project(SocketAddress)
add_library(${PROJECT_NAME} ${pl_library_type} SocketAddress.cpp SocketAddress.h)

pl_add_module_auto_caller(Utilities/Debug)
