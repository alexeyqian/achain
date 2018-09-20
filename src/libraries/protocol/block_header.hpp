#pragma once

#include "types.hpp"

namespace achain {
    namespace protocol {
        class public_key{};
        class private_key{};

        struct block_header {
            static uint32_t num_from_id(const block_id_type &id);

            digest_type digest() const;

            uint32_t block_num() const { return num_from_id(previous) + 1; }

            block_id_type previous;
            time_point_sec timestamp;
            string witness;
            checksum_type transaction_merkle_root;
            block_header_extensions_type extensions;
        };

        struct signed_block_header : public block_header {
            block_id_type id() const;

            public_key signee() const;

            void sign(const private_key &signer);

            bool validate_signee(const public_key &expected_signee) const;

            signature_type witness_signature;
        };
    }
}