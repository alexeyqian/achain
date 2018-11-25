#pragma once

#include <cstdint>

#define CHAINBASE_SET_INDEX_TYPE(OBJECT_TYPE, INDEX_TYPE) \
    namespace chainbase{template<> struct get_index_type<OBJECT_TYPE> {typedef INDEX_TYPE type;};}

#define CHAINBASE_DEFAULT_CONSTRUCTOR(OBJECT_TYPE) \
    template<typename Constructor, typename Allocator> OBJECT_TYPE(Constructor& c, Allocator&&){c(*this);}

namespace chainbase {

    template<typename T>
    class oid {
    public:
        oid(int64_t i = 0) : _id(i) {}

        oid &operator++() {
            ++_id;
            return *this;
        }

        friend bool operator<(const oid &a, const oid &b) { return a._id < b._id; }

        friend bool operator>(const oid &a, const oid &b) { return a._id > b._id; }

        friend bool operator==(const oid &a, const oid &b) { return a._id == b._id; }

        friend bool operator!=(const oid &a, const oid &b) { return a._id != b._id; }

        int64_t _id = 0;
    };

    template<uint16_t TypeNumber, typename Derived>
    struct object {
        typedef oid<Derived> id_type;
        static const uint16_t type_id = TypeNumber;
    };

    template<typename T>
    struct get_index_type {
    };

    /**
    * The code we want to implement is this:
    *
    * ++target; try { ... } finally { --target }
    *
    * In C++ the only way to implement finally is to create a class
    * with a destructor, so that's what we do here.
    */
    class int_incrementer {
    public:
        int_incrementer(int32_t &target) : _target(target) {
            ++_target;
        }

        ~int_incrementer() {
            --_target;
        }

        int32_t get() const {
            return _target;
        }

    private:
        int32_t &_target;
    };

}