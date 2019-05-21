cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
project(Spirograph)

file(GLOB SOURCES "./*.cpp")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
add_executable(Spirograph ${SOURCES})
target_link_libraries(Spirograph sfml-graphics sfml-window sfml-system)