cmake_minimum_required(VERSION 3.5)
project(exportGlobalVariableData VERSION 0.0.1)

add_compile_definitions(PROJECT_NAME="${CMAKE_PROJECT_NAME}")
add_compile_definitions(PROJECT_EXPORT=${CMAKE_PROJECT_NAME})
add_compile_definitions(PROJECT_VERSION="${CMAKE_PROJECT_VERSION}")


add_library(exportGlobalVariableData SHARED
	src/Source.cpp
	src/RedoBlHooks.cpp
	src/Torque.cpp
)