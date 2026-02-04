#include "Skybox.h"
#include "stb/stb_image.h"
#include <stdexcept>

namespace ke
{
    static constexpr float SKYBOX_VERTICES[] =
    {
        -1,  1, -1,  -1, -1, -1,   1, -1, -1,
         1, -1, -1,   1,  1, -1,  -1,  1, -1,

        -1, -1,  1,  -1, -1, -1,  -1,  1, -1,
        -1,  1, -1,  -1,  1,  1,  -1, -1,  1,

         1, -1, -1,   1, -1,  1,   1,  1,  1,
         1,  1,  1,   1,  1, -1,   1, -1, -1,

        -1, -1,  1,  -1,  1,  1,   1,  1,  1,
         1,  1,  1,   1, -1,  1,  -1, -1,  1,

        -1,  1, -1,   1,  1, -1,   1,  1,  1,
         1,  1,  1,  -1,  1,  1,  -1,  1, -1,

        -1, -1, -1,  -1, -1,  1,   1, -1, -1,
         1, -1, -1,  -1, -1,  1,   1, -1,  1
    };

    static constexpr size_t SKYBOX_BUFFER_SIZE =
        36 * 3 * sizeof(float);

    Skybox::Skybox(const SkyboxDesc& desc)
    {
        loadFromFile(desc);
        createBuffers();
    }

    Skybox::~Skybox() noexcept
    {
        destroy();
    }

    Skybox::Skybox(Skybox&& other) noexcept
    {
        m_id = other.m_id;
        m_vao = other.m_vao;
        m_vbo = other.m_vbo;

        other.m_id = 0;
        other.m_vao = 0;
        other.m_vbo = 0;
    }

    Skybox& Skybox::operator=(Skybox&& other) noexcept
    {
        if (this == &other) return *this;

        destroy();

        m_id = other.m_id;
        m_vao = other.m_vao;
        m_vbo = other.m_vbo;

        other.m_id = 0;
        other.m_vao = 0;
        other.m_vbo = 0;

        return *this;
    }

    void Skybox::loadFromFile(const SkyboxDesc& desc)
    {
       if (desc.paths.size() != 6) throw std::runtime_error("Skybox paths count is incorrect");

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_id);

        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        
        for (int i = 0; i < 6; ++i)
        {

            glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, desc.minFilter == TextureFilter::Linear ? GL_LINEAR : GL_NEAREST);
            glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, desc.minFilter == TextureFilter::Nearest ? GL_LINEAR : GL_NEAREST);

            stbi_set_flip_vertically_on_load(true);

            int width, height, nrChannels;
            stbi_uc* data = stbi_load(desc.paths[i].c_str(), &width, &height, &nrChannels, 0);
            
            GLenum dataFormat;
            GLenum internalFormat;

            if (nrChannels == 1)
            {
                dataFormat = GL_RED;
                internalFormat = GL_R8;
            }

            else if (nrChannels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }

            else if (nrChannels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }

            else throw std::runtime_error("Texture format is not found: " + desc.paths[i]);

            if (i == 0)
            {
                glTextureStorage2D(m_id, 1, internalFormat, width, height);
            }

            glTextureSubImage3D(m_id, 0, 0, 0, i, width, height, 1, dataFormat, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
    }

    void Skybox::createBuffers()
    {
        glCreateVertexArrays(1, &m_vao);
        glCreateBuffers(1, &m_vbo);

        glNamedBufferStorage(m_vbo, SKYBOX_BUFFER_SIZE, SKYBOX_VERTICES, 0);

        glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, 3 * sizeof(float));

        glEnableVertexArrayAttrib(m_vao, 0);
        glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(m_vao, 0, 0);
    }

    void Skybox::destroy() noexcept
    {
        if (m_id)  glDeleteTextures(1, &m_id);
        if (m_vao) glDeleteVertexArrays(1, &m_vao);
        if (m_vbo) glDeleteBuffers(1, &m_vbo);

        m_id = m_vao = m_vbo = 0;
    }

    void Skybox::bind(TextureSlot slot) const
    {
        glBindTextureUnit(static_cast<GLuint>(slot), m_id);
    }
}
