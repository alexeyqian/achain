#pragma once

#include <cstdint>
#include <achain/protocol/config.hpp>
#include <achain/protocol/types.hpp>

namespace achain{ namespace protocol {
    typedef uint64_t asset_symbol_type;

    struct asset{
        asset(share_type a = 0, asset_symbol_type id = COIN_SYMBOL)
        :amount(a), symbol(id){}
        
        double to_real() const{
            return double(amount.value) / precision();
        }

        int64_t precision() const;
        
        uint8_t decimals() const;        
        void set_decimals(uint8_t d);

        std::string symbol_name() const;
        static asset from_string(const string& from);
        string to_string() const;

        asset& operator += (const asset& o){
            FC_ASSERT(symbol == o.symbol);
            amount += o.amount;
            return *this;
        }

        asset& operator -= (const asset& o){
            FC_ASSERT(symbol == o.symbol);
            amount -= o.amount;
            return *this;
        }

        asset operator -()const {return asset(-amount, symbol);}

        share_type amount;
        asset_symbol_type symbol;

        friend bool operator == (const asset&a, const asset& b){
            return std::tie(a.symbol, a.amount) == std::tie(b.symbol, b.amount);
        }
        friend bool operator < (const asset& a, const asset& b){
            FC_ASSERT(a.symbol == b.symbol);
            return std::tie(a.amount, a.symbol) < std::tie(b.amount, b.symbol);
        }
        friend bool operator <= (const asset& a, const asset& b){
            return (a == b) || (a < b);
        }
        friend bool operator != ( const asset& a, const asset& b )
        {
            return !(a == b);
        }
        friend bool operator > ( const asset& a, const asset& b )
        {
            return !(a <= b);
        }
        friend bool operator >= ( const asset& a, const asset& b )
        {
            return !(a < b);
        }

        friend asset operator + ( const asset& a, const asset& b )
        {
            FC_ASSERT( a.symbol == b.symbol );
            return asset( a.amount + b.amount, a.symbol );
        }
            friend asset operator - ( const asset& a, const asset& b )
        {
            FC_ASSERT( a.symbol == b.symbol );
            return asset( a.amount - b.amount, a.symbol );
        }
      
    };

    struct price{

        price(const asset& = asset(), const asset& quote = asset())
        :base(base), quote(quote){}

        static price max(asset_symbol_type base, asset_symbol_type quote);
        static price min(asset_symbol_type base, asset_symbol_type quote);

        price max() const{ return price::max(base.symbol, quote.symbol);}
        price min() const {return price::min(base.symbol, quote.symbol);}

        double to_real() const {return base.to_real() / quote.to_real(); }
        bool is_null() const;
        void validate() const;

        asset base;
        asset quote;
    };

    price operator / ( const asset& base, const asset& quote );
    inline price operator~( const price& p ) { return price{p.quote,p.base}; }

    bool  operator <  ( const asset& a, const asset& b );
    bool  operator <= ( const asset& a, const asset& b );
    bool  operator <  ( const price& a, const price& b );
    bool  operator <= ( const price& a, const price& b );
    bool  operator >  ( const price& a, const price& b );
    bool  operator >= ( const price& a, const price& b );
    bool  operator == ( const price& a, const price& b );
    bool  operator != ( const price& a, const price& b );
    asset operator *  ( const asset& a, const price& b );

}}

namespace fc {
    inline void to_variant( const achain::protocol::asset& var,  fc::variant& vo ) { vo = var.to_string(); }
    inline void from_variant( const fc::variant& var,  achain::protocol::asset& vo ) { vo = achain::protocol::asset::from_string( var.as_string() ); }
}

FC_REFLECT( achain::protocol::asset, (amount)(symbol) )
FC_REFLECT( achain::protocol::price, (base)(quote) )