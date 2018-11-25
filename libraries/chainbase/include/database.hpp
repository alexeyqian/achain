#pragma once

#include <memory>

#include <boost/filesystem.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include "object.hpp"
#include "read_write_mutex_manager.hpp"
#include "generic_index.hpp"
#include "environment_check.hpp"

namespace chainbase {

#ifdef CHAINBASE_CHECK_LOCKING
#define CHAINBASE_REQUIRE_READ_LOCK(m, t) require_read_lock(m, typeid(t).name())
#define CHAINBASE_REQUIRE_WRITE_LOCK(m, t) require_write_lock(m, typeid(t).name())
#else
#define CHAINBASE_REQUIRE_READ_LOCK(m, t)
#define CHAINBASE_REQUIRE_WRITE_LOCK(m, t)
#endif

    // its a implementation of object database
    class database {
    public:

        // internal session for database
        struct session {
        public:
            session(session &&s) : _index_sessions(std::move(s._index_sessions)), _revision(s._revision) {}

            session(std::vector<std::unique_ptr<abstract_session>> &&s) : _index_sessions(std::move(s)) {
                if (_index_sessions.size())
                    _revision = _index_sessions[0]->revision();
            }

            ~session() {
                undo();
            }

            void push() {
                for (auto &i: _index_sessions) i->push();
                _index_sessions.clear();
            }

            void squash() {
                for (auto &i : _index_sessions) i->squash();
                _index_sessions.clear();
            }

            void undo() {
                for (auto &i : _index_sessions) i->undo();
                _index_sessions.clear();
            }

            int64_t revision() const { return _revision; }

        private:
            friend class database;

            session() {}

            // database session contains a vector of index/table sessions
            std::vector<std::unique_ptr<abstract_session>> _index_sessions;
            int64_t _revision = -1;
        };

        enum open_flags {
            read_only = 0,
            read_write = 1
        };

        void open(const boost::filesystem::path &dir, uint32_t flags = read_only, uint64_t shared_file_size = 0);

        void close();

        void flush();

        void wipe(const boost::filesystem::path &dir);

        session start_undo_session(bool enabled);

        int64_t revision() const {
            if (_index_list.size() == 0) return -1;
            return _index_list[0]->revision();
        }

        void set_revision(int64_t revision) {
            CHAINBASE_REQUIRE_WRITE_LOCK("set_revision", int64_t);
            for (auto i: _index_list) i->set_revision(revision);
        }

        void undo();

        void undo_all();

        void squash();

        void commit(int64_t revision);

        auto get_segment_manager() -> decltype(
        ((boost::interprocess::managed_mapped_file *) nullptr)
                ->get_segment_manager()) {
            return _segment->get_segment_manager();
        }

        // no exception throw version
        template<typename MultiIndexType>
        bool has_index() const {
            CHAINBASE_REQUIRE_READ_LOCK("get_index", typename MultiIndexType::value_type);
            typedef generic_index<MultiIndexType> index_type;
            return _index_map.size() > index_type::value_type::type_id
                   && _index_map[index_type::value_type::type_id];
        }

        // has_index with exception throw version
        template<typename MultiIndexType>
        void check_index_exists() const {
            if (!has_index<MultiIndexType>()) {
                typedef generic_index<MultiIndexType> index_type;
                std::string type_name = boost::core::demangle(typeid(typename index_type::value_type).name());
                BOOST_THROW_EXCEPTION(std::runtime_error("unable to find index for " + type_name + " in database"));
            }
            return;
        }

        // get an index from an object database
        // consider this as get a table from database
        template<typename MultiIndexType>
        const generic_index<MultiIndexType> &get_index() const {
            CHAINBASE_REQUIRE_READ_LOCK("get_index", typename MultiIndexType::value_type);
            typedef generic_index<MultiIndexType> index_type;
            typedef index_type *index_type_ptr;

            /*
            if(!has_index<MultiIndexType>()){
                std::string type_name = boost::core::demangle( typeid( typename index_type::value_type ).name() );
                BOOST_THROW_EXCEPTION( std::runtime_error( "unable to find index for " + type_name + " in database" ) );
            }*/
            check_index_exists<MultiIndexType>();

            return *index_type_ptr(_index_map[index_type::value_type::type_id]->get());
        }

        template<typename MultiIndexType, typename ByIndex>
        auto get_index() const -> decltype(((generic_index<MultiIndexType> *) (nullptr))->indicies().template get<ByIndex>()) {
            CHAINBASE_REQUIRE_READ_LOCK("get_index", typename MultiIndexType::value_type);
            typedef generic_index<MultiIndexType> index_type;
            typedef index_type *index_type_ptr;

            check_index_exists<MultiIndexType>();

            return index_type_ptr(
                    _index_map[index_type::value_type::type_id]->get())->indices().template get<ByIndex>();
        }

