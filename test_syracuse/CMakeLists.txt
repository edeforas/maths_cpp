cmake_minimum_required(VERSION 3.20.0)
project (test_syracuse)

add_executable(main_test_cycle 
	main_test_cycle.cpp 
	biginteger.cpp biginteger.h
	syracuse_test_cycle.cpp syracuse_test_cycle.h
)

add_executable(main_test_numbers_mod_k
	main_test_numbers_mod_k.cpp 
	biginteger.cpp biginteger.h
	syracuse_test_numbers_mod_k.cpp syracuse_test_numbers_mod_k.h
)

add_executable(main_test_numbers
	main_test_numbers.cpp 
	biginteger.cpp biginteger.h
	syracuse_test_numbers.cpp syracuse_test_numbers.h
)