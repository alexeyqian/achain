#pragma once

#include <vector>

#include <achain/protocol/types.hpp>
#include <achain/protocol/block_header.hpp>
#include <achain/protocol/transaction.hpp>

namespace achain{namespace protocol{
    struct signed_block: public signed_block_header{
        checksum_type calculate_merkle_root() const;
        std::vector<singed_transaction> transactions;
    };
}}