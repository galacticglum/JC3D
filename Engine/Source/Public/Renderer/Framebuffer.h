/**
 * Author: Shon Verch
 * File Name: Framebuffer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/19
 * Modified Date: 06/12/19
 * Description: A framebuffer object.
 */

#pragma once
#include <cstdint>
#include <memory>
#include <vector>

/**
 * @enum FramebufferFormat Framebuffer.H
 * @brief The format of the Framebuffer.
 */
enum class FramebufferFormat
{
	None = 0,
	RGBA8 = 1,
	RGBA16F = 2
};

/**
 * @class Framebuffer Framebuffer.h
 * @brief A framebuffer object. 
 */
class Framebuffer
{
public:
	static Framebuffer* Create(uint32_t width, uint32_t height, FramebufferFormat format);

	virtual ~Framebuffer() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void Resize(uint32_t width, uint32_t height) = 0;
	virtual void BindTexture(uint32_t slot = 0) const = 0;

	virtual uint32_t GetHandle() const = 0;
	virtual uint32_t GetColorAttachmentHandle() const = 0;
	virtual uint32_t GetDepthAttachmentHandle() const = 0;
};

/**
 * @class FramebufferPool Framebuffer.h
 * @brief Main manager for all Framebuffers.
 */
class FramebufferPool final
{
public:
	explicit FramebufferPool(uint32_t maximumFramebuffers = 32);
	~FramebufferPool();

	std::weak_ptr<Framebuffer> AllocateBuffer();
	void Add(Framebuffer* framebuffer);

	const std::vector<Framebuffer*>& GetAll() const { return m_Pool; }

	static FramebufferPool* GetGlobal()
	{
		return s_Instance;
	}
private:
	std::vector<Framebuffer*> m_Pool;
	static FramebufferPool* s_Instance;
};
