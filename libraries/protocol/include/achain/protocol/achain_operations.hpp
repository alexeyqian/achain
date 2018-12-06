#pragma once

#include <steemit/protocol/base.hpp>
#include <steemit/protocol/block_header.hpp>
#include <steemit/protocol/asset.hpp>

#include <fc/utf8.hpp>
#include <fc/crypto/equihash.hpp>

namespace achain{ namespace protocol {
    inline void validate_account_name(const string& name){
        FC_ASSERT(is_valid_account_name (name), "Account name ${n} is invalide", ("n", name));
    }

    inline void validate_permlink(const string& permlink){
        FC_ASSERT(permlink.size() < ACHAIN_MAX_PERMLINK_LENGTH, "Permlink is too long.");
        FC_ASSERT(fc::is_utf8(permlink), "Permlink is not formatted in UTF8");
    }

    struct account_create_opration :public base_operation{
        asset fee;
        acount_name_type creator;
        account_name_type new_account_name;
        authority owner;
        authority active;
        authority posting;
        public_key_type memo_key;
        string json_metadata;

        void validate() const;
        void get_required_active_authorities(flat_set<account_name_type>& a) const{ a.insert(creator);}
    };

    struct account_create_with_delegation_operation : public base_operation
    {
        asset             fee;
        asset             delegation;
        account_name_type creator;
        account_name_type new_account_name;
        authority         owner;
        authority         active;
        authority         posting;
        public_key_type   memo_key;
        string            json_metadata;

        extensions_type   extensions;

        void validate()const;
        void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(creator); }
    };

    struct account_update_operation : public base_operation
    {
        account_name_type             account;
        optional< authority >         owner;
        optional< authority >         active;
        optional< authority >         posting;
        public_key_type               memo_key;
        string                        json_metadata;

        void validate()const;

        void get_required_owner_authorities( flat_set<account_name_type>& a )const
        { if( owner ) a.insert( account ); }

        void get_required_active_authorities( flat_set<account_name_type>& a )const
        { if( !owner ) a.insert( account ); }
    };

    struct comment_operation: public base_operation{
        account_name_type parent_author;
        string parent_permlink;

        account_name_type author;
        string permlink;

        string title;
        string body;
        string json_metadata;

        void validte() const;
        void get_required_posting_authorities(flat_set<account_name_type>& a) const{a.insert(author);}
    };

    struct beneficiary_route_type
    {
        beneficiary_route_type() {}
        beneficiary_route_type( const account_name_type& a, const uint16_t& w ) : account( a ), weight( w ){}

        account_name_type account;
        uint16_t          weight;

        // For use by std::sort such that the route is sorted first by name (ascending)
        bool operator < ( const beneficiary_route_type& o )const { return account < o.account; }
    };

    struct comment_payout_beneficiaries
    {
        vector< beneficiary_route_type > beneficiaries;

        void validate()const;
    };

    typedef static_variant<
                comment_payout_beneficiaries
            > comment_options_extension;

    typedef flat_set< comment_options_extension > comment_options_extensions_type;



}}
