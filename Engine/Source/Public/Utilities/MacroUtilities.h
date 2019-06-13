/**
 * Author: Shon Verch
 * File Name: MacroUtilities.h
 * Project Name: JesusChristIn3D
 * Creation Date: 06/13/2019
 * Modified Date: 06/13/2019
 * Description: Common macro tricks and functionality.
 */

#pragma once

#define FIRST_ARG_(N, ...) N
#define FIRST_ARG(args) FIRST_ARG_ args
#define XSTR(S) STR(S)
#define STR(S) #S
