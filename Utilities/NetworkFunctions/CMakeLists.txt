project(NetworkFunctions)
add_library(${PROJECT_NAME} ${pl_library_type} NetworkFunctions.cpp NetworkFunctions.h)

pl_add_module_auto_caller(Utilities/OctetVector)
