#pragma once

#include "TotoGL/GPUPointer/Texture.hpp"
#include "TotoGL/Misc/Factory.hpp"
#include <fstream>

namespace TotoGL {

class Texture {
public:
    Texture() = default;
    Texture(const int& width, const int& height, const int& color_depth);
    Texture(std::ifstream&& file);

    void create(const int& width, const int& height, const int& color_depth);
    void load(std::ifstream&& file);

    void bind();
    static void unbind();

    // protected:
    GLuint id() { return _texture.id(); }

    int width() const { return _width; }
    int height() const { return _height; }
    int colorDepth() const { return _color_depth; }

private:
    TextureId _texture;
    int _width, _height, _color_depth;

    friend class BufferTexture;
};

using TextureFactory = Factory<Texture>;
using TextureInstanceId = ObjectInstanceId<Texture>;

} // namespace TotoGL
