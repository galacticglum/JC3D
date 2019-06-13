/**
 * Author: Shon Verch
 * File Name: STLUtilities.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/12/2019
 * Modified Date: 06/12/2019
 * Description: STL utilities.
 */

#pragma once

#include <utility>

 /**
  * @brief A hash function used to hash a pair of any kind
  */
struct HashPair {
	template <class T1, class T2>
	size_t operator()(const std::pair<T1, T2>& p) const
	{
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);
		return hash1 ^ hash2;
	}
};
