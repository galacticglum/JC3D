/**
 * Author: Shon Verch
 * File Name: FlagEnum.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/08/2019
 * Modified Date: 06/08/2019
 * Description: A type-safe bitmask enum implementation.
 */

#pragma once
#include <type_traits>

template <typename Enum>
struct EnableFlagEnumOperators
{
	static const bool Enable = false;
};

#define ENUM_FLAG_TEMPLATE template<typename Enum> typename std::enable_if<EnableFlagEnumOperators<Enum>::Enable, Enum>::type

/**
 * @brief Bitwise OR implementation for two flag enums.
 */
ENUM_FLAG_TEMPLATE
operator |(Enum left, Enum right)
{
	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum> (static_cast<underlying>(left) | static_cast<underlying>(right));
}

/**
 * @brief Bitwise OR implementation for two flag enums.
 */
ENUM_FLAG_TEMPLATE
operator |=(Enum& left, Enum right)
{
	left = left | right;
	return left;
}

/**
 * @brief Bitwise AND implementation for two flag enums.
 */
ENUM_FLAG_TEMPLATE
operator &(Enum left, Enum right)
{
	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum> (static_cast<underlying>(left) & static_cast<underlying>(right));
}

/**
 * @brief Bitwise AND implementation for two flag enums.
 */
ENUM_FLAG_TEMPLATE
operator &=(Enum& left, Enum right)
{
	left = left & right; return left;
}

/**
 * @brief Bitwise COMPLEMENT implementation for two flag enums.
 */
ENUM_FLAG_TEMPLATE
operator ~(const Enum value)
{
	using underlying = typename std::underlying_type<Enum>::type;
	return static_cast<Enum> (~static_cast<underlying>(value));
}

/**
 * @brief Converts the enum type @p T to a bitmask flag enum.
 * @param T The type of the enum.
 */
#define ENABLE_FLAG_ENUM(T) template<> struct EnableFlagEnumOperators<T> { static const bool Enable = true; };