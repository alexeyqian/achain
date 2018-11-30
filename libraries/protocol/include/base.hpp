#pragma once

#include <vector>
namespace achain {namespace protocol {
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
}}