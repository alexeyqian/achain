PROJECT(CRYPTO)
#MESSAGE(STATUS "This is BINARY dir " ${CRYPTOLIB_BINARY_DIR})
#MESSAGE(STATUS "This is SOURCE dir " ${CRYPTOLIB_SOURCE_DIR})
#SET(CXXFLAGS "std=c++11")
# add source and binary directory to project
ADD_SUBDIRECTORY(src bin)
#INSTALL(PROGRAMS runcrypto.sh DESTINATION bin)
#INSTALL(DIRCTORY doc/ DESTINATION share/doc/crypto)
