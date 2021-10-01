#pragma once
#include <memory>
#include <iostream>

//if I ever do SSE...
#define DEFAULT_ALIGNMENT 16
typedef unsigned int uint;

class Buffer
{
public:
	Buffer(uint width, uint height, uint allignment = DEFAULT_ALIGNMENT)
		:width(width), height(height), mem(nullptr), size(width * height), size_b(size * sizeof(uint))
	{
		mem = (uint*)malloc(size_b);
	};
	~Buffer()
	{
		free(mem);
	}
	void Resize(uint x, uint y)
	{
		if (x != 0 && y != 0)
		{
			width = x;
			height = y;
			size = x * y;
			size_b = size * sizeof(uint);
			free(mem);
			mem = (uint*)malloc(size_b);
		}
	}

	uint* mem;
	uint size;
	uint size_b;
	uint width, height;
};