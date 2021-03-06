CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(tools)

OPTION(BUILD_PROTOBUF 
  "Build Protobuf (Leave this off)" OFF)
OPTION(BUILD_LUA 
  "Build Lua" OFF)
OPTION(BUILD_MYSQLPP 
  "Build MySQL++" OFF)
  
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

IF (BUILD_PROTOBUF)
  SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  
  SET(CMAKE_BUILD_TYPE $ENV{BUILD_TYPE})
  SET(protobuf_BUILD_TESTS OFF)
  
  IF (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    SET(CMAKE_RULE_MESSAGES OFF)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
    
    #build protobuf
    SET(protobuf_MSVC_STATIC_RUNTIME ON CACHE BOOL "" FORCE)
    ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/protobuf/cmake)
    
    SET(CMAKE_RULE_MESSAGES ON)
  ENDIF()
ENDIF()

IF(BUILD_LUA)
  #build lua
  SET(lua_MSVC_STATIC_RUNTIME ON CACHE BOOL "" FORCE)
  ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/lua)
ENDIF()

IF(BUILD_MYSQLPP) 
  SET(mysqlpp_MSVC_STATIC_RUNTIME ON CACHE BOOL "" FORCE)
  SET(mysqlpp_STATIC_LIB ON CACHE BOOL "" FORCE)
  ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/mysqlpp)
ENDIF()

IF(BUILD_TOOLS)
  INCLUDE_DIRECTORIES(
    SYSTEM ${SPDLOG_ROOT}
  )
  
  IF(ENABLE_TESTING AND ENABLE_TESTING_COVERAGE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
  ENDIF() # ENABLE_TESTING_COVERAGE
  
  IF( ENABLE_ASAN )
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer")
    SET(LINK_FLAGS "${LINK_FLAGS} -g -fsanitize=address")
  ENDIF()
  
  IF(BUILD_STRESS)
	ADD_SUBDIRECTORY(stress)
  ENDIF()
ENDIF()
