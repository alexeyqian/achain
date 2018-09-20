PROJECT(CRYPTOTEST)
SET(SRC_LIST crypto_test.cpp ../crypto/ctaes/ctaes.c aes.cpp chacha20.cpp
        hmac_sha256.cpp hmac_sha512.cpp ripemd160.cpp
        sha1.cpp sha256.cpp sha256_avx2.cpp sha256_sse4.cpp sha256_sse41.cpp sha512.cpp)
MESSAGE(STATUS "This is BINARY dir " ${CRYPTOTEST_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir " ${CRYPTOTEST_SOURCE_DIR})
ADD_EXECUTABLE(CRYPTOTEST ${SRC_LIST})