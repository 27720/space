cmake_minimum_required(VERSION 3.20)
project(space LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Adding math.cpp when I implement it
add_executable(main src/main.cpp)

# Link libraries
# (open its respective .cmake file and search add_library to find the target name)
target_link_libraries(main PRIVATE
    glfw
    glad::glad
    glm::glm
)
