#pragma once

#include <cstdint>
#include <string>

namespace ke
{
	struct AppConfig
	{
		static constexpr int WindowWidth = 1270;
		static constexpr int WindowHeight = 720;
		static constexpr const char* WindowTitle = "Window";
		static constexpr const char* WindowIcon = "assets/images/pharaoh.png";
	};

	enum class TextureSlot : uint32_t
	{
		Albedo = 0,
		GrayScale = 0,
		Skybox = 0
	};

	struct CameraDefaults
	{
		static constexpr float Fov = 45.f;
		static constexpr float Near = 1.f;
		static constexpr float Far = 1000.f;
	};

	enum class TextureWrap
	{
		Repeat,
		ClampToEdge
	};

	enum TextureFilter
	{
		Linear,
		Nearest
	};
}
