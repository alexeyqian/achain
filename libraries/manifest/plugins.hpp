#pragma once

#include<string>
#include <vector>
#include<memory>

namespace achain{
    namespace app{
        class abstract_plugin;
        class application;
    }
}

namespace achain{
    namespace plugin{
        void initialize_plugin_factories();
        std::shared_ptr<achain::app::abstract_plugin> create_plugin(
                const std::string& name, achain::app::application* app);
        std::vector<std::string> get_available_plugins();
    }
}