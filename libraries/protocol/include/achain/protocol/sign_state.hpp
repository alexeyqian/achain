#pragma once

#include <string>
#include <achain/protocol/types.hpp>
#include <achain/protocol/authority.hpp>

namespace achain { namespace protocol {
    struct sign_state{

        sign_state(const boost::flat_set<public_key_type>& sigs,
                    const authority_getter& a,
                    const boost::flat_set<public_key_type>& keys);

        /** returns true if we have a signature for this key or can
         * produce a signature for this key, else returns false.
         */
        bool signed_by(const public_key_type& k);
        bool check_authority(std::string id);

        /**
         *  Checks to see if we have signatures of the active authorites of
         *  the accounts specified in authority or the keys specified.
         */
        bool check_authority(const authority& au, uint32_t depth = 0);
        bool remove_unused_signatures();


        const authority_getter& get_active;
        const flat_set<public_key_type>& available_keys;

        boost::flat_map<public_key_type, bool> provided_signatures;
        boost::flat_set<std::string> approved_by;
        uint32_t max_recursion = ACHAIN_MAX_SIG_CHAECK_DEPTH;
    }
}}