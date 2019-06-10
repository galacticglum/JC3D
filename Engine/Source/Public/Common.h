/**
 * Author: Shon Verch
 * File Name: Common.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/09/2019
 * Modified Date: 06/09/2019
 * Description: Common functionality header.
 */

#pragma once

#define BIT(x) 1 << x
#define BIND_EVENT(fn) std::bind(&fn, this, std::placeholders::_1)