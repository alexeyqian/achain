#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "test_chainbase"

#include <iostream>
#include <string>

#include <boost/test/included/unit_test.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/filesystem.hpp>

#include "../libraries/chainbase/include/chainbase.hpp"

using namespace boost;
using namespace boost::multi_index;

namespace bip = boost::interprocess;
namespace bfs = boost::filesystem;

struct book: public chainbase::object<0, book>{
    template<typename Constructor, typename Allocator>
    book(Constructor&& c, Allocator&& a){c(*this);}

    id_type id;
    int a = 0;
    int b = 1;
};

typedef multi_index_container<
        book,
        indexed_by<
                ordered_unique<member<book, book::id_type, &book::id> >,
                ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(book, int, a)>,
                ordered_non_unique<BOOST_MULTI_INDEX_MEMBER(book, int, b)>
                >,
        chainbase::allocator<book>
        > book_index;

CHAINBASE_SET_INDEX_TYPE(book, book_index)

BOOST_AUTO_TEST_SUITE( test_chainbase )

    BOOST_AUTO_TEST_CASE(t_open_and_create)
    {
        bfs::path temp = bfs::unique_path();
        try{

            std::cerr << temp.native() << std::endl;
            chainbase::database db;
            BOOST_CHECK_THROW(db.open(temp), std::runtime_error);

            db.open(temp, chainbase::database::read_write, 1024*1024*8);
            chainbase::database db2;
            db2.open(temp);
            // index does not exist in read only database
            /*
            BOOST_CHECK_THROW(db2.add_index<book_index>(), std::runtime_error);

            db.add_index< book_index >();
            // cannot add same index twice
            BOOST_CHECK_THROW( db.add_index<book_index>(), std::logic_error );

            // index should exist now
            db2.add_index<book_index>();*/

            BOOST_TEST_MESSAGE("Creating book");


        }catch(...){
            bfs::remove_all(temp);
            throw;
        }
    }

    BOOST_AUTO_TEST_CASE(t_chainbase_book)
    {


        BOOST_CHECK_EQUAL(16, 0x10);
    }

BOOST_AUTO_TEST_SUITE_END()