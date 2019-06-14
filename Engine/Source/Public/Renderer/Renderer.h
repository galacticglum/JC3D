/**
 * Author: Shon Verch
 * File Name: Renderer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/10/2019
 * Modified Date: 06/10/2019
 * Description: The engine renderer.
 */

#pragma once

#include <Renderer/RenderCommandQueue.h>
#include <Renderer/RendererAPI.h>

/**
 * @class Renderer Renderer.h
 * @brief The engine renderer.
 */
class Renderer
{
public:
	/**
	 * @brief The function pointer for render commands.
	 */
	typedef void(*RenderCommandHandler)(void*);

	/**
	 * @brief Clear the screen.
	 */
	static void Clear();

	/**
	 * @brief Clear the screen given a colour.
	 */
	static void Clear(float r, float g, float b, float a = 1.0f);

	/**
	 * @brief Draw the contents of an index buffer.
	 */
	static void DrawIndexed(unsigned int count, bool depthTest = true);

	/**
	 * @brief Initialize the Renderer.
	 */
	static void Initialize();

	/**
	 * @brief Submit a render command.
	 */
	static void* Submit(const RenderCommandHandler func, const unsigned int size)
	{
		return s_Instance->m_CommandQueue.Allocate(func, size);
	}

	/**
	 * @brief Synchronous render call.
	 */
	static void WaitAndRender();

	/**
	 * @brief Get the current render instance.
	 */
	static Renderer& Get()
	{
		return *s_Instance;
	}
private:
	static Renderer* s_Instance;
	RenderCommandQueue m_CommandQueue;
};

/**
 * The following macros allow for a very pleasant render API by wrapping the ugly 
 * function binding code and render command calls in one single function.
 */

// Utilities to generate a unique name
#define ENGINE_RENDER_PASTE2(a, b) a ## b
#define ENGINE_RENDER_PASTE(a, b) ENGINE_RENDER_PASTE2(a, b)
#define ENGINE_RENDER_UNIQUE(x) ENGINE_RENDER_PASTE(x, __LINE__)

#define ENGINE_RENDER(code) \
    struct ENGINE_RENDER_UNIQUE(RenderCommand) \
    {\
        static void Execute(void*)\
        {\
            code\
        }\
    };\
	{\
		auto mem = ::Renderer::Submit(ENGINE_RENDER_UNIQUE(RenderCommand)::Execute, sizeof(ENGINE_RENDER_UNIQUE(RenderCommand)));\
		new (mem) ENGINE_RENDER_UNIQUE(RenderCommand)();\
	}\

#define ENGINE_RENDER_1(arg0, code) \
	do {\
    struct ENGINE_RENDER_UNIQUE(RenderCommand) \
    {\
		ENGINE_RENDER_UNIQUE(RenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((ENGINE_RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg0;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    };\
	{\
		auto mem = ::Renderer::Submit(ENGINE_RENDER_UNIQUE(RenderCommand)::Execute, sizeof(ENGINE_RENDER_UNIQUE(RenderCommand)));\
		new (mem) ENGINE_RENDER_UNIQUE(RenderCommand)(arg0);\
	} } while(0)

#define ENGINE_RENDER_2(arg0, arg1, code) \
    struct ENGINE_RENDER_UNIQUE(RenderCommand) \
    {\
		ENGINE_RENDER_UNIQUE(RenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((ENGINE_RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((ENGINE_RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg1;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    };\
	{\
		auto mem = ::Renderer::Submit(ENGINE_RENDER_UNIQUE(RenderCommand)::Execute, sizeof(ENGINE_RENDER_UNIQUE(RenderCommand)));\
		new (mem) ENGINE_RENDER_UNIQUE(RenderCommand)(arg0, arg1);\
	}\

#define ENGINE_RENDER_3(arg0, arg1, arg2, code) \
    struct ENGINE_RENDER_UNIQUE(RenderCommand) \
    {\
		ENGINE_RENDER_UNIQUE(RenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((ENGINE_RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((ENGINE_RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((ENGINE_RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg2;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    };\
	{\
		auto mem = ::Renderer::Submit(ENGINE_RENDER_UNIQUE(RenderCommand)::Execute, sizeof(ENGINE_RENDER_UNIQUE(RenderCommand)));\
		new (mem) ENGINE_RENDER_UNIQUE(RenderCommand)(arg0, arg1, arg2);\
	}\

#define ENGINE_RENDER_4(arg0, arg1, arg2, arg3, code) \
    struct ENGINE_RENDER_UNIQUE(RenderCommand) \
    {\
		ENGINE_RENDER_UNIQUE(RenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((ENGINE_RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((ENGINE_RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((ENGINE_RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((ENGINE_RENDER_UNIQUE(RenderCommand)*)argBuffer)->arg3;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
    };\
	{\
		auto mem = Renderer::Submit(ENGINE_RENDER_UNIQUE(RenderCommand)::Execute, sizeof(ENGINE_RENDER_UNIQUE(RenderCommand)));\
		new (mem) ENGINE_RENDER_UNIQUE(RenderCommand)(arg0, arg1, arg2, arg3);\
	}

#define ENGINE_RENDER_S(code) auto self = this;\
	ENGINE_RENDER_1(self, code)

#define ENGINE_RENDER_S1(arg0, code) auto self = this;\
	ENGINE_RENDER_2(self, arg0, code)

#define ENGINE_RENDER_S2(arg0, arg1, code) auto self = this;\
	ENGINE_RENDER_3(self, arg0, arg1, code)

#define ENGINE_RENDER_S3(arg0, arg1, arg2, code) auto self = this;\
	ENGINE_RENDER_4(self, arg0, arg1, arg2, code)