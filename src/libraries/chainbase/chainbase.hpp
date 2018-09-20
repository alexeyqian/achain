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

#ifndef CHAINBASE_NUM_RW_LOCKS
#define CHAINBASE_NUM_RW_LOCKS 10
#endif

#ifdef CHAINBASE_CHECK_LOCKING
#define CHAINBASE_REQUIRE_READ_LOCK(m, t) require_read_lock(m, typeid(t).name())
#define CHAINBASE_REQUIRE_WRITE_LOCK(m, t) require_write_lock(m, typeid(t).name())
#else
#define CHAINBASE_REQUIRE_READ_LOCK(m, t)
#define CHAINBASE_REQUIRE_WRITE_LOCK(m, t)
#endif

namespace chainbase {
    namespce bip = boost::interprocess;
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

    template<typename T>
    class oid{
    public:
        oid(int64_t i = 0): _id(i){}
        oid& operator++(){++id; return *this;}

        friend bool operator < ( const oid& a, const oid& b ) { return a._id < b._id; }
        friend bool operator > ( const oid& a, const oid& b ) { return a._id > b._id; }
        friend bool operator == ( const oid& a, const oid& b ) { return a._id == b._id; }
        friend bool operator != ( const oid& a, const oid& b ) { return a._id != b._id; }

        int64_t _id = 0;
    };

    template<uint16_t TypeNumber, typename Derived>
    struct object{
        typedef oid<Derived> id_type;
        static const uint16_t type_id = TypeNumber;
    };

    template<typename T>
    struct get_index_type{};

#define CHAINBASE_SET_INDEX_TYPE(OBJECT_TYPE, INDEX_TYPE) \
    namespace chainbase{template<> struct get_index_type<OBJECT_TYPE> {typedef INDEX_TYPE type;};}

#define CHAINBASE_DEFAULT_CONSTRUCTOR(OBJECT_TYPE) \
    template<typename Constructor, typename Allocator> OBJECT_TYPE(Constructor& c, Allocator&&){c(*this);}

    template<typename value_type>
    class undo_state{
    public:
        typedef typename value_type::id_type id_type;
        typedef allocator<std::pair<const id_type, value_type>> id_value_allocator_type;
        typedef allocator<id_type> id_allocator_type;

        template<typename T>
                undo_state(allocator<T> al)
                        : old_values(id_value_allocator_type(al.get_segment_manager())),
        removed_values(id_value_allocator_type(al.get_segment_manager())),
        new_ids(id_allocator_type(al.get_segment_manager())){}

        typedef boost::interprocess::map<id_type, value_type, std::less<id_type>, id_value_allocator_type> id_value_type_map;
        typedef boost::interprocess::set<id_type, std::less<id_type>, id_allocator_type> id_type_set;

        id_value_type_map old_values;
        id_value_type_map removed_values;
        id_type_set new_ids;
        id_type old_next_id = 0;
        int64_t revision = 0;
    };

    /**
    * The code we want to implement is this:
    *
    * ++target; try { ... } finally { --target }
    *
    * In C++ the only way to implement finally is to create a class
    * with a destructor, so that's what we do here.
    */
    class int_incrementer{
    public:
        int_incrementer(int32& target): _target(target){
            ++_target;
        }
        ~int_incrementer(){
            --_target;
        }

        int32_t get()const {
            return _target;
        }
    private:
        int32_t& _target;
    };

    /**
    *  The value_type stored in the multiindex container must have a integer field with the name 'id'.  This will
    *  be the primary key and it will be assigned and managed by generic_index.
    *
    *  Additionally, the constructor for value_type must take an allocator
    */
    template<typename MultiIndexType>
    class generic_index{
        typedef bip::managed_mapped_file::segment_manager segment_manager_type;
        typedef MultiIndexType index_type;
        typedef typename index_type::value_type value_type;
        typedef bip::allocator<generic_index, segment_manager_type> allocator_type;
        typedef undo_state<value_type> undo_state_type;

    private:


        boost::interprocess::deque<undo_state_type, allocator<undo_state_type>> _stack;

        /**
          *  Each new session increments the revision, a squash will decrement the revision by combining
          *  the two most recent revisions into one revision.
          *
          *  Commit will discard all revisions prior to the committed revision.
          */
        int64_t _revision = 0;
        typename value_type::id_type _next_id = 0;
        index_type _indices;
        uint32_t _size_of_value_type = 0;
        uint32_t _size_of_this = 0;

    };

    class abstract_session{

    };
}