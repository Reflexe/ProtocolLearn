project(Timeout)
add_library(${PROJECT_NAME} ${pl_library_type} Timeout.cpp Timeout.h)

pl_add_module_auto_caller(Utilities/PTime)
pl_add_module_auto_caller(Utilities/Exceptions)
