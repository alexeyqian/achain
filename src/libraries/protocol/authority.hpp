#pragma once

#include <vector>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>

#include "./types.hpp"

using namespace std;
using boost::container::flat_map;
using boost::container::flat_set;

namespace achain {
    namespace protocol {
        struct authority {

            authority(){}

            enum classification {
                owner = 0,
                active = 1,
                key = 2,
                posting = 3
            };

            template<class ...Args>
            authority(uint32_t threshold, Args... auths): weight_threshold(threshold) {
                add_authorities(auths...);
            }

            void add_authority(const public_key_type &k, weight_type w);

            void add_authority(const account_name_type &k, weight_type w);

            template<typename AuthType>
            void add_authorities(AuthType k, weight_type w) {
                add_authority(k, w);
            }

            template<typename AuthType, class ...Args>
            void add_authorities(AuthType k, weight_type w, Args... auths) {
                add_authority(k, w);
                add_authorities(auths...);
            };

            vector<public_key_type> get_keys() const;

            bool is_impossible() const;

            uint32_t num_auths() const;

            void clear();

            void validate()const;

            typedef flat_map<account_name_type, weight_type> account_authority_map;
            typedef flat_map<public_key_type, weight_type> key_authority_map;

            uint32_t weight_threshold = 0;
            account_authority_map account_auths;
            key_authority_map key_auths;

        };

        template<typename AuthorityType>
        void add_authority_accounts(flat_set<account_name_type>& result,const AuthorityType& a){
            for(auto& item : a.account_auths)
                result.insert(item.first);
        }

        bool is_valid_account_name(const string &name);

        bool operator==(const authority &a, const authority &b);

    }
}