#include "TotoGL/RenderObject/Scene.hpp"

#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/RenderObject/Light.hpp"
#include "TotoGL/RenderObject/RenderObject.hpp"
#include <variant>

namespace TotoGL {

std::vector<SceneComponentReference> Scene::sceneComponents() const {
    std::vector<SceneComponentReference> scene_components;
    for (auto& id : _scene_components) {
        if (std::holds_alternative<LightInstanceId>(id)) {
            scene_components.push_back(Factory<Light>::get(std::get<LightInstanceId>(id)));
        } else if (std::holds_alternative<RenderObjectInstanceId>(id)) {
            scene_components.push_back(Factory<RenderObject>::get(std::get<RenderObjectInstanceId>(id)));
        }
    }
    return scene_components;
}

Scene& Scene::add(const LightInstanceId& light) {
    _scene_components.push_back(light);
    return *this;
}

Scene& Scene::add(const RenderObjectInstanceId& object) {
    _scene_components.push_back(object);
    return *this;
}

Scene& Scene::clear() {
    _scene_components.clear();
    return *this;
}

} // namespace TotoGL
