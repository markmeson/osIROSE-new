cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

link_directories(${EXTERNAL_BINARY_DIR})

set(PROJECT MapServer)
project(${PROJECT})

IF(ENABLE_MYSQL_ONE_INSTANCE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_MYSQL_ONE_INSTANCE")
ENDIF()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE")
set(GCC_LINK_FLAGS "${GCC_LINK_FLAGS}")
set(Clang_LINK_FLAGS "")
set(MSVC_LINK_FLAGS " /NODEFAULTLIB:MSVCRTD /NODEFAULTLIB:MSVCRT")

IF (NOT MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Werror")
ENDIF()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-packed-bitfield-compat -Wl,-Map=${PROJECT}.map")
ELSEIF( ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-keyword-compat")
ENDIF()

set( SOURCES_HEADER
  include/cmapserver
  include/cmapclient
  include/cmapisc 
)

set( SOURCES_MAIN
  src/main.cpp
  src/cmapserver
  src/cmapclient
  src/cmapisc
  src/entitysystem
)

find_package( Threads REQUIRED )
add_executable(${PROJECT} ${SOURCES_MAIN} ${SOURCES_HEADER})

target_include_directories(${PROJECT} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS "${${CMAKE_CXX_COMPILER_ID}_LINK_FLAGS}")
				
IF(ENABLE_MYSQL)
  target_link_libraries(${PROJECT} ${MYSQL_LIBRARIES} mysqlpp)
ENDIF()

target_link_libraries(${PROJECT} ${MYSQL_LIBRARIES} RoseCommon)
target_link_libraries(${PROJECT} ${MYSQL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

IF (MSVC)
  target_link_libraries(${PROJECT} wsock32.lib)
  target_link_libraries(${PROJECT} libprotobuf)
ELSE()
  target_link_libraries(${PROJECT} mysqlclient)
  target_link_libraries(${PROJECT} protobuf)
ENDIF()

#INSTALL(
#    TARGETS ${PROJECT}
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#    ARCHIVE DESTINATION lib/static
#    CONFIGURATIONS Release)
    
