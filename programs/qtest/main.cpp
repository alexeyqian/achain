
#include<iostream>
#include<string>

#include <boost/version.hpp>
#include <boost/config.hpp>

using namespace std;

int main(int argc, char **argv) {

    cout << "Starting quick test..." << endl;

    cout << "Boost version: " << BOOST_VERSION << endl;
    cout << "Boost lib version: " << BOOST_LIB_VERSION << endl;

    cout << "The end." << endl;

}