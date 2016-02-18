project(OptionsParser)
add_library(${PROJECT_NAME} ${pl_library_type} OptionsParser.cpp OptionsParser.h)

pl_add_module_auto_caller(Utilities/OctetVector)
