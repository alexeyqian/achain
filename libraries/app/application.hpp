#pragma once

#include <memory>
#include <boost/program_options.hpp>

namespace achain{
    namespace app{
        namespace detail{
            class application_impl;
        }

        class plugin;

        class application{
        public:
            application();
            ~application();

            void set_program_options(boost::program_options::options_description& command_line_options,
                    boost::program_options::options_description& configure);
            void initialize(const std::string& path, const boost::program_options::variables_map& options);
            void initialize_plugins(const boost::program_options::variables_map& options);
            void startup();
            void shutdown();
            void startup_plugins();
            void shutdown_plugins();

        private:
            std::shared_ptr<detail::application_impl> my;

            boost::program_options::options_description _cli_options;
            boost::program_options::options_description _cfg_options;

            const std::shared_ptr<plugin> null_plugin;
        };
    }
}