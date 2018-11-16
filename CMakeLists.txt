cmake_minimum_required (VERSION 3.5)

set(TARGET sockets_threads)

set(PROJ_PATH ${CMAKE_SOURCE_DIR})

project({TARGET})

include_directories(${PROJ_PATH}/inc)

set(EXECUTABLE_OUTPUT_PATH ${PROJ_PATH}/bin)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")

set(SOURCE
	${PROJ_PATH}/src/Debug.cpp
	${PROJ_PATH}/src/Socket.cpp
	${PROJ_PATH}/src/Thread.cpp
	${PROJ_PATH}/src/Mutex.cpp
	${PROJ_PATH}/main.cpp
)

add_executable(${TARGET} ${SOURCE})

target_link_libraries(${TARGET} pthread)
