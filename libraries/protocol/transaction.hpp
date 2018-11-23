#pragma once

#include "types.hpp"

namespace achain {
    namespace protocol {

        struct transaction {
            digest_type digest() const;

            transaction_id_type id() const;

            void validate() const;

            digest_type sig_digest(const block_id_type &reference_block);

            void set_expiration(time_point_sec expiration_time);

            void set_reference_block(const block_id_type &referenct_block);

            uint16_t ref_block_num = 0;
            uint32_t ref_block_prefix = 0;
            time_point_sec expiration;
            vector<operation> operations;
            extensions_type extensions;
        };

        struct signed_transaction : public transaction {

        };
    }
}
