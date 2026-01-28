#include <GL/glew.h>
#include <cstdint>

namespace ke
{
	enum class ClearCommand : uint32_t
	{
		None = 0,
		Color = 1 << 0,
		Depth = 1 << 1
	};
}

inline ke::ClearCommand operator|(ke::ClearCommand a, ke::ClearCommand b)
{
	return static_cast<ke::ClearCommand>(
		static_cast<uint32_t>(a) |
		static_cast<uint32_t>(b)
		);
}

inline ke::ClearCommand operator&(ke::ClearCommand a, ke::ClearCommand b)
{
	return static_cast<ke::ClearCommand>(
		static_cast<uint32_t>(a) &
		static_cast<uint32_t>(b)
		);
}

inline ke::ClearCommand& operator|=(ke::ClearCommand& a, ke::ClearCommand b)
{
	a = a | b;
	return a;
}

inline ke::ClearCommand& operator&=(ke::ClearCommand& a, ke::ClearCommand b)
{
	a = a & b;
	return a;
}


namespace ke
{
	class RenderCommand
	{
	public:
		static void DrawIndexed(GLuint vao, uint32_t indexCount);
		static void Clear(ClearCommand cmd);
		static void ClearColor(float r, float g, float b, float a);
	};
}
