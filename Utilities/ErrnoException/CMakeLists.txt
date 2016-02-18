project(ErrnoException)
add_library(${PROJECT_NAME} ${pl_library_type} ErrnoException.cpp ErrnoException.h)

pl_add_module_auto_caller(Utilities/Exceptions)
