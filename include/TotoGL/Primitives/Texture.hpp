#define STB_IMAGE_IMPLEMENTATION
#include "TotoGL/GPUPointers/Texture.hpp"
#include <fstream>
#include <stb_image.h>

namespace TotoGL {

class Texture {
public:
    void load(std::ifstream&& file) {
        auto str = std::string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>());
        auto cstr = str.c_str();
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

    void bind() {
        glBindTexture(GL_TEXTURE_2D, _texture.id());
    }
    static void unbind() {
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }

private:
    TextureId _texture;
    int _width, _height, _color_depth;
};

} // namespace TotoGL
