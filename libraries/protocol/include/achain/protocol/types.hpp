#pragma once

#include <string>
#include <fc/crypto/ripemd160.hpp>
#include <fc/crypto/elliptic.hpp>
#include <fc/crypto/sha224.hpp>
#include <fc/safe.hpp>

#include <achain/protocol/config.hpp>

namespace achain {

    namespace protocol {
        typedef std::string account_name_type;

        typedef fc::ecc::private_key private_key_type;
        typedef fc::sha256 chain_id_type;
        typedef fc::sha256 digest_type;
        
        typedef fc::ripemd160 block_id_type;
        typedef fc::ripemd160 checksum_type;
        typedef fc::ripemd160 transaction_id_type;
        typedef fc::ecc::compact_signature signature_type;
        
        typedef safe<int64_t> share_type;
        typedef uint16_t weight_type;
    }
}