# Request the minimum version of CMake, in case of lower version throws error
# See: https://cmake.org/cmake/help/latest/command/cmake_minimum_required.html

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Include headers project
# See: https://cmake.org/cmake/help/latest/command/include_directories.html

# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Collect tests
file(GLOB TESTS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/unit/*.c)

# Files to test
# file(GLOB SRC_FILES 
# ${CMAKE_CURRENT_SOURCE_DIR}/../src/functions.c)


if(RUN_COVERAGE)
	message("Coverage enabled")
	find_program(LCOV_PATH lcov)
		if(NOT LCOV_PATH)
		message(FATAL_ERROR "lcov not found! Aborting..")
	endif()

	find_program(GCOV_PATH gcov)
		if(NOT GCOV_PATH)
		message(FATAL_ERROR "gcov not found! Aborting..")
	endif()

	find_program(GENHTML_PATH genhtml)
		if(NOT GENHTML_PATH)
		message(FATAL_ERROR "genh tml not found! Aborting..")
	endif()

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fprofile-arcs -ftest-coverage --coverage")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov --coverage")
    endif()

# Create test executable
add_executable(test_${PROJECT_NAME} ${TESTS_FILES})

# Link with Unity
target_link_libraries(test_${PROJECT_NAME} Unity_test lib)

# Add test
add_test(NAME test_${PROJECT_NAME} COMMAND test_${PROJECT_NAME})
