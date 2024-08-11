# Request the minimum version of CMake
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Define your project
project(
  gushgush-myshell
  VERSION 1.0.0
  DESCRIPTION "Development of Bourne shell for operating systems I"
  LANGUAGES C
)

# Define the C standard, we are going to use std17
set(CMAKE_C_STANDARD 17)

# Add unity library using fetch content
include(FetchContent)

set(UNITY_URL "https://github.com/ThrowTheSwitch/Unity.git")

FetchContent_Declare(
  unity
  GIT_REPOSITORY ${UNITY_URL} 
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/unity
  UPDATE_COMMAND ""
)
FetchContent_MakeAvailable(unity)

# Add header
include_directories(include)

# Add Unity to the project
add_library(Unity_test STATIC ${unity_SOURCE_DIR}/src/unity.c)
target_include_directories(Unity_test PUBLIC ${unity_SOURCE_DIR}/src) # Add the include folder

# Add Library from lib folder
file(GLOB SOURCES "lib/*.c")
add_library(lib STATIC ${SOURCES})

# Create the executable
add_executable(${PROJECT_NAME} src/main.c )
target_link_libraries(${PROJECT_NAME} lib)

# Add tests if enabled
if(RUNS_TESTS EQUAL 1 OR RUNS_COVERAGE EQUAL 1)
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()

# Compilation flags
if(FLAGS EQUAL 1)
  set(COMMON_FLAGS "-Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11")
endif()
