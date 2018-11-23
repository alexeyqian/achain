
#include<iostream>
#include<string>

//#include "../../src/libraries/protocol/config.hpp"
//#include "../../src/libraries/manifest/plugins.hpp"
#include "../../libraries/app/application.hpp"

using namespace std;

int main(int argc, char **argv) {

    cout << "Starting weku network" << endl;
    //cout << "initminer public key: " << INIT_PUBLIC_KEY_STR << endl;
    //cout << "chain id: " << CHAIN_ID << endl;
    //cout << "chain version: " << CHAIN_VERSION << endl;
    /*
    achain::plugin::initialize_plugin_factories();
    achain::app::application* node = new achain::app::application();

    for(const std::string& plugin_name: achain::plugin::get_available_plugins())
        node->register_abstract_plugin(achain::plugin::create_plugin(plugin_name, node));
    */

    application* node = new application();
    node->startup();

    return 0;
}