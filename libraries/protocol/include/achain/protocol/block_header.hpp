#pragma once

#include <string>
#include <achain/protocol/base.hpp>

namespace achain{namespace protocol{
    struct block_header{
        digest_type digest() const;        
        uint32_t block_num() const {return num_from_id(previous) + 1;}        
        static uint32_t num_from_id(const block_id_type& id);

        block_id_type previous;
        fc::time_point_sec timestamp;
        std::string witness;
        checksum_type transaction_merkle_root;
        block_header_extensions_type extensions;
    };

    struct signed_block_header: public block_header{        
        block_id_type id() const;
        fc::ecc:public_key signee() const;
        void sign(const fc::ecc::private_key& signer);
        bool validate_signee(const fc::ecc::public_key& expected_signee) const;

        signature_type witness_signature;
    };
}}

FC_REFLECT( achain::protocol::block_header, (previous)(timestamp)(witness)(transaction_merkle_root)(extensions) )
FC_REFLECT_DERIVED( achain::protocol::signed_block_header, (steemit::protocol::block_header), (witness_signature) )
