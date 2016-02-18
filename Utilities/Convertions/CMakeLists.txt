project(Convertions)
add_library(${PROJECT_NAME} ${pl_library_type} Convertions.cpp Convertions.h)

pl_add_module_auto_caller(Utilities/Debug)
pl_add_module_auto_caller(Utilities/Exceptions)
