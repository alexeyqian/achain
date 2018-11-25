#include "include/object.hpp"
#include "include/environment_check.hpp"
#include "include/read_write_mutex_manager.hpp"
#include "include/generic_index.hpp"
#include "include/database.hpp"

namespace chainbase {

    namespace bfs = boost::filesystem;
    namespace bip = boost::interprocess;

    void database::open(const boost::filesystem::path &dir, uint32_t flags, uint64_t shared_file_size) {
        bool write = flags & database::read_write;

        if (!bfs::exists(dir) && !write)
            BOOST_THROW_EXCEPTION(std::runtime_error("database file not found at" + dir.native()));

        bfs::create_directories(dir);

        if (_data_dir != dir) close();
        _data_dir = dir;

        // step 1: process shared_memory.bin file
        auto abs_path = bfs::absolute(dir / "shared_memory.bin");

        if (bfs::exists(abs_path)) { // file exists
            if (write) { // read write

                auto existing_file_size = bfs::file_size(abs_path);
                if (shared_file_size > existing_file_size) { // grow
                    // Please, remember that no process should be modifying the file/shared memory
                    // while the growing/shrinking process is performed.
                    // Otherwise, the managed segment will be corrupted.
                    auto ok = bip::managed_mapped_file::grow(
                            abs_path.generic_string().c_str(),
                            shared_file_size - existing_file_size);
                    if (!ok)
                        BOOST_THROW_EXCEPTION(std::runtime_error("could not grow database file to requested size."));
                }

                _segment.reset(new bip::managed_mapped_file(
                        bip::open_only,
                        abs_path.generic_string().c_str()
                ));

            } else { // read only
                _segment.reset(new bip::managed_mapped_file(
                        bip::open_read_only,
                        abs_path.generic_string().c_str()
                ));
                _read_only = true;
            }

            // environment check
            auto env = _segment->find<environment_check>("environment");
            if (!env.first || !(*env.first == environment_check()))
                BOOST_THROW_EXCEPTION(
                        std::runtime_error("database created by a different compiler, build, or operating system."));

        } else { // file not exists
            _segment.reset(new bip::managed_mapped_file(
                    bip::create_only,
                    abs_path.generic_string().c_str(),
                    shared_file_size
            ));

            _segment->find_or_construct<environment_check>("environment")();
        }

        // step 2: process shared_memory.meta file
        abs_path = bfs::absolute(dir / "shared_memory.meta");

        if (bfs::exists(abs_path)) { // file exists
            _meta.reset(new bip::managed_mapped_file(
                    bip::open_only,
                    abs_path.generic_string().c_str()));

            _rw_manager = _meta->find<read_write_mutex_manager>("rw_manager").first;
            if (!_rw_manager)
                BOOST_THROW_EXCEPTION(std::runtime_error("could not find read write lock manager"));

        } else { // file not exists
            _meta.reset(new bip::managed_mapped_file(
                    bip::create_only,
                    abs_path.generic_string().c_str(),
                    sizeof(read_write_mutex_manager) * 2
            ));

            _rw_manager = _meta->find_or_construct<read_write_mutex_manager>("rw_manager")();
        }

        if (write) {
            // A file lock can't guarantee synchronization between threads of the same process
            // so just use file locks to synchronize threads from different processes.
            _flock = bip::file_lock(abs_path.generic_string().c_str());
            // try_lock
            // Effects: The calling thread tries to acquire exclusive ownership of the mutex without waiting.
            // If no other thread has exclusive, or sharable ownership of the mutex this succeeds.
            // Returns: If it can acquire exclusive ownership immediately returns true.
            // If it has to wait, returns false.
            // Throws: interprocess_exception on error.
            if (!_flock.try_lock())
                BOOST_THROW_EXCEPTION(std::runtime_error("could not gain write access to the shared memory file"));
        }

    }

    void database::flush() {
        if (_segment)
            _segment->flush();
        if (_meta)
            _meta->flush();
    }

    void database::close() {
        _segment.reset();
        _meta.reset();
        _data_dir = boost::filesystem::path();
    }

    void database::wipe(const boost::filesystem::path &dir) {
        _segment.reset();
        _meta.reset();

        boost::filesystem::remove_all(dir / "shared_memory.bin");
        boost::filesystem::remove_all(dir / "shared_memory.meta");

        _data_dir = boost::filesystem::path();
        _index_list.clear();
        _index_map.clear();
    }

    void database::undo() {
        for (auto &item : _index_list)
            item->undo();
    }

    void database::squash() {
        for (auto &item: _index_list)
            item->squash();
    }

    void database::commit(int64_t revision) {
        for (auto &item : _index_list)
            item->commit(revision);
    }

    void database::undo_all() {
        for (auto &item: _index_list)
            item->undo_all();
    }

    // start a database session will start a index/table session for all indices/tables.
    database::session database::start_undo_session(bool enabled) {
        if (!enabled)
            return session();

        std::vector <std::unique_ptr<abstract_session>> _sub_sessions;
        _sub_sessions.reserve(_index_list.size());
        for (auto& item : _index_list)
            _sub_sessions.push_back(item->start_undo_session(enabled));

        return session(std::move(_sub_sessions));
    }


}