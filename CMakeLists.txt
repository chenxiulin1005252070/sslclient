###################################################
#
# created by tom.chen
#
###################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
project(pthreadpool)

SET(CMAKE_C_COMPILE "gcc.exe")
SET(CMAKE_CXX_COMPILE "g++.exe")
SET(LIBDIR third-party)
SET(LIBDIR_PTHREADPOOL  ${LIBDIR}/pthead_2.9.0_release)
SET(LIBDIR_OPENSSL      ${LIBDIR}/openssl_1.1.1f)

add_definitions("-Wall -Wunknown-pragmas -g -gstabs+")

aux_source_directory(src SRC_LIST)

link_directories(
    ${LIBDIR_PTHREADPOOL}/dll
    ${LIBDIR_OPENSSL}/
)

include_directories(
    inc
    ${LIBDIR_PTHREADPOOL}/inc
)

add_executable(main ${SRC_LIST})
# must be at end of add_executable
target_link_libraries (
    main 
    pthreadGC2
    ws2_32
    Mswsock.lib
    crypto-1_1
    ssl-1_1
)