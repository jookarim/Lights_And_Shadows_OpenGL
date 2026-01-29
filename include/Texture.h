#include <GL/glew.h>
#include <string>
#include <string_view>
#include <cstdint>
#include "stb/stb_image.h"
#include <stdexcept>

namespace ke
{	
	class Texture
	{
	private:
		GLuint m_id{};
		
		void loadFromFile(std::string_view path);
		void destroyTexture() noexcept;
	public:
		Texture(std::string_view path);
		~Texture() noexcept;

		void bind(uint32_t bindingPoint) const;

		GLuint getID() const { return m_id; }
	};
}