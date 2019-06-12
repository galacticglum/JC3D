/**
 * Author: Shon Verch
 * File Name: OpenGLFramebuffer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/19
 * Modified Date: 06/12/19
 * Description: OpenGL implementation of the Framebuffer class.
 */

#pragma once

#include <Renderer/Framebuffer.h>

/**
 * @class OpenGLFramebuffer OpenGLFramebuffer.h
 * @brief OpenGL implementation of the Framebuffer class.
 */
class OpenGLFramebuffer final : public Framebuffer
{
public:
	OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferFormat format);
	virtual ~OpenGLFramebuffer();

	void Bind() const override;
	void Unbind() const override;

	void Resize(uint32_t width, uint32_t height) override;
	void BindTexture(uint32_t slot) const override;

	uint32_t GetHandle() const override
	{
		return m_FramebufferId;
	}

	uint32_t GetColorAttachmentHandle() const override
	{
		return m_ColorAttachment;
	}

	uint32_t GetDepthAttachmentHandle() const override
	{
		return m_DepthAttachment;
	}

	uint32_t GetWidth() const
	{
		return m_Width;
	}

	uint32_t GetHeight() const
	{
		return m_Height;
	}

	FramebufferFormat GetFormat() const
	{
		return m_Format;
	}
private:
	uint32_t m_FramebufferId = 0;
	uint32_t m_Width, m_Height;
	FramebufferFormat m_Format;

	uint32_t m_ColorAttachment, m_DepthAttachment;
};