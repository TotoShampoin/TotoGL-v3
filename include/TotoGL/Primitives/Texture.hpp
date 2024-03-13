#pragma once

#include "TotoGL/GPUPointer/Texture.hpp"
#include "TotoGL/Misc/Factory.hpp"
#include <fstream>

namespace TotoGL {

class Texture {
public:
    Texture() = default;
    Texture(std::ifstream&& file);

    void load(std::ifstream&& file);

    void bind();
    static void unbind();

private:
    TextureId _texture;
    int _width, _height, _color_depth;
};

using TextureFactory = Factory<Texture>;
using TextureInstanceId = ObjectInstanceId<Texture>;

} // namespace TotoGL
