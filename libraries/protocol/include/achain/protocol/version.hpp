#pragma once

#include <cstdint>
#include <fc/string.hpp>
#include <fc/time.hpp>

namespace achain{ namespace protocol {

    /*
    * This class represents the basic versioning scheme of the blockchain.
    * All versions are a triple consisting of a major version, hardfork version, and release version.
    * It allows easy comparison between versions. A version is a read only object.
    */
    struct version{
        version(){}
        version(uint8_t m, uint8_t h, uint16_t r);
        virtual ~version(){}

        bool operator == (const version& o) const {return v_num == o.v_num;}
        bool operator != ( const version& o )const { return v_num != o.v_num; }
        bool operator <  ( const version& o )const { return v_num <  o.v_num; }
        bool operator <= ( const version& o )const { return v_num <= o.v_num; }
        bool operator >  ( const version& o )const { return v_num >  o.v_num; }
        bool operator >= ( const version& o )const { return v_num >= o.v_num; }

        operator fc::string() const;

        uint32_t v_num = 0;
    };

    struct hardfork_version : version{
        hardfork_version(): version(){}
        hardfork_version(uint8_t m, uint8_t h): version(m, h, 0){}
        hardfork_version(version v) {v_num = v.v_num & 0xFFFF0000;}
        ~hardfork_version(){}

        void operator = (const version& o){v_num = o.v_num & 0xFFFF0000;}
        void operator = (const hardfork_version& o){v_num = o.v_num & 0xFFFF0000;}

        bool operator == ( const hardfork_version& o )const { return v_num == o.v_num; }
        bool operator != ( const hardfork_version& o )const { return v_num != o.v_num; }
        bool operator <  ( const hardfork_version& o )const { return v_num <  o.v_num; }
        bool operator <= ( const hardfork_version& o )const { return v_num <= o.v_num; }
        bool operator >  ( const hardfork_version& o )const { return v_num >  o.v_num; }
        bool operator >= ( const hardfork_version& o )const { return v_num >= o.v_num; }

        bool operator == ( const version& o )const { return v_num == ( o.v_num & 0xFFFF0000 ); }
        bool operator != ( const version& o )const { return v_num != ( o.v_num & 0xFFFF0000 ); }
        bool operator <  ( const version& o )const { return v_num <  ( o.v_num & 0xFFFF0000 ); }
        bool operator <= ( const version& o )const { return v_num <= ( o.v_num & 0xFFFF0000 ); }
        bool operator >  ( const version& o )const { return v_num >  ( o.v_num & 0xFFFF0000 ); }
        bool operator >= ( const version& o )const { return v_num >= ( o.v_num & 0xFFFF0000 ); }
    };

    struct hardfork_version_vote{
        hardfork_version_vote(){}
        hardfork_version_vote(hardfork_version v, fc::time_point_sec t):hf_version(v), hf_time(t){}

        hardfork_version hf_version;
        fc::time_point_sec hf_time;
    };
}}

namespace fc{
    class variant;
    void to_variant(const achain::protocol::version& v, variant& var);
    void from_variant(const variant& var, achain::protocol::version& v);

    void to_variant( const achain::protocol::hardfork_version& hv, variant& var );
    void from_variant( const variant& var, achain::protocol::hardfork_version& hv );
}

#include <fc/reflect/reflect.hpp>
FC_REFLECT(achain::protocol::version, (v_num));
FC_REFLECT_DERIVED(achain::protocol::hardfork_version, (achain::protocol::version), )
FC_REFLECT(achain::protocol::hardfork_version_vote, (hf_version)(hf_time) )