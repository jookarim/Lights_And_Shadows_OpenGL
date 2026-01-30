#pragma once

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
    };
}
