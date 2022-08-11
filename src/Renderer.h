#pragma once

class VertexArray;
class IndexBuffer;
class Shader;

//callback function GL calls when errors happen
void GLAPIENTRY errorOccurredGL(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar * message,
    const void* userParam);


class Renderer
{
private:
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};

