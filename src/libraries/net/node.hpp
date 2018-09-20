#pragma once

namespace graphene{
    namespace net{

        namespace detail{
            class node_impl;
            struct node_impl_deleter{
                void operator()(node_impl*);
            };
        }

        struct message_propagation_data{

        };

        class node_delegate{
        public:
            virtual ~node_delegate(){}

            virtual bool has_item(const net::item_id& id) = 0;
        };
    }
}