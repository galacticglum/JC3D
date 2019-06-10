/**
 * Author: Shon Verch
 * File Name: Layer.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: An application layer.
 */

#pragma once

#include <Events/Event.h>

/**
 * @class Layer Layer.h
 * @brief An application layer.
 */
class Layer
{
public:
	/**
	 * @brief Initialize a new Layer.
	 * @param name The name of the Layer. Defaults to "Layer".
	 */
	explicit Layer(const std::string& name = "Layer");

	/**
	 * @brief Layer destructor.
	 */
	virtual ~Layer();

	/**
	 * @brief Called when this Layer is attached to the Application.
	 */
	virtual void OnAttach() {}

	/**
	 * @brief Called when this Layer is detached from the Application.
	 */
	virtual void OnDetach() {}

	/**
	 * @brief Called every frame to update this Layer.
	 */
	virtual void OnUpdate() {}

	/**
	 * @brief Called whenever this Layer receives an Event.
	 */
	virtual void OnEvent(Event& event) {}

	/**
	 * @brief Retrieves the name of this Layer.
	 */
	std::string GetName() const { return m_Name; }
private:
	/**
	 * @brief The name of this Layer.
	 * @note This is really just used for debugging and logging purposes.
	 */
	std::string m_Name;
};
