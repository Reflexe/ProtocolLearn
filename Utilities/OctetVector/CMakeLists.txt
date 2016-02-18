project(OctetVector)
add_library(${PROJECT_NAME} ${pl_library_type} OctetVector.h OctetVector.cpp)

pl_add_module_auto_caller(Utilities/Debug)
