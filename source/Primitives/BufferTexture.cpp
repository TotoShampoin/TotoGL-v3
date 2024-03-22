#include "TotoGL/Primitives/BufferTexture.hpp"
#include "TotoGL/Primitives/Texture.hpp"

namespace TotoGL {

BufferTexture::BufferTexture(const int& width, const int& height)
    : _width(width)
    , _height(height)
    , _texture(TextureFactory::create(Texture(width, height, 3))) {
    bindTexture();
    bindFrameBuffer();

    // bindRenderBuffer();
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer.id());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderbuffer.id());

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture->id(), 0);
    GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer is not complete!");
    }
    unbindFrameBuffer();
}

void BufferTexture::bind() {
    bindFrameBuffer();
    glViewport(0, 0, _width, _height);
}
void BufferTexture::unbind(int width, int height) {
    unbindFrameBuffer();
    glViewport(0, 0, width, height);
}

void BufferTexture::bindFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer.id());
}
void BufferTexture::unbindFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void BufferTexture::bindTexture() {
    // glBindTexture(GL_TEXTURE_2D, _texture.id());
    _texture->bind();
}
void BufferTexture::unbindTexture() {
    // glBindTexture(GL_TEXTURE_2D, 0);
    Texture::unbind();
}

} // namespace TotoGL
