#pragma once

#include <fc/exception/exception.hpp>
#include <achain/protocol/protocol.hpp>

#define ACHAIN_ASSERT(expr, exc_type, FORMAT, ...) \
    FC_MULTILINE_MACRO_BEGIN \
    if(!(expr)) \
        FC_THROW_EXCEPTION(exc_type, FORMAT, __VA_ARGS__); \
    FC_MULTILINE_MACRO_END

namespace achain{ namespace protocol {
    FC_DECLARE_EXCEPTION( transaction_exception, 3000000, "transaction exception" )
    FC_DECLARE_DERIVED_EXCEPTION( tx_missing_active_auth,            achain::protocol::transaction_exception, 3010000, "missing required active authority" )
    FC_DECLARE_DERIVED_EXCEPTION( tx_missing_owner_auth,             achain::protocol::transaction_exception, 3020000, "missing required owner authority" )
    FC_DECLARE_DERIVED_EXCEPTION( tx_missing_posting_auth,           achain::protocol::transaction_exception, 3030000, "missing required posting authority" )
    FC_DECLARE_DERIVED_EXCEPTION( tx_missing_other_auth,             achain::protocol::transaction_exception, 3040000, "missing required other authority" )
    FC_DECLARE_DERIVED_EXCEPTION( tx_irrelevant_sig,                 achain::protocol::transaction_exception, 3050000, "irrelevant signature included" )
    FC_DECLARE_DERIVED_EXCEPTION( tx_duplicate_sig,                  achain::protocol::transaction_exception, 3060000, "duplicate signature included" )

}}