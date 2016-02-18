project(MessageSocket)
add_library(${PROJECT_NAME} ${pl_library_type} MessageSocket.cpp MessageSocket.h)

pl_add_module_auto_caller(Utilities/DataStream Utilities/Socket)
