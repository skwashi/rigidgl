/**
 * @author Jonas Ransjö
 */

#ifndef BATCH_H
#define BATCH_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "rtypes.h"
#include "math/movable3.h"
#include "../gl/vertexattrib.h"
#include "../gl/vertexbuffer.h"
#include "../gl/shaderprogram.h"
#include "../gl/renderable.h"

namespace rgraphics {

template <typename V>
class Batch : public rm::Movable3,
              public rgl::Renderable,
              public rgl::VBuffer<V>
{
public:

    enum State {
        IDLE = 0,
        BATCHING = 1
    };

    Batch(GLenum usage = GL_DYNAMIC_DRAW, GLenum mode = GL_TRIANGLES);

    Batch(uint vertexCapacity, uint indexCapacity,
          bool flushing = true,
          GLenum usage = GL_DYNAMIC_DRAW, GLenum mode = GL_TRIANGLES);

    void setColor(const Color& color);
    void setColor(float r, float g, float b, float a);
    void unsetColor();
    void begin() { state = BATCHING; }
    void end() { flush(); state = IDLE; }
    void finalize() { this->bufferData(GL_STATIC_DRAW); state = IDLE; }
    void flush() { render(); this->clear(); }

    using rgl::Renderable::updateMatrices;
    void updateMatrices();
    virtual void render(rgl::ShaderProgram& program);
    virtual void render();

protected:
    virtual void bind() const;
    virtual void flushCheck(uint newCount = 0);

    GLenum mode;
    uint vertexCapacity;
    uint indexCapacity;
    bool flushing;
    State state = IDLE;
    Color color;
    Color oldColor;
};

template <typename V>
inline Batch<V>::Batch(GLenum usage, GLenum mode)
{
    rgl::VBuffer<V>::VBuffer(usage);
    this->mode = mode;
    flushing = false;
}

template <typename V>
inline Batch<V>::Batch(uint vertexCapacity, uint IndexCapacity,
                       bool flushing,
                       GLenum usage, GLenum mode)
{
    rgl::VBuffer<V>::VBuffer(usage);
    this->indexCapacity = indexCapacity;
    this->vertexCapacity = vertexCapacity;
    reserve(vertexCapacity, indexCapacity);
    this->flushing = flushing;
    this->mode = mode;
}

template <typename V>
inline void Batch<V>::setColor(const Color& color)
{
    oldColor = color;
    this->color = color;
}

template <typename V>
inline void Batch<V>::setColor(float r, float g, float b, float a)
{
    oldColor = color;
    color = Color(r, g, b, a);
}

template <typename V>
inline void Batch<V>::unsetColor()
{
    color = oldColor;
}

template <typename V>
inline void Batch<V>::updateMatrices()
{
    this->modelMatrix = this->getMat4();
    this->normalMatrix = glm::mat3(this->modelMatrix);
}

template <typename V>
inline void Batch<V>::render(rgl::ShaderProgram& program)
{
    bind();
    updateUniforms(program);
    rgl::VBuffer<V>::render(mode);
}

template <typename V>
inline void Batch<V>::render()
{
    if (program != NULL) {
        program->use();
        render(program);
    } else {
        bind();
        rgl::VBuffer<V>::render(mode);
    }
}

template <typename V>
inline void Batch<V>::bind() const
{
    rgl::VBuffer<V>::bind();
}

template <typename V>
inline void Batch<V>::flushCheck(uint newCount)
{
    if (flushing && (this->vcount + newCount > vertexCapacity))
        flush();
}

} // namespace

#endif
