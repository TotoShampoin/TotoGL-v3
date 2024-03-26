#include "TotoGL/RenderObject/RenderGroup.hpp"

namespace TotoGL {

RenderGroup::RenderGroup(std::vector<RenderObjectInstanceId>&& objects)
    : _objects(std::move(objects)) { }

RenderGroup::~RenderGroup() {
    for (auto& object : _objects) {
        RenderObjectFactory::destroy(object);
    }
}

} // namespace TotoGL
