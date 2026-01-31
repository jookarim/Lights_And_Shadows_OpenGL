#include <GL/glew.h>
#include "Texture.h"
#include <vector>

namespace ke
{
	struct RenderTargetDesc
	{
		std::vector<Texture*> colorAttachments;
		Texture* depthAttachment = nullptr;
		int width = 800, height = 600;
	};

	class RenderTarget
	{
	private:
		GLuint m_id{};

		void createRenderTarget();
		void destroyRenderTarget() noexcept;
	public:
		RenderTarget();
		~RenderTarget() noexcept;

		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;

		RenderTarget(RenderTarget&& other) noexcept;
		RenderTarget& operator=(RenderTarget&& other) noexcept;

		void attachData(const RenderTargetDesc& desc);

		void bind() const;
	};
}