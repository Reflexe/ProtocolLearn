project(ByteOrder)
add_library(${PROJECT_NAME} ${pl_library_type} ByteOrder.cpp ByteOrder.h)

pl_add_module_auto_caller(Utilities/OctetVector)
