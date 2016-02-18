project(Socket)
add_library(${PROJECT_NAME} ${pl_library_type} Socket.cpp Socket.h)

pl_add_module_auto_caller(Utilities/SocketAddress)
pl_add_module_auto_caller(Utilities/ErrnoException)
pl_add_module_auto_caller(Utilities/Timeout)
pl_add_module_auto_caller(Utilities/OctetVector)
