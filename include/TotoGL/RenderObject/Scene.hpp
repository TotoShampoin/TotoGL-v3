#pragma once

#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/RenderObject/Light.hpp"
#include "TotoGL/RenderObject/RenderObject.hpp"
#include <functional>
#include <variant>

namespace TotoGL {

using SceneComponentReference = std::variant<
    std::reference_wrapper<Light>,
    std::reference_wrapper<RenderObject>>;
using SceneComponentInstanceId = std::variant<
    LightInstanceId,
    RenderObjectInstanceId>;

class Scene {
public:
    std::vector<SceneComponentInstanceId>& sceneComponentsIds() { return _scene_components; }
    const std::vector<SceneComponentInstanceId>& sceneComponentsIds() const { return _scene_components; }

    std::vector<SceneComponentReference> sceneComponents() const {
        std::vector<SceneComponentReference> scene_components;
        for (auto& id : _scene_components) {
            if (std::holds_alternative<ObjectInstanceId<Light>>(id)) {
                scene_components.push_back(Factory<Light>::get(std::get<ObjectInstanceId<Light>>(id)));
            } else if (std::holds_alternative<ObjectInstanceId<RenderObject>>(id)) {
                scene_components.push_back(Factory<RenderObject>::get(std::get<ObjectInstanceId<RenderObject>>(id)));
            }
        }
        return scene_components;
    }

    Scene& add(const LightInstanceId& light) {
        _scene_components.push_back(light);
        return *this;
    }
    Scene& add(const RenderObjectInstanceId& object) {
        _scene_components.push_back(object);
        return *this;
    }

private:
    std::vector<SceneComponentInstanceId> _scene_components;
};

} // namespace TotoGL
