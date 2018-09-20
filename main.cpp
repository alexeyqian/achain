#include <iostream>
#include <string>

#include <boost/version.hpp>
#include <boost/timer.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <boost/interprocess/allocators/node_allocator.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/offset_ptr.hpp>

#include <boost/container/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
using namespace boost::multi_index;

namespace bipc = ::boost::interprocess;
typedef bipc::managed_mapped_file managed_mapped_file_t;
typedef bipc::managed_mapped_file::segment_manager mapped_segment_manager_t;
typedef bipc::node_allocator<float, mapped_segment_manager_t> vec_allocator_t;
typedef bipc::vector<float, vec_allocator_t> vector_t;

struct Msg{
    Msg(const vec_allocator_t &vec_alloc) : score(vec_alloc){}

    uint32_t id;
    uint32_t age;
    vector_t score;
};

typedef std::pair<const uint32_t, Msg> pair_t;
typedef bipc::node_allocator<pair_t, mapped_segment_manager_t> allocator_t;
typedef std::less<uint32_t> less_t;

typedef bipc::map<uint32_t, Msg, less_t, allocator_t> msg_map_t;
typedef msg_map_t::iterator map_iter_t;



struct employee {

    employee(int id_, string name_, int age_)
            :id(id_), name(name_), age(age_){}

    friend ostream &operator<<(ostream &os, const employee &e) {
        os << e.id << " " << e.name << " " << e.age << endl;
        return os;
    }

    int id;
    string name;
    int age;
};

typedef boost::multi_index_container <
    employee,
    indexed_by<
        ordered_unique <member<employee, int, &employee::id> >,
        ordered_non_unique <member<employee, string, &employee::name> >,
        ordered_non_unique <member<employee, int, &employee::age> >
    >
> employee_container;

/*
typedef employee_container::nth_index<0>::type id_index;
typedef employee_container::nth_index<1>::type name_index;
typedef employee_container::nth_index<2>::type age_index;
*/

// example 2
struct index_by_id{};
struct index_by_name{};
struct index_by_age{};

typedef boost::multi_index_container <
employee,
indexed_by<
        ordered_unique <tag<index_by_id>, member<employee, int, &employee::id> >,
        ordered_non_unique <tag<index_by_name>, member<employee, string, &employee::name> >,
        ordered_non_unique <tag<index_by_age>, member<employee, int, &employee::age> >
        >
> employee_container2;


int main() {
    cout << "Hello, World!" << std::endl;

    cout << "====== BEGIN SYS INFO ======" << endl;
    cout << BOOST_VERSION << endl;
    cout << BOOST_LIB_VERSION << endl;
    cout << BOOST_PLATFORM << endl;
    cout << BOOST_COMPILER << endl;
    cout << BOOST_STDLIB << endl;
    cout << "====== END SYS INFO ======" << endl;

    string str("String Test");
    cout << str << endl;


    employee_container container;
    container.insert(employee(0, "Joe", 31));
    container.insert(employee(1, "Robert", 27));
    container.insert(employee(2, "John", 40));

    auto& ids = container.get<0>();
    copy(ids.begin(), ids.end(), ostream_iterator<employee>(cout));
    cout<<endl;

    auto& names = container.get<1>();
    copy(names.begin(), names.end(), ostream_iterator<employee>(cout));
    cout<<endl;

    auto& ages = container.get<2>();
    copy(ages.begin(), ages.end(), ostream_iterator<employee>(cout));
    cout<<endl;

    cout << "==== example 2===="<<endl;
    // example 3
    employee_container2 container2;
    container2.insert(employee(0, "Joe", 31));
    container2.insert(employee(1, "Robert", 27));
    container2.insert(employee(2, "John", 40));

    auto& idx_id = container2.get<index_by_id>();
    copy(idx_id.begin(), idx_id.end(), ostream_iterator<employee>(cout));
    cout<<endl;

    auto& idx_name = container2.get<index_by_name>();
    copy(idx_name.begin(), idx_name.end(), ostream_iterator<employee>(cout));
    cout<<endl;

    auto& idx_age = container2.get<index_by_age>();
    copy(idx_age.begin(), idx_age.end(), ostream_iterator<employee>(cout));
    cout<<endl;


    managed_mapped_file_t obj_mapped_file(bipc::open_or_create, "./data.mmap", 1024*1024);
    msg_map_t *p_msg_map = obj_mapped_file.find_or_construct<msg_map_t>("msg_map")
            (less_t(), obj_mapped_file.get_segment_manager());

    if(NULL == p_msg_map)
    {
        std:;cerr<<"canstruct msg_map failed"<<endl;
        return -1;
    }

    vec_allocator_t obj_alloc(obj_mapped_file.get_segment_manager());

    for(int i = 0; i < 10; ++i)
    {
        map_iter_t itr = p_msg_map->find(i);
        if(itr == p_msg_map->end())
        {
            std::cout<<"not find:"<<i<<" insert:"<<i<<std::endl;

            Msg msg(obj_alloc);
            msg.id = i;
            msg.age = 100 +i;
            msg.score.push_back(i);
            msg.score.push_back(i + 1);

            p_msg_map->insert(std::pair<uint32_t, Msg>(i, msg));
        }
        else
        {
            std::cout<<"find:"<<i<<" data:"<<itr->second.age;
            std::cout<<" score:";
            for(int j = 0; j < itr->second.score.size(); ++j)
                std::cout<<itr->second.score[j]<<" ";
            std::cout<<std::endl;
        }
    }

    return 0;
}