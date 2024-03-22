#pragma once

#include "TotoGL/GPUPointer/Texture.hpp"
#include "TotoGL/Misc/Factory.hpp"
#include "TotoGL/Primitives/Texture.hpp"
#include <functional>

namespace TotoGL {

class BufferTexture {
public:
    BufferTexture() = default;
    BufferTexture(const int& width, const int& height);

    void draw(const std::function<void()>& callback);

    void copy(const BufferTexture& other);

    TextureInstanceId texture() const { return _texture; }
    Texture& texture() { return *_texture; }

private:
    void bindFrameBuffer();
    static void unbindFrameBuffer();

    // void bindRenderBuffer();
    // static void unbindRenderBuffer();

    void bindTexture();
    static void unbindTexture();

    int _width, _height;
    FrameBufferId _framebuffer;
    RenderBufferId _renderbuffer;
    TextureInstanceId _texture;
};

using BufferTextureFactory = Factory<BufferTexture>;
using BufferTextureInstanceId = ObjectInstanceId<BufferTexture>;

} // namespace TotoGL
