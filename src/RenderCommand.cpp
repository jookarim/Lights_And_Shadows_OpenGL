#include "RenderCommand.h"
#include <GL/glew.h>

namespace ke
{
    void RenderCommand::DrawIndexed(GLuint vao, uint32_t indexCount)
    {
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void RenderCommand::Clear(ClearCommand cmd)
    {
        GLbitfield mask = 0;

        if ((cmd & ClearCommand::Color) != ClearCommand::None)
        {
            mask |= GL_COLOR_BUFFER_BIT;
        }

        if ((cmd & ClearCommand::Depth) != ClearCommand::None)
        {
            mask |= GL_DEPTH_BUFFER_BIT;
        }

        if (mask != 0)
        {
            glClear(mask);
        }
    }

    void RenderCommand::ClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }
}
