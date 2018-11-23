#include <iostream>
#include <string>

#include "chainbase.hpp"

namespace chainbase{
    struct environment_check{
        evironment_check(){

        }
    };

    void database::open(const bfs::path& dir, uint32_t flags, uint64_t shared_file_size) {
        bool write = flags & database::read_write;

        if(!bfs::exists(dir)){
            if(!write)
                BOOST_THROW_EXCEPTION(std::runtime_error("database file not found at" + dir.native()));
        }

        bfs::create_directories(dir);
        if(_data_dir != dir) close();

        _data_dir = dir;
        auto abs_path = bfs::absolute( dir / "shared_memory.bin");

        if(bfs::exists(abs_path)){ // file exists
            if(write){ // read write

                auto existing_file_size = bfs::file_size(abs_path);
                if(shared_file_size > existing_file_size){ // grow

                }

                _segment.reset(new bip::managed_mapped_file(
                        bip::open_only,
                        abs_path.generic_string().c_str()
                        ));

            }else{ // read only
                _segment.reset(new bip::managed_mapped_file(
                        bip::open_read_only,
                        abs_path.generic_string().c_str()
                ));
                _readonly = true;
            }

            // environment check

        }else{ // file not exists
            _segment.reset(new bip::managed_mapped_file(
                    bip::create_only,
                    abs_path.generic_string().c_str(),
                    shared_file_size
            ));

            _segment->find_or_construct<environment_check>("environment");
        }

        abs_path = bfs::absolute(dir / "shared_memory.meta");

        if(bfs::exists(abs_path)){ // file exists
            _meta.reset( new bip::managed_mapped_file(
                    bip::open_only,
                    abs_path.generic_string().c_str()
            ));

        }else{ // file not exists
            _meta.reset( new bip::managed_mapped_file(
                    bip::create_only,
                    abs_path.generic_string().c_str(),
                    sizeof( read_write_mutex_manager ) * 2
            ) );

        }

        if(wirte){

        }

    }


}