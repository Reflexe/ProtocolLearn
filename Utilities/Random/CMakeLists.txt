project(Random)
add_library(${PROJECT_NAME} ${pl_library_type} Random.cpp Random.h)

pl_add_module_auto_caller(Utilities/PTime)
pl_add_module_auto_caller(Utilities/Exceptions)

add_executable("${PROJECT_NAME}TestE" RandomTest.cpp)
pl_add_module("${PROJECT_NAME}TestE" Utilities/Random)
add_test(RandomTest RandomTestE)
