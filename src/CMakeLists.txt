cmake_minimum_required(VERSION 3.4)

project(render)

file(GLOB all_src "*.hpp" "*.cpp")
add_executable(render ${all_src})

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

find_package(SFML 2 REQUIRED graphics window system)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(render ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()