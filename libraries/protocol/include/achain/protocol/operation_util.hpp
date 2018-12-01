#pragma once

#include <string>
#include <vector>
#include <boost/container/flat_set.hpp>
#include <fc/variant.hpp>
#include <achain/protocol/authority.hpp>

#define DECLARE_OPERATION_TYPE ( OperationType ) \
namespace fc{ \
void to_variant(const OperationType&, fc::variant& ); \
void from_variant(const fc::variant&, OperationType&); \
} \ 
namespace achain { namespace protocol { \
void operation_validate(const OperationType& o); \
void operation_get_required_authorities(const OperationType& op, \
    boost::flat_set<account_name_type>& active, \
    boost::flat_set<account_name_type>& owner,  \
    boost::flat_set<account_name_type>& posting,\
    std::vector<authority>& other); \
}} 
