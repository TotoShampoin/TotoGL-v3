#pragma once

#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/RenderObject/Light.hpp"
#include "TotoGL/RenderObject/RenderGroup.hpp"
#include "TotoGL/RenderObject/RenderObject.hpp"
#include "TotoGL/RenderObject/Skydome.hpp"
#include <functional>
#include <variant>

namespace TotoGL {

using SceneComponentReference = std::variant<
    std::reference_wrapper<Light>,
    std::reference_wrapper<RenderObject>,
    std::reference_wrapper<RenderGroup>,
    std::reference_wrapper<Skydome>>;
using SceneComponentInstanceId = std::variant<
    LightInstanceId,
    RenderObjectInstanceId,
    RenderGroupInstanceId,
    SkydomeInstanceId>;

/**
 * @brief A collection of RenderObjects and Lights. You can just pass it to the Renderer and it will render everything.
 *
 */
class Scene {
public:
    std::vector<SceneComponentInstanceId>& sceneComponentsIds() { return _scene_components; }
    const std::vector<SceneComponentInstanceId>& sceneComponentsIds() const { return _scene_components; }

    std::vector<SceneComponentReference> sceneComponents() const;

    Scene& add(const SceneComponentInstanceId& id);

    Scene& clear();

private:
    std::vector<SceneComponentInstanceId> _scene_components;
};

using SceneFactory = Factory<Scene>;
using SceneInstanceId = ObjectInstanceId<Scene>;

} // namespace TotoGL
