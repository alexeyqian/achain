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

typedef boost::interprocess::interprocess_sharable_mutex read_write_mutex;
typedef boost::interprocess::sharable_lock<read_write_mutex> read_lock;
typedef boost::unique_lock<read_write_mutex> write_lock;

class read_write_mutex_manager{
public:
    read_write_mutex_manager(){
        _current_lock = 0;
    }

    ~read_write_mutex_manager(){}

    void next_lock(){
        _current_lock++;
        new(& _locks[_current_lock % CHAINBASE_NUM_RW_LOCKS]) read_write_mutex();
    }

    read_write_mutex& current_lock(){
        return _locks[_current_lock % CHAINBASE_NUM_RW_LOCKS];
    }

    uint32_t current_lock_num(){
        return _current_lock;
    }

private:
    std::array<read_write_mutex, CHAINBASE_NUM_RW_LOCKS> _locks;
    std::atomic<uint32_t> _current_lock;
};