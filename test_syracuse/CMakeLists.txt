cmake_minimum_required(VERSION 3.0.0)
project (test_syracuse)
add_executable(test_syracuse main.cpp 
	biginteger.cpp biginteger.h
	syracuse_test_cycle.cpp syracuse_test_cycle.h
	syracuse_test_numbers.cpp syracuse_test_numbers.h
	syracuse_test_numbers_mod_k.cpp syracuse_test_numbers_mod_k.h
)