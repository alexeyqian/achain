#include <list>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/lambda/lambda.hpp>

namespace achain{
    namespace datatype{
        struct id{};
        struct name{};

        struct employee{
            unsigned int id;
            std::string name;

            employee(unsigned int id, const std::string& name):id(id), name(name){}

            bool operator<  (const employee& e) const{return id < e.id;}
            bool operator<= (const employee& e) const{return id <= e.id;}

            friend std::ostream& operator<<(std::ostream& os, const employee& e);
        };

        std::ostream& operator<<(std::ostream& os, const employee& e){
            os << "[" << e.id << "]" << e.name.c_str();
            return os;
        }

        struct change_name{
            change_name(const std::string& new_name): new_name(new_name){}

            void operator()(employee& e){
                e.name = new_name;
            }

            private:
                std::string new_name;
        };

        typedef boost::multi_index::multi_index_container<
                emplyee,
                boost::multi_index::index_by<
                        boost::multi_index::ordered_unique<boost::multi_index::tag<id>, boost::multi_index::identity<employee>>,
                                >


                >employee_set;

    }
}