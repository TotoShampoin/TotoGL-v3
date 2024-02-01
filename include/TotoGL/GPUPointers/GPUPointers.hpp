#pragma once

#include <GL/glew.h> // MUST STAY ON TOP

namespace TotoGL {

// using GPUPointersStructor = void(GLsizei, GLuint*);
using GPUPointersContructor = GLuint();
using GPUPointersDestructor = void(GLuint&);

template <
    GPUPointersContructor creater,
    GPUPointersDestructor destroyer>
class GPUPointers {
public:
    GPUPointers()
        : _id(creater()) { }
    ~GPUPointers() {
        destroyer(_id);
    }

    GPUPointers(GPUPointers&& other)
        : _id(other._id) {
        other._id = GL_NONE;
    }
    GPUPointers& operator=(GPUPointers&& other) {
        _id = other._id;
        other._id = GL_NONE;
        return *this;
    }

    GPUPointers(const GPUPointers&) = delete;
    GPUPointers& operator=(const GPUPointers&) = delete;

    const GLuint& id() const { return _id; }

private:
    GLuint _id;
};

} // namespace TotoGL
