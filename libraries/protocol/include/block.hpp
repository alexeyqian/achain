#pragma once
#include <vector>

#include "types.hpp"

namespace achain{namespace protocol{
    struct signed_block: public signed_block_header{
        checksum_type calculate_merkle_root() const;
        std::vector<singed_transaction> transactions;
    }
}}