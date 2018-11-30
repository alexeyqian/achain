#pragma once
#include "types.hpp"

namespace achain{namespace protocol{
    struct block_header{
        block_id_type previous;
        fc::time_point_sec timestamp;
        string witness;
        checksum_type transaction_merkle_root;
        block_header_extensions_type extensions;

        digest_type digest() const;        
        uint32_t block_num() const {return num_from_id(previous) + 1;}
        
        static uint32_t num_from_id(const block_id_type& id);
    };

    struct signed_block_header: public block_header{
        signature_type withness_signature;

        block_id_type id() const;
        fc::ecc:public_key signee() const;
        void sign(const fc::ecc::private_key& signer);
        bool validate_signee(const fc::ecc::public_key& expected_signee) const;
    };
}}