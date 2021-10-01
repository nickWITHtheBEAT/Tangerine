#pragma once
#include "mafs.h"

class Color
{
public:
	unsigned int uint;
public:
	constexpr Color() : uint(0xcccccc) {}
	constexpr Color(const Color& col)
		:
		uint(col.uint)
	{}
	constexpr Color(unsigned int col)
		:
		uint(col)
	{}
	constexpr Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
		:
		uint((a << 24u) | (r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(unsigned char r, unsigned char g, unsigned char b)
		:
		uint((r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(Color col, unsigned char x)
		:
		Color((x << 24u) | col.uint)
	{}
	Color& operator =(Color color)
	{
		uint = color.uint;
		return *this;
	}
	constexpr unsigned char GetX() const
	{
		return uint >> 24u;
	}
	constexpr unsigned char GetA() const
	{
		return GetX();
	}
	constexpr unsigned char GetR() const
	{
		return (uint >> 16u) & 0xFFu;
	}
	constexpr unsigned char GetG() const
	{
		return (uint >> 8u) & 0xFFu;
	}
	constexpr unsigned char GetB() const
	{
		return uint & 0xFFu;
	}
	void SetX(unsigned char x)
	{
		uint = (uint & 0xFFFFFFu) | (x << 24u);
	}
	void SetA(unsigned char a)
	{
		SetX(a);
	}
	void SetR(unsigned char r)
	{
		uint = (uint & 0xFF00FFFFu) | (r << 16u);
	}
	void SetG(unsigned char g)
	{
		uint = (uint & 0xFFFF00FFu) | (g << 8u);
	}
	void SetB(unsigned char b)
	{
		uint = (uint & 0xFFFFFF00u) | b;
	}
};

namespace Colors
{
	static constexpr Color MakeRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return { unsigned int((r << 16) | (g << 8) | b) };
	}
	static constexpr Color White = MakeRGB(255u, 255u, 255u);
	static constexpr Color Black = MakeRGB(0u, 0u, 0u);
	static constexpr Color Gray = MakeRGB(0x80u, 0x80u, 0x80u);
	static constexpr Color LightGray = MakeRGB(0xD3u, 0xD3u, 0xD3u);
	static constexpr Color Red = MakeRGB(255u, 0u, 0u);
	static constexpr Color Green = MakeRGB(0u, 255u, 0u);
	static constexpr Color Blue = MakeRGB(0u, 0u, 255u);
	static constexpr Color Yellow = MakeRGB(255u, 255u, 0u);
	static constexpr Color Cyan = MakeRGB(0u, 255u, 255u);
	static constexpr Color Magenta = MakeRGB(255u, 0u, 255u);
}