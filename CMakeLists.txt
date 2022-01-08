CMAKE_MINIMUM_REQUIRED(VERSION 3.19)
PROJECT(matrix)

include_directories(project/include)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(matrix project/src/main.cpp project/src/matrix.cpp)