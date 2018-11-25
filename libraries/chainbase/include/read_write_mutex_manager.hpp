#pragma once

#include <array>
#include <atomic>
#include <memory>
#include <boost/thread/lock_types.hpp>
#include <boost/interprocess/sync/interprocess_sharable_mutex.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>

#ifndef CHAINBASE_NUM_RW_LOCKS
#define CHAINBASE_NUM_RW_LOCKS 10
#endif

// A mutex is an object which can be locked.
// A lock is the object which maintains the lock.
// To create a lock, you need to pass it a mutex.
typedef boost::interprocess::interprocess_sharable_mutex read_write_mutex;
typedef boost::interprocess::sharable_lock<read_write_mutex> read_lock;
typedef boost::unique_lock<read_write_mutex> write_lock;

class read_write_mutex_manager {
public:
    read_write_mutex_manager() {
        _current_lock_num = 0;
    }

    ~read_write_mutex_manager() {}

    void next_lock() {
        _current_lock_num++;
        new(&_locks[_current_lock_num % CHAINBASE_NUM_RW_LOCKS]) read_write_mutex();
    }

    read_write_mutex& current_lock() {
        return _locks[_current_lock_num % CHAINBASE_NUM_RW_LOCKS];
    }

    uint32_t current_lock_num() {
        return _current_lock_num;
    }

private:
    std::array<read_write_mutex, CHAINBASE_NUM_RW_LOCKS> _locks;
    std::atomic<uint32_t> _current_lock_num;
};