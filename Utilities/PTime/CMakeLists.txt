project(PTime)
add_library(${PROJECT_NAME} ${pl_library_type} PTime.cpp PTime.h)

pl_add_module_auto_caller(Utilities/Debug)
