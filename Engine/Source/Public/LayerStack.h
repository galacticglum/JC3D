/**
 * Author: Shon Verch
 * File Name: LayerStack.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: A stack data-structure consisting of Layers.
 */

#pragma once

#include <vector>
#include <Layer.h>

/**
 * @class LayerStack LayerStack.h
 * @brief A stack data-structure consisting of Layers.
 * @note The stack is essentially structured into two parts: layers and overlay layers.
 *	     Layers get pushed onto the fist half of the stack; overlay layers get pushed onto
 *		 the second half. This is because, we always want the overlay layers to be rendered
 *		 after the regular Layers. Therefore, if we push an overlay, we want to ensure that it
 *	     is after the Layers hence putting it at the very end. Likewise, if we push a regular Layer,
 *		 we want to ensure that it is before all the overlays hence putting it at the front.
 */
class LayerStack
{
	using LayerStackVector = std::vector<Layer*>;
public:
	/**
	 * @brief Default LayerStack constructor.
	 */
	LayerStack();

	/**
	 * @brief Default LayerStack destructor.
	 */
	~LayerStack();

	/**
	 * @brief Pushes the specified @p layer onto this LayerStack.
	 * @note Layers get pushed onto the first half of the stack.
	 */
	void PushLayer(Layer* layer);

	/**
	 * @brief Pushes the specified overlay @p layer onto this LayerStack.
	 * @note Overlay layers get pushed onto the second half of the stack.
	 */
	void PushOverlay(Layer* layer);

	/**
	 * @brief Pops the specified @p layer from this LayerStack.
	 */
	void PopLayer(Layer* layer);

	/**
	 * @brief Pops the specified overlay @p layer from this LayerStack.
	 */
	void PopOverlay(Layer* layer);

	/**
	 * @brief Gets an iterator pointing to the beginning of this LayerStack.
	 */
	LayerStackVector::iterator Begin()
	{
		return m_Layers.begin();
	}

	/**
	 * @brief Gets an iterator pointing to the end of this LayerStack.
	 */
	LayerStackVector::iterator End()
	{
		return m_Layers.end();
	}
private:
	/**
	 * @brief A std::vector of Layers.
	 */
	LayerStackVector m_Layers;

	/**
	 * @brief An iterator pointing to the top of the LayerStack.
	 */
	LayerStackVector::iterator m_LayersTop;
};