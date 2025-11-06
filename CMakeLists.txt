cmake_minimum_required(VERSION 3.10)
project(HighPrecisionTimerExample)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(timer_example example.cpp)

# Link pthread library (required for std::thread on Linux)
find_package(Threads REQUIRED)
target_link_libraries(timer_example PRIVATE Threads::Threads)

# Optional: Enable warnings
if(MSVC)
    target_compile_options(timer_example PRIVATE /W4)
else()
    target_compile_options(timer_example PRIVATE -Wall -Wextra -pedantic)
endif()
