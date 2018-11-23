#pragma once

#include "read_write_mutex_manager.hpp"

class database{
public:
    enum open_flags{
        read_only = 0,
        read_write = 1
    };

    void open(const boost::filesystem::path& dir, uint32_t flags = read_only, uint64_t shared_file_size = 0);
    void close();
    void flush();
    void wipe(const boost::filesystem::path& dir);
    void set_require_locking(bool enable_require_locking);

    template<typename MultiIndexType>
    void add_index(){
        const uint16_t type_id = generic_index<MutiIndexType>::value_type::type_id;

    }
private:
    unique_ptr<boost::interprocess::managed_mapped_file> _segment;
    unique_ptr<boost::interprocess::managed_mapped_file> _meta;

    bool _readonly;
    boost::interprocess::file_lock _flock;
};