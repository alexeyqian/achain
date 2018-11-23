#pragma once

#include <typeinfo>
#include <cstring>

#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/set.hpp>
#include <boost/interprocess/containers/flat_map.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/interprocess_sharable_mutex.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/chrono.hpp>
#include <boost/config.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/throw_exception.hpp>

#include "database.hpp"

namespace chainbase {
    namespace bip = boost::interprocess;
    namespace bfs = boost::filesystem;
    using std::unique_ptr;
    using std::vector;

    template<typename T>
    using allocator = bip::allocator<T, bip::managed_mapped_file::segment_manager>;

    typedef bip::basic_string<char, std::char_traits<char>, allocator<char>> shared_string;

    template<typename T>
    using shared_vector = std::vector<T, allocator<T>>;

    struct strcmp_less {
        bool operator()(const shared_string &a, const shared_string &b) const {
            return less(a.c_str(), b.c_str());
        }

        bool operator()(const shared_string &a, const std::string &b) const {
            return less(a.c_str(), b.c_str());
        }

        bool operator()(const std::string &a, const shared_string &b) const {
            return less(a.c_str(), b.c_str());
        }

    private:
        inline bool less(const char *a, const char *b) const {
            return std::strcmp(a, b) < 0;
        }

    };

    // A mutex is an object which can be locked.
    // A lock is the object which maintains the lock.
    // To create a lock, you need to pass it a mutex.

    typedef boost::interprocess::interprocess_sharable_mutex read_write_mutex;
    typedef boost::interprocess::sharable_lock<read_write_mutex> read_lock;
    typedef boost::unique_lock<read_write_mutex> write_lock;


    template<typename value_type>
    class undo_state {
    public:
        typedef typename value_type::id_type id_type;
        typedef allocator<std::pair<const id_type, value_type>> id_value_allocator_type;
        typedef allocator<id_type> id_allocator_type;

        template<typename T>
        undo_state(allocator<T> al)
                : old_values(id_value_allocator_type(al.get_segment_manager())),
                  removed_values(id_value_allocator_type(al.get_segment_manager())),
                  new_ids(id_allocator_type(al.get_segment_manager())) {}

        typedef boost::interprocess::map<id_type, value_type, std::less<id_type>, id_value_allocator_type> id_value_type_map;
        typedef boost::interprocess::set<id_type, std::less<id_type>, id_allocator_type> id_type_set;

        id_value_type_map old_values;
        id_value_type_map removed_values;
        id_type_set new_ids;
        id_type old_next_id = 0;
        int64_t revision = 0;
    };





}