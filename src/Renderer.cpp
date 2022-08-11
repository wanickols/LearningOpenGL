#include "pch.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "shader.h"

//callback function GL calls when errors happen
void GLAPIENTRY errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    printf("Message from OpenGL:\nSource: 0x%x\nType: 0x%x\n"
        "Id: 0x%x\nSeverity: 0x%x\n", source, type, id, severity);
    printf("%s\n", message);
    exit(-1);
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    shader.Bind();

    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}
