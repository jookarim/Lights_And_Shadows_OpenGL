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

    void RenderCommand::ApplyRenderState(const RenderState& renderState)
    {
        if (renderState.depthTest)
        {
            glEnable(GL_DEPTH_TEST);
        }

        else
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (renderState.depthWrite)
        {
            glDepthMask(GL_TRUE);
        }

        else
        {
            glDepthMask(GL_FALSE);
        }

        if (renderState.depthFunc == DepthFunc::Less)
        {
            glDepthFunc(GL_LESS);
        }

        else if (renderState.depthFunc == DepthFunc::LessEqual)
        {
            glDepthFunc(GL_LEQUAL);
        }

        else if (renderState.depthFunc == DepthFunc::Greater)
        {
            glDepthFunc(GL_GREATER);
        }

        if (renderState.cullEnabled)
        {
            glEnable(GL_CULL_FACE);
        }

        else
        {
            glDisable(GL_CULL_FACE);
        }

        if (renderState.cullMode == CullMode::Back)
        {
            glCullFace(GL_BACK);
        }

        else if (renderState.cullMode == CullMode::Front)
        {
            glCullFace(GL_FRONT);
        }
    }
}
