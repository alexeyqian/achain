#pragma once

#include "block_header.hpp"
#include "transaction.hpp"

namespace achain {
    namespace protocol {

        struct signed_block : public signed_block_header {
            checksum_type calculate_merkle_root() const;

            vector <signed_transaction> transactions;

        };
    }
}