cmake_minimum_required(VERSION 3.10)
project(5-image-structure)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} 5-1.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/horse_mask.png
                ${CMAKE_CURRENT_BINARY_DIR}/horse_mask.png)
