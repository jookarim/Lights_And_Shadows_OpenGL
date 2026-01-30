#pragma once

#include <cstdint>

namespace ke
{
	struct AppConfig
	{
		static constexpr int WindowWidth = 1270;
		static constexpr int WindowHeight = 720;
		static constexpr const char* WindowTitle = "Window";
	};

	enum class TextureSlot : uint32_t
	{
		Albedo = 1
	};

	struct CameraDefaults
	{
		static constexpr float Fov = 45.f;
		static constexpr float Near = 1.f;
		static constexpr float Far = 1000.f;
	};
}
