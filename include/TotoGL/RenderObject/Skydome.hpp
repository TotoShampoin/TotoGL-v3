#pragma once

#include "TotoGL/RenderObject/RenderObject.hpp"

namespace TotoGL {

class Skydome {
public:
    Skydome();
    Skydome(Texture& texture);
    Skydome(Skydome&&);
    ~Skydome();

    RenderObject& object() const { return RenderObjectFactory::get(_object); }

    void setTexture(Texture& texture);

private:
    RenderObjectInstanceId _object;
    bool _is_owner = true;
};

using SkydomeFactory = Factory<Skydome>;
using SkydomeInstanceId = ObjectInstanceId<Skydome>;

} // namespace TotoGL
