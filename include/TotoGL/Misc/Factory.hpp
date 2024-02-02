#pragma once

#include <map>

namespace TotoGL {

template <typename Type>
class Factory {
public:
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    static Factory& get() {
        static Factory instance;
        return instance;
    }

    std::pair<size_t, Type&> createInstance(Type&& object = Type()) {
        auto id = nextId();
        _instances.emplace(id, std::move(object));
        return { id, _instances[id] };
    }
    void destroyInstance(const size_t& id) {
        if (!_instances.contains(id))
            return;
        if constexpr (std::is_pointer<Type>::value) {
            delete _instances.at(id);
        }
        _instances.erase(id);
    }
    Type& getInstance(const size_t& id) {
        return _instances[id];
    }

private:
    Factory() = default;

    std::map<size_t, Type> _instances;
    size_t nextId() {
        static size_t id = 0;
        return id++;
    }
};

} // namespace TotoGL
