project(GetRoute)
add_executable(${PROJECT_NAME} main.cpp)

pl_add_module_auto_caller(Layers/Other/RtNetlink/Route)