        template<typename MultiIndexType>
        generic_index<MultiIndexType> &get_mutable_index() {
            CHAINBASE_REQUIRE_WRITE_LOCK("get_mutable_index", typename MultiIndexType::value_type);
            typedef generic_index<MultiIndexType> index_type;
            typedef index_type *index_type_ptr;

            check_index_exists<MultiIndexType>();

            return *index_type_ptr(_index_map[index_type::value_type::type_id]->get());

        }

        template<typename ObjectType, typename IndexedByType, typename CompatibleKey>
        const ObjectType *find(CompatibleKey &&key) const {
            CHAINBASE_REQUIRE_READ_LOCK("find", ObjectType);
            typedef typename get_index_type<ObjectType>::type index_type;
            const auto &idx = get_index<index_type>().indices().template get<IndexedByType>();
            auto itr = idx.find(std::forward<CompatibleKey>(key));
            if (itr == idx.end()) return nullptr;
            return &*itr;
        }

        template<typename ObjectType>
        const ObjectType *find(oid<ObjectType> key = oid<ObjectType>()) const {
            CHAINBASE_REQUIRE_READ_LOCK("find", ObjectType);
            typedef typename get_index_type<ObjectType>::type index_type;
            const auto &idx = get_index<index_type>().indices();
            auto itr = idx.find(key);
            if (itr == idx.end()) return nullptr;
            return &*itr;
        }

        template<typename ObjectType, typename IndexedByType, typename CompatibleKey>
        const ObjectType &get(CompatibleKey &&key) const {
            CHAINBASE_REQUIRE_READ_LOCK("get", ObjectType);
            auto obj = find<ObjectType, IndexedByType>(std::forward<CompatibleKey>(key));
            if (!obj) BOOST_THROW_EXCEPTION(std::out_of_range("unknown key"));
            return *obj;
        }

        // get an object from object database by oid.
        // oid contains required info: table / index type, and id
        template<typename ObjectType>
        const ObjectType &get(const oid<ObjectType> &key = oid<ObjectType>()) const {
            CHAINBASE_REQUIRE_READ_LOCK("get", ObjectType);
            auto obj = find<ObjectType>(key);
            if (!obj) BOOST_THROW_EXCEPTION(std::out_of_range("unknown key"));
            return *obj;
        }

        // consider this function as add a table into database
        template<typename MultiIndexType>
        void add_index() {
            typedef generic_index<MultiIndexType> index_type;
            typedef typename index_type::allocator_type index_alloc;

            const uint16_t type_id = generic_index<MultiIndexType>::value_type::type_id;
            const std::string type_name = boost::core::demangle(typeid(typename index_type::value_type).name());
            if (!(_index_map.size() <= type_id || _index_map[type_id] == nullptr))
                BOOST_THROW_EXCEPTION(std::logic_error(type_name + "::type_id is already in use"));

            index_type *idx_ptr = nullptr;
            if (!_read_only) {
                idx_ptr = _segment->find_or_construct<index_type>(type_name.c_str())
                        (index_alloc(_segment->get_segment_manager()));
            } else {
                idx_ptr = _segment->find<index_type>(type_name.c_str()).first();
                if (!idx_ptr)
                    BOOST_THROW_EXCEPTION(
                            std::runtime_error("unable to find index for " + type_name + " in read only database"));

            }

            idx_ptr->validate();

            if (type_id >= _index_map.size())
                _index_map.resize(type_id + 1);

            auto new_index = new index<index_type>(*idx_ptr);
            _index_map[type_id].reset(new_index);
            _index_list.push_back(new_index);
        }

        template<typename MultiIndexType>
        void add_index_extension(std::shared_ptr<index_extension> ext) {
            typedef generic_index<MultiIndexType> index_type;

            /*
            if( !has_index< MultiIndexType >() )
            {
                std::string type_name = boost::core::demangle( typeid( typename index_type::value_type ).name() );
                BOOST_THROW_EXCEPTION( std::runtime_error( "unable to find index for " + type_name + " in database" ) );
            }*/
            check_index_exists<MultiIndexType>();


            _index_map[index_type::value_type::type_id]->add_index_extension(ext);
        }

        size_t get_free_memory() const {
            return _segment->get_segment_manager()->get_free_memory();
        }

        void set_require_locking(bool enable_require_locking) {
#ifdef CHAINBASE_CHECK_LOCKING
            _enable_require_locking = enable_require_locking;
#endif
        }

#ifdef CHAINBASE_CHECK_LOCKING
        void require_lock_fail(const char* method, const char* lock_type, const char* tname) const{
            std::string err_msg = "database::" + std::string( method )
                    + " require_" + std::string( lock_type )
                    + "_lock() failed on type " + std::string( tname );
            std::cerr << err_msg << std::endl;
            BOOST_THROW_EXCEPTION( std::runtime_error( err_msg ) );
        }

