cmake_minimum_required(VERSION 3.1)

set(PROJECTNAME s3nsor)

project( ${PROJECTNAME} )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(EV3C_DIR "${CMAKE_SOURCE_DIR}/../ev3c" CACHE STRING "Location of ev3c")
set(LIBS    ${LIBS}    "${EV3C_DIR}/lib/ev3c.a")
set(INCLUDE ${INCLUDE} "${EV3C_DIR}/include")

find_package(PkgConfig)
pkg_check_modules(MICROHTTP REQUIRED libmicrohttpd)
set(LIBS    ${LIBS}    ${MICROHTTP_LIBRARIES})
set(INCLUDE ${INCLUDE} ${MICROHTTP_INCLUDE_DIRS})
set(OPTIONS ${OPTIONS} ${MICROHTTP_CFLAGS_OTHER})

set(SRCS
	${CMAKE_SOURCE_DIR}/src/s3nsor.cpp
)

add_executable( ${PROJECTNAME} ${HDRS} ${SRCS} )
target_include_directories(${PROJECTNAME} PUBLIC ${INCLUDE})
target_link_libraries(${PROJECTNAME} PUBLIC ${LIBS})
target_compile_options(${PROJECTNAME} PUBLIC ${OPTIONS})

install(TARGETS ${PROJECTNAME} RUNTIME DESTINATION bin)
