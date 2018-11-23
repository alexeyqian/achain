#include <iostream>
#include <string>
#include <vector>

#include "application.hpp"

namespace achain {
    namespace app {

        using std::cout;
        using std::string;
        using std::vector;

        namespace bpo = boost::program_options;

        namespace detail {

            class application_impl {
            public:
                application_impl(application *self) : _self(self) {}

                ~application_impl() {}

                void initialize_plugins(){

                }
                void startup() {

                }

                void shutdown() {

                }


                application *_self;
                string _data_dir;
                const boost::program_options::variables_map* _options = nullptr;
                bool _running;
            };


        }

        application::application() : my(new detail::application_impl(this)) {}

        application::~application() {
            // release resources here ...
        }

        void application::set_program_options(bpo::options_description &command_line_options,
                                              bpo::options_description &configuration_file_options) {
            vector<string> default_plugins;
            default_plugins.push_back("witness");
            default_plugins.push_back("account_history");
            default_plugins.push_back("account_by_key");

            configuration_file_options.add_options()
                    ("p2p-endpoint", bpo::value<string>(), "Endpoint for P2P node to listen on")
                    ("p2p-max-connections", bpo::value<uint32_t>(), "Maximum number of incoming connections on P2P endpoint")
                    ;

            command_line_options.add(configuration_file_options);
            command_line_options.add_options()
                    ("replay", "Rebuild object graph by replaying all blocks")
                    ("resync", "Delete all blocks and re-sync with network from scratch")
                    ;

            command_line_options.add(_cli_options);
            configuration_file_options.add(_cfg_options);
        }

        void application::initialize(const std::string& data_dir, const bpo::variables_map& options) {
            my->_data_dir = data_dir;
            my->_options = &options;
        }

        void application::initialize_plugins(const bpo::variables_map& options) {
            cout << "initialize plugins";
            //my->initialize_plugins();
        }

        void application::startup() {
            cout << "application starting...";
            try {
                my->startup();
            } catch (...) {
                //elog("unexpected exception");
                throw;
            }
        }



    }
}