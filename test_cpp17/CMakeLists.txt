cmake_minimum_required(VERSION 3.20.0)

project (test_cpp17)

add_executable(test_filesystem test_filesystem.cpp)
set_target_properties(test_filesystem PROPERTIES CXX_STANDARD 17)

add_executable(test_parallel_foreach test_parallel_foreach.cpp)
set_target_properties(test_parallel_foreach PROPERTIES CXX_STANDARD 17)

add_executable(test_parallel_thread test_parallel_thread.cpp)
set_target_properties(test_parallel_thread PROPERTIES CXX_STANDARD 17)

add_executable(test_parallel_ThreadPool ThreadPool.h test_parallel_ThreadPool.cpp)