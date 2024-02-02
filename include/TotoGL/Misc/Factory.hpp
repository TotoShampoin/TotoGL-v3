#pragma once

#include <cstddef>
#include <map>

namespace TotoGL {

template <typename Type>
class Factory {
public:
    struct InstanceId {
        const size_t id;
        auto operator<=>(const InstanceId& other) const { return this->id <=> other.id; }
    };
    static constexpr auto NULL_INSTANCE = InstanceId { 0 };

    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    static Factory& get() {
        static Factory instance;
        return instance;
    }

    InstanceId createInstance(Type&& object = Type()) {
        auto id = nextId();
        _instances.emplace(id, std::move(object));
        return id;
    }
    void destroyInstance(const InstanceId& id) {
        if (!_instances.contains(id))
            return;
        if constexpr (std::is_pointer<Type>::value) {
            delete _instances.at(id);
        }
        _instances.erase(id);
    }
    Type& getInstance(const InstanceId& id) {
        return _instances.at(id);
    }

private:
    Factory() = default;

    std::map<InstanceId, Type> _instances;
    InstanceId nextId() {
        static size_t id = 1;
        return InstanceId { id++ };
    }
};

} // namespace TotoGL
