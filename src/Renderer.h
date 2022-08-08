#pragma once

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
};

