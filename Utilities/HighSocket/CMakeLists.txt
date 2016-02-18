project(HighSocket)
add_library(${PROJECT_NAME} ${pl_library_type} HighSocket.cpp HighSocket.h)

pl_add_module_auto_caller(Utilities/Socket)
