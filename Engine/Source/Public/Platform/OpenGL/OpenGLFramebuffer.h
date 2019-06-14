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
	/**
	 * @brief Initialize a new OpenGLFrameBuffer.
	 */
	OpenGLFramebuffer(uint32_t width, uint32_t height, FramebufferFormat format);

	/**
	 * @brief Destroy this OpenGLFrameBuffer.
	 */
	virtual ~OpenGLFramebuffer();

	/**
	 * @brief Bind this OpenGLFrameBuffer.
	 */
	void Bind() const override;

	/**
	 * @brief Unbind this OpenGLFrameBuffer.
	 */
	void Unbind() const override;

	/**
	 * @brief Resize this OpenGLFrameBuffer.
	 */
	void Resize(uint32_t width, uint32_t height) override;

	/**
	 * @brief Bind this OpenGLFrameBuffer texture to the specified unit.
	 */
	void BindTexture(uint32_t slot) const override;

	/**
	 * @brief Get a handle to this OpenGLFrameBuffer.
	 */
	uint32_t GetHandle() const override
	{
		return m_FramebufferId;
	}

	/**
	 * @brief Get a handle to the colour attachment of this OpenGLFrameBuffer.
	 */
	uint32_t GetColorAttachmentHandle() const override
	{
		return m_ColorAttachment;
	}

	/**
	 * @brief Get a handle to the depth attachment of this OpenGLFrameBuffer.
	 */
	uint32_t GetDepthAttachmentHandle() const override
	{
		return m_DepthAttachment;
	}

	/**
	 * @brief Get the width of this OpenGLFrameBuffer.
	 */
	uint32_t GetWidth() const
	{
		return m_Width;
	}

	/**
	 * @brief Get the height of this OpenGLFrameBuffer.
	 */
	uint32_t GetHeight() const
	{
		return m_Height;
	}

	/**
	 * @brief Get the format of this OpenGLFrameBuffer.
	 */
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