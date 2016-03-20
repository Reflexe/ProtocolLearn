project(DataStream)
add_library(${PROJECT_NAME} ${pl_library_type} DataStream.cpp DataStream.h)

pl_add_module_auto_caller(Utilities/Timeout)
pl_add_module_auto_caller(Utilities/OctetVector)
pl_add_module_auto_caller(Utilities/CompilerFunctions)
