#include "RenderTarget.h"

namespace ke
{
	void RenderTarget::createRenderTarget()
	{
		glCreateFramebuffers(1, &m_id);
	}

	void RenderTarget::destroyRenderTarget() noexcept
	{
		if (m_id)
		{
			glDeleteFramebuffers(1, &m_id);
			m_id = 0;
		}
	}

    void RenderTarget::attachData(const RenderTargetDesc& desc)
    {
        std::vector<GLenum> drawBuffers;

        for (size_t i = 0; i < desc.colorAttachments.size(); ++i)
        {
            Texture* tex = desc.colorAttachments[i];
            if (!tex)
                continue;

            glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i), tex->getID(), 0);

            drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i));
        }
        
        if (desc.depthAttachment)
        {
            glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, desc.depthAttachment->getID(), 0);
        }

        if (!drawBuffers.empty())
        {
            glNamedFramebufferDrawBuffers(m_id, static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data());
        }

        else
        {
            glNamedFramebufferDrawBuffers(m_id, 0, nullptr);
        }

        GLenum status = glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER);
    }

	RenderTarget::RenderTarget()
	{
		createRenderTarget();
	}

	RenderTarget::~RenderTarget() noexcept
	{
		destroyRenderTarget();
	}

	void RenderTarget::bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}
}