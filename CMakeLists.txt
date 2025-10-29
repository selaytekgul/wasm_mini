cmake_minimum_required(VERSION 3.35)
project(wasm_mini)
set(CMAKE_CXX_STANDARD 23)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(AutoCollect)
include(CompileOptions)

add_executable(wasm_mini main.cpp)
add_subdirectory(dependencies)
target_link_libraries(${PROJECT_NAME} PUBLIC rlImGui)