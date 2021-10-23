#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    :m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));//部分平台unsigned int长度不同
    GLCall(glGenBuffers(1, &m_RendererId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(GLuint), data, GL_STATIC_DRAW));
}
IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererId));
}
void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}
void IndexBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
