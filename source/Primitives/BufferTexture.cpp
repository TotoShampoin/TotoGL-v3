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

void BufferTexture::draw(const std::function<void()>& callback) {
    bindFrameBuffer();
    glViewport(0, 0, _width, _height);
    callback();
    unbindFrameBuffer();
}

void BufferTexture::copy(const BufferTexture& other) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, other._framebuffer.id());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _framebuffer.id());
    glBlitFramebuffer(0, 0, other._width, other._height, 0, 0, _width, _height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    _width = other._width;
    _height = other._height;
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
