
#include<string>
#include<vector>
#include<boost/container>
#include <boost/container/flat_map.hpp>
#include "plugins.hpp"

namespace achain{
    namespace plugin{

        std::shared_ptr< achain::app::abstract_plugin > create_raw_block_plugin( achain::app::application* app );

        std::shared_ptr< achain::app::abstract_plugin > create_auth_util_plugin( achain::app::application* app );

        //TODO ...

        boost::container::flat_map<std::string,
                std::function<std::shared_ptr<achain::app::abstract_plugin>(achain::app::application* app)>> plugin_factories_by_name;

        void intialize_plugin_factories(){
            plugin_factories_by_name["raw_block"] = [](achain::app::application* app)
                    ->std::shared_ptr<achain::app::abstract_plugin>{
                return create_raw_block_plugin(app);
            };

            plugin_factories_by_name["auth_util"] = [](achain::app::application* app)
                    ->std::shared_ptr<achain::app::abstract_plugin>{
                return create_auth_util_plugin(app);
            };
        }


        std::shared_ptr<achain::app::abstract_plugin> create_plugin(
                const std::string& name, achain::app::application* ){
            auto it = plugin_factories_by_name.find(name);
            if(it== plugin_factories_by_name.end())
                return std::shared_ptr<achain::app::abstract_plugin>();
            return it->second(app);
        }

        std::vector<std::string> get_available_plugins(){
            std::vector<std::string> result;
            for(const auto& e: plugin_factories_by_name)
                result.push_back(e.first);
            return result;
        }

    }
}