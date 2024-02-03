#pragma once

#include <cstddef>
#include <map>

namespace TotoGL {

template <typename Type>
class Factory {
public:
    struct ObjectInstanceId {
        const size_t id;
        auto operator<=>(const ObjectInstanceId& other) const { return this->id <=> other.id; }
    };
    static constexpr auto NULL_INSTANCE = ObjectInstanceId { 0 };

    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    static ObjectInstanceId create(Type&& object_instance = Type()) {
        auto& factory = factoryInstance();
        auto id = factory.nextObjectInstanceId();
        factory._object_instances.emplace(id, std::move(object_instance));
        return id;
    }
    static void destroy(const ObjectInstanceId& id) {
        auto& factory = factoryInstance();
        if (!factory._object_instances.contains(id))
            return;
        if constexpr (std::is_pointer<Type>::value) {
            delete factory._object_instances.at(id);
        }
        factory._object_instances.erase(id);
    }
    static Type& get(const ObjectInstanceId& id) {
        auto& factory = factoryInstance();
        return factory._object_instances.at(id);
    }

    static Factory& factoryInstance() {
        static Factory instance;
        return instance;
    }

private:
    Factory() = default;
    ~Factory() = default;

    std::map<ObjectInstanceId, Type> _object_instances;
    ObjectInstanceId nextObjectInstanceId() {
        static size_t id = 1;
        return ObjectInstanceId { id++ };
    }
};

} // namespace TotoGL
