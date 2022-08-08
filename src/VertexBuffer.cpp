#include "pch.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    //Allocates memeory on the VRAM of the graphics card for use   
    glGenBuffers(1, &m_RendererID);  //generates a buffer and returns and id
    //binds it, draw will call whatever is bound
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); //binds buffer as an array
    //sets data of buffer. 
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); //binds buffer as an array
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0); //unbinds buffer as an array
}
