project(VectorFunctions)
add_library(${PROJECT_NAME} ${pl_library_type} VectorFunctions.cpp VectorFunctions.h)

pl_add_module_auto_caller(Utilities/Debug)
