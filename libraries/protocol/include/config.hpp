#pragma once

#define ACHAIN_BLOCKCHAIN_VERSION              ( version(0, 0, 1) )
#define ACHAIN_BLOCKCHAIN_HARDFORK_VERSION     ( hardfork_version( ACHAIN_BLOCKCHAIN_VERSION ) )

#define ACHAIN_INIT_PUBLIC_KEY_STR             "WKA859Vj7Hh38YN1uU4eCrxsJvZ8Ls7TqzQ2CYXZ9ebksz9UAGhpg"
#define ACHAIN_CHAIN_ID                          (fc::sha256::hash("achain_production"))
#define VESTS_SYMBOL  (uint64_t(6) | (uint64_t('V') << 8) | (uint64_t('E') << 16) | (uint64_t('S') << 24) | (uint64_t('T') << 32) | (uint64_t('S') << 40)) ///< VESTS with 6 digits of precision
#define COIN_SYMBOL  (uint64_t(3) | (uint64_t('W') << 8) | (uint64_t('E') << 16) | (uint64_t('K') << 24) | (uint64_t('U') << 32) )                        ///< WEKU with 3 digits of precision
#define CBD_SYMBOL    (uint64_t(3) | (uint64_t('W') << 8) | (uint64_t('K') << 16) | (uint64_t('D') << 24) )                                                ///< WEKU Backed Dollars with 3 digits of precision
#define STMD_SYMBOL   (uint64_t(3) | (uint64_t('W') << 8) | (uint64_t('E') << 16) | (uint64_t('K') << 24) | (uint64_t('U') << 32) | (uint64_t('D') << 40)) ///< WEKU Dollars with 3 digits of precision
#define ACHAIN_SYMBOL                          "WEKU"
#define ACHAIN_ADDRESS_PREFIX                  "WKA"

//GMT: Tuesday, May 1, 2018 9:00:00 AM
#define GENESIS_TIME                    (uint32_t(1525165200)) //1458835200
//GMT: Tuesday, May 1, 2018 10:00:00 AM
#define MINING_TIME                     (uint32_t(1525168800)) //1458838800