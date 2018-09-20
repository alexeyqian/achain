#pragma once

#include <boost/program_options>

namespace achain{
    namespace app{
        namespace detail{
            class application_impl;
        }

        using std::string;

        class abstract_plugin;
        class plugin;
        class application;

        class network_broadcast_api;
        class login_api;

        class application{
        public:
            application();
            ~application();

        private:
            std::shared_ptr<detail::application_impl> my;

            boost::program_options::options_description _cli_options;
            boost::program_options::options_description _cfg_options;

            const std::shared_ptr<plugin> null_plugin;
        };
    }
}