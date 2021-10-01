#pragma once

#include "RetardWindows.h"
#include <iostream>

typedef unsigned int uint;

inline void PopUp(const char* text, const char* title = "Error!")
{
	MessageBoxExA(0, text, title, MB_OK, 0);
}

inline void Print(const char* text)
{
	std::cout << text << '\n';
}
template<typename T> inline void Print(const T text)
{
	std::cout << text << '\n';
}