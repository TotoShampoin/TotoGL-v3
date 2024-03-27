#include "TotoGL/Primitives/Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "TotoGL/GPUPointer/Texture.hpp"
#include <fstream>

namespace TotoGL {

Texture::Texture(const int& width, const int& height, const int& color_depth)
    : Texture() {
    create(width, height, color_depth);
}

Texture::Texture(std::ifstream&& file)
    : Texture() {
    load(std::move(file));
}

void Texture::create(const int& width, const int& height, const int& color_depth) {
    _width = width;
    _height = height;
    _color_depth = color_depth;

    int gl_depth;
    switch (_color_depth) {
    case 1:
        gl_depth = GL_R;
        break;
    case 2:
        gl_depth = GL_RG;
        break;
    case 3:
        gl_depth = GL_RGB;
        break;
    case 4:
        gl_depth = GL_RGBA;
        break;
    }

    bind();
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, gl_depth,
        GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind();
}

void Texture::load(std::ifstream&& file) {
    auto str = std::string(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());
    auto cstr = str.c_str();
    stbi_set_flip_vertically_on_load(true);
    auto data = stbi_load_from_memory(reinterpret_cast<const u_char*>(cstr), str.size(), &_width, &_height, &_color_depth, 0);

    int gl_depth;
    switch (_color_depth) {
    case 1:
        gl_depth = GL_R;
        break;
    case 2:
        gl_depth = GL_RG;
        break;
    case 3:
        gl_depth = GL_RGB;
        break;
    case 4:
        gl_depth = GL_RGBA;
        break;
    }

    bind();
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, gl_depth,
        GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbind();
    stbi_image_free(data);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, _texture.id());
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
}

} // namespace TotoGL
