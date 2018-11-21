#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "test_one"

#include <boost/test/included/unit_test.hpp>
#include <boost/smart_ptr.hpp>

using namespace boost;

BOOST_AUTO_TEST_SUITE( test_suite1 )

BOOST_AUTO_TEST_CASE(t_scoped_ptr)
{
    BOOST_ASSERT(16 == 0X10);
    scoped_ptr<int> p(new int(874));
    BOOST_CHECK(p);
    BOOST_CHECK_EQUAL(*p, 874);

    p.reset();
    BOOST_CHECK(p == 0);
    BOOST_CHECK_EQUAL(16, 0x10);
    //BOOST_ERROR("Display an error message.");
}

BOOST_AUTO_TEST_SUITE_END()