        void require_read_lock(const char* method, const char* tname)const{
            if(BOOST_UNLIKELY(
                    _enable_require_locking
                    & _read_only
                    & (_read_lock_count <= 0)
                    ))
                require_lock_fail(method, "read", tname);
        }

        void require_write_lock(const char* method, const char*tname){
            if( BOOST_UNLIKELY(
                    _enable_require_locking
                    & (_write_lock_count <= 0)
                    ))
                require_lock_fail(method, "write", tname);

        }

#endif

        template<typename ObjectType, typename Constructor>
        const ObjectType &create(Constructor &&con) {
            CHAINBASE_REQUIRE_WRITE_LOCK("create", ObjectType);
            typedef typename get_index_type<ObjectType>::type index_type;
            return get_mutable_index<index_type>().emplace(std::forward<Constructor>(con));
        }

        template<typename ObjectType, typename Modifier>
        void modify(const ObjectType &obj, Modifier &&m) {
            CHAINBASE_REQUIRE_WRITE_LOCK("modify", ObjectType);
            typedef typename get_index_type<ObjectType>::type index_type;
            get_mutable_index<index_type>().modify(obj, m);
        }

        template<typename ObjectType>
        void remove(const ObjectType &obj) {
            CHAINBASE_REQUIRE_WRITE_LOCK("remove", ObjectType);
            typedef typename get_index_type<ObjectType>::type index_type;
            return get_mutable_index<index_type>().remove(obj);
        }

        template<typename Lambda>
        auto with_read_lock(Lambda &&callback, uint64_t wait_micro = 1000000) -> decltype((*(Lambda * )

        nullptr)()){
            read_lock lock(_rw_manager->current_lock(), boost::interprocess::defer_lock_type());

#ifdef CHAINBASE_CHECK_LOCKING
            BOOST_ATTRIBUTE_UNUSED
            int_incrementer ii(_read_lock_count);
#endif

            if (!wait_micro) {
                lock.lock();
            } else {
                if (!lock.timed_lock(boost::posix_time::microsec_clock::universal_time() +
                                     boost::posix_time::microseconds(wait_micro)))
                    BOOST_THROW_EXCEPTION(std::runtime_error("unable to acquire lock"));

            }

            return callback();

        }

        template<typename Lambda>
        auto with_write_lock(Lambda &&callback, uint64_t wait_micro = 1000000) -> decltype((*(Lambda * )

    nullptr)()) {
            if (_read_only)
                BOOST_THROW_EXCEPTION(std::logic_error("coannot acquire write lock on read-only process"));

            // use _rw_manager->current_lock to get a read_write_mutex,
            // then construct a unique lock for write
            write_lock lock(_rw_manager->current_lock(), boost::defer_lock_t());

#ifdef CHAINBASE_CHECK_LOCKING
            BOOST_ATTRIBUTE_UNUSED
            int_incrementer ii(_write_lock_count);
#endif

            if (!wait_micro) {
                lock.lock();
            } else {
                while (!lock.timed_lock(boost::posix_time::microsec_clock::universal_time() +
                                        boost::posix_time::microseconds(wait_micro))) {
                    _rw_manager->next_lock();
                    std::cerr << "Lock timeout, moving to lock " << _rw_manager->current_lock_num() << std::endl;
                    lock = write_lock(_rw_manager->current_lock(), boost::defer_lock_t());
                }
            }

            return callback();
        }

        template<typename IndexExtensionType, typename Lambda>
        void for_each_index_extension(Lambda &&callback) const {
            for (const abstract_index *idx : _index_list) {
                const index_extensions &exts = idx->get_index_extensions();
                for (const std::shared_ptr<index_extension> &e : exts) {
                    std::shared_ptr<IndexExtensionType> e2 = std::dynamic_pointer_cast<IndexExtensionType>(e);
                    if (e2)
                        callback(e2);
                }
            }
        }


    private:
        // This is a sparse list of known indicies kept to accelerate creation of undo sessions
        // consider this as database tables
        std::vector<abstract_index *> _index_list;

        //This is a full map (size 2^16) of all possible index designed for constant time lookup
        // for better search permormance,
        // _index_map = index_list + index_extensions
        std::vector<std::unique_ptr<abstract_index>> _index_map;

        std::unique_ptr<boost::interprocess::managed_mapped_file> _segment;
        std::unique_ptr<boost::interprocess::managed_mapped_file> _meta;
        read_write_mutex_manager *_rw_manager = nullptr;

        boost::filesystem::path _data_dir;
        int32_t _read_lock_count = 0;
        int32_t _write_lock_count = 0;
        bool _enable_require_locking = false;
        boost::interprocess::file_lock _flock;
        bool _read_only = false;
    };

}