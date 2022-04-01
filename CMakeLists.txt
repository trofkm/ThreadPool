cmake_minimum_required(VERSION 3.15)

project(threadPool VERSION 0.1.1)

set(ENV{current_project} ${CMAKE_PROJECT_NAME})


message(STATUS "Setup ENV current_project to: $ENV{current_project}" )


file(GLOB_RECURSE sources "src/modules/**/*.cpp")



add_executable(${CMAKE_PROJECT_NAME} src/main.cpp ${sources})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE pthread)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src src/modules )
