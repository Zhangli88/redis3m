//
//  main.cpp
//  unittest
//
//  Created by Luca Marturana on 03/02/14.
//  Copyright (c) 2014 Luca Marturana. All rights reserved.
//

#include <redis3m/connection.h>

#define BOOST_TEST_MODULE redis3m
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/assign.hpp>

using namespace redis3m;

class test_connection
{
public:
    test_connection()
    {
        c = redis3m::connection::create(getenv("REDIS_HOST"));
        c->run_command(boost::assign::list_of("FLUSHDB"));
    }
    redis3m::connection::ptr_t c;
};

BOOST_AUTO_TEST_CASE ( fail_connect )
{
    BOOST_CHECK_THROW(connection::create("localhost", 9090), unable_to_connect);
}

BOOST_AUTO_TEST_CASE( correct_connection )
{
    BOOST_CHECK_NO_THROW(connection::create(getenv("REDIS_HOST")));
}

BOOST_AUTO_TEST_CASE( test_info)
{
    test_connection tc;

    redis3m::reply r = tc.c->run_command(boost::assign::list_of("INFO"));

}

BOOST_AUTO_TEST_CASE( test_ping)
{
    test_connection tc;
    tc.c->append_command(boost::assign::list_of("PING"));
    reply r = tc.c->get_reply();
    BOOST_CHECK_EQUAL(r.str(), "PONG");
}

BOOST_AUTO_TEST_CASE( set_get)
{
    test_connection tc;

    BOOST_CHECK_EQUAL("", tc.c->run_command(boost::assign::list_of("GET")("foo")).str());
    BOOST_CHECK_NO_THROW(tc.c->run_command(boost::assign::list_of("SET")("foo")("bar")));
    BOOST_CHECK_EQUAL("bar", tc.c->run_command(boost::assign::list_of("GET")("foo")).str());
}
