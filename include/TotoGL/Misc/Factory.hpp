#pragma once

#include <cstddef>
#include <map>
#include <stdexcept>

namespace TotoGL {

template <typename Type>
struct ObjectInstanceId {
    size_t id;
    bool operator==(const ObjectInstanceId& other) const { return id == other.id; }
    bool operator!=(const ObjectInstanceId& other) const { return id != other.id; }
    bool operator<(const ObjectInstanceId& other) const { return id < other.id; }
    bool operator>(const ObjectInstanceId& other) const { return id > other.id; }
    bool operator<=(const ObjectInstanceId& other) const { return id <= other.id; }
    bool operator>=(const ObjectInstanceId& other) const { return id >= other.id; }
};

/**
 * @brief This is a factory class to allocate objects in an ECS fashion.
 *
 * @tparam Type - The class type to be allocated.
 */
template <typename Type>
class Factory {
public:
    // using ObjectInstanceId = ObjectInstanceId<Type>;
    static constexpr auto NULL_INSTANCE = ObjectInstanceId<Type> { 0 };

    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    static ObjectInstanceId<Type> create(Type&& object_instance = Type()) {
        auto& factory = factoryInstance();
        auto id = factory.nextObjectInstanceId();
        factory._object_instances.emplace(id, std::move(object_instance));
        return id;
    }
    static void destroy(const ObjectInstanceId<Type>& id) {
        auto& factory = factoryInstance();
        if (!factory._object_instances.contains(id))
            return;
        if constexpr (std::is_pointer<Type>::value) {
            delete factory._object_instances.at(id);
        }
        factory._object_instances.erase(id);
    }
    static Type& get(const ObjectInstanceId<Type>& id) {
        if (id == NULL_INSTANCE)
            throw std::out_of_range("ObjectInstanceId points to null value");
        auto& factory = factoryInstance();
        if (!factory._object_instances.contains(id))
            throw std::out_of_range("ObjectInstanceId points to non-existing object");
        return factory._object_instances.at(id);
    }

    static Factory& factoryInstance() {
        static Factory instance;
        return instance;
    }

private:
    Factory() = default;
    ~Factory() = default;

    std::map<ObjectInstanceId<Type>, Type> _object_instances;
    ObjectInstanceId<Type> nextObjectInstanceId() {
        static size_t id = 1;
        return ObjectInstanceId<Type> { id++ };
    }
};

} // namespace TotoGL
