#pragma once

#include "TotoGL/RenderObject/RenderObject.hpp"

namespace TotoGL {

class Skydome {
public:
    Skydome();
    Skydome(Texture& texture);
    ~Skydome();

    RenderObject& object() const { return RenderObjectFactory::get(_object); }

    void setTexture(Texture& texture);

private:
    RenderObjectInstanceId _object;
};

} // namespace TotoGL
