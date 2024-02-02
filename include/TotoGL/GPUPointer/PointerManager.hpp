#pragma once

#include <unordered_map>

namespace TotoGL {

template <typename ObjectType>
class PointerManager {
public:
    using IDType = unsigned int;
    using ObjectMap = std::unordered_map<IDType, ObjectType>;

    std::pair<IDType, ObjectType&> create() {
        IDType id = generateID();
        _objects[id] = ObjectType();
        return { id, _objects[id] };
    }

    void destroy(IDType id) {
        if (_objects.contains(id)) {
            _objects.erase(id);
        }
    }

private:
    IDType generateID() {
        static IDType id = 0;
        return id++;
    }
    ObjectMap _objects;
};

} // namespace TotoGL
