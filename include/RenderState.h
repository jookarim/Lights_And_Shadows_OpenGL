#pragma once

#include <glm/glm.hpp>

namespace ke
{
    enum class DepthFunc
    {
        Less,
        LessEqual,
        Greater
    };

    enum class CullMode
    {
        Back,
        Front
    };

    struct RenderState
    {
        bool depthTest = true;
        bool depthWrite = true;
        DepthFunc depthFunc = DepthFunc::Less;

        bool cullEnabled = true;
        CullMode cullMode = CullMode::Back;

        glm::vec4 clearColor = { 0.f, 0.f, 0.f, 0.f };
        
        RenderState() = default;
        ~RenderState() noexcept = default;

    };
}
