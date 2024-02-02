#pragma once

#include <GL/glew.h> // MUST STAY ON TOP

namespace TotoGL {

// using GPUPointersStructor = void(GLsizei, GLuint*);
using GPUPointerContructor = GLuint();
using GPUPointerDestructor = void(GLuint&);

template <
    GPUPointerContructor creater,
    GPUPointerDestructor destroyer>
class GPUPointer {
public:
    GPUPointer()
        : _id(creater()) { }
    ~GPUPointer() {
        destroyer(_id);
    }

    GPUPointer(GPUPointer&& other)
        : _id(other._id) {
        other._id = GL_NONE;
    }
    GPUPointer& operator=(GPUPointer&& other) {
        _id = other._id;
        other._id = GL_NONE;
        return *this;
    }

    GPUPointer(const GPUPointer&) = delete;
    GPUPointer& operator=(const GPUPointer&) = delete;

    const GLuint& id() const { return _id; }

private:
    GLuint _id;
};

} // namespace TotoGL
