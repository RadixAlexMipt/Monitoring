cmake_minimum_required(VERSION 3.5.0)
project(Monitoring VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
 
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
 
add_subdirectory(third_party)
 
add_executable(Monitoring main.cpp monitoring.cpp sensor.cpp)
 
target_link_libraries(Monitoring csv)
target_link_libraries(Monitoring cpr)