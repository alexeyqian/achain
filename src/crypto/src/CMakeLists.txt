SET(CRYPTO_SRC
        aes.cpp
        hmac_sha256.cpp
        hmac_sha512.cpp
        ripemd160.cpp
        sha1.cpp
        sha256.cpp
        sha512.cpp)

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
ADD_LIBRARY(crypto SHARED ${CRYPTO_SRC})
#ADD_EXECUTABLE(CRYPTO ${SRC_LIST})

