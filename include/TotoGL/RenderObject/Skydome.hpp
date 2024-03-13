#pragma once

#include "TotoGL/RenderObject/RenderObject.hpp"

namespace TotoGL {

class Skydome {
public:
    Skydome();
    Skydome(Texture& texture);
    ~Skydome() = default;

    RenderObject& object() const { return RenderObjectFactory::get(_object); }

    void setTexture(Texture& texture);

private:
    RenderObjectInstanceId _object;
};

using SkydomeFactory = Factory<Skydome>;
using SkydomeInstanceId = ObjectInstanceId<Skydome>;

} // namespace TotoGL
