#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE "test_app"

#include <boost/test/included/unit_test.hpp>

#include "../libraries/app/application.hpp"

using namespace boost;
using namespace achain;
using namespace achain::app;

BOOST_AUTO_TEST_SUITE( test_app )

    BOOST_AUTO_TEST_CASE(t_app_startup)
    {
        application* node = new application();
        node->startup();
        BOOST_CHECK_EQUAL(16, 0x10);
    }

BOOST_AUTO_TEST_SUITE_END()