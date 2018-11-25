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
}