cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# Variables
set(PROJECT_NAME gep)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Build confugure" FORCE)
if(DEFINED CMAKE_BUILD_TYPE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# Set project
project(${PROJECT_NAME} C)

# Add definition
if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DDEBUG)
endif()

# Include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set file list
file(GLOB PROJECT_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
set(PROJECT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/gep.h)

# Check type size
include(CheckTypeSize)
set(CMAKE_EXTRA_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/gep.h)
check_type_size("union GEP_NODE" SIZEOF_GEP)
set(CMAKE_EXTRA_INCLUDE_FILES)

# Setup galib
set(GA_TYPE "struct { char size[${SIZEOF_GEP}]; }" CACHE STRING "GA variable type")
set(GA_PREFIX "gep" CACHE STRING "GA function prefix")

# Add galib
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/galib/src)

# Build library
add_library(${PROJECT_NAME} STATIC ${PROJECT_SRCS})
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	OUTPUT_NAME			${PROJECT_NAME}
		PREFIX			"lib"
		PUBLIC_HEADER	${PROJECT_HEADER}
	)

# Install
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
	LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
	PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
	)

