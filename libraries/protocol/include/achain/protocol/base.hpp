#pragma once

#include <vector>
#include <boost/container/flat_set.hpp>

#include <achain/protocol/types.hpp>
#include <achain/protocol/authority.hpp>
#include <achain/protocol/version.hpp>

namespace achain {namespace protocol {
    using boost::container::flat_set;
    using boost::container::flat_map;

    struct base_operation{
        void get_required_authorities(std::vector<authority>&) const{}
        void get_required_active_autorities(flat_set<account_name_type>&) const{}
        void get_required_posting_authorities(flat_set<account_name_type>&) const{}
        void get_required_owner_autorities(flat_set<account_name_type>& )const{}

        bool is_virtual() const{return false;}
        void validate() const{}
    };

    struct virtual_operation : public base_operation{
        bool is_virtual() const {return true;}
        void validate() const {FC_ASSERT(false, "This is a virtual operation");}
    };

    typedef fc::static_variant<void_t, version, hardfork_version_vote> block_header_extensions;
    typedef fc::static_variant<void_t> future_extensions;
    typedef boost::container::flat_set<block_header_extensions> block_header_extensions_type;
    typedef boost::container::flat_set<future_extensions> extensions_type;
}}

FC_REFLECT_TYPENAME( achain::protocol::block_header_extensions )
FC_REFLECT_TYPENAME( achain::protocol::future_extensions )