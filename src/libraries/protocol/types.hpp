#pragma once

#include<string>

using namespace std;
namespace achain {

    namespace protocol {
        typedef string public_key_type;
        typedef string account_name_type;
        typedef uint16_t weight_type;
        typedef string block_id_type;
        typedef string transaction_id_type;
        typedef string digest_type;
        typedef string signature_type;
        typedef string checksum_type;
        typedef string block_header_extensions_type;
        typedef uint32_t time_point_sec;
    }
}