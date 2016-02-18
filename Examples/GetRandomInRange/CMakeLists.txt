project(GetRandomNumberInRange)
add_executable(${PROJECT_NAME} main.cpp)

pl_add_module_auto_caller(Utilities/Random)
pl_add_module_auto_caller(Utilities/Convertions)



