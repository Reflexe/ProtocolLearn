project(MacAddress)
add_library(${PROJECT_NAME} ${pl_library_type} MacAddress.cpp MacAddress.h)

pl_add_module_auto_caller(Utilities/StringFunctions)
pl_add_module_auto_caller(Utilities/Convertions)
