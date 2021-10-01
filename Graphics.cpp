#include "Graphics.h"
#include "Utils.h"
#include <math.h>
#include "mafs.h"

constexpr float DEPTH_TEST_END = 0;

inline void Clamp(int* val, int min, int max)
{
	if (*val < min)*val = min;
	if (*val > max)*val = max;
}

inline void Clamp(float& val, int min, int max)
{
	if (val < min)val = min;
	if (val > max)val = max;
}

inline void Clamp(float* val, int min, int max)
{
	if ((int)std::floor(*val) < min)*val = min;
	if ((int)std::floor(*val) > max)*val = max;
}

void Graphics::ClearDepth()
{
	//Print(DepthBuffer[200 * view.width + 300]);
	const float* end = &DepthBuffer[view.size];
	for (float* pix = &DepthBuffer[0]; pix < end; pix++)
	{
		*pix = DEPTH_TEST_END;
	}
	//memset(&DepthBuffer[0], 0xfe, view.size_b);
}

Graphics::Graphics(unsigned int width, unsigned int height)
	:view(width, height), DepthBuffer(nullptr)
{
	DepthBuffer = (float*)malloc(view.size_b);
	ClearDepth();
	bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = (-1) * (height);
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
}

void Graphics::Fill(unsigned int c)
{
	unsigned int* pix = view.mem;
	const size_t size = view.size;
	for (uint* end = &pix[size]; pix <= end; pix++)
		*pix = c;
}

void Graphics::Clear()
{
	memset(view.mem, 0, view.size_b);
}

inline int Graphics::PutPixel(int x, int y, uint c)
{
	if (x < view.width && x >= 0 && y < view.height && y >= 0)
	{
		view.mem[view.width * y + x] = c;
		return 1;
	}
	else
	{
		return 0;
	}
}

inline bool Graphics::DepthTest(const float z, const int& x, const int& y)
{
		int pos = y * view.width + x;
		if (z <= DepthBuffer[pos])
		{
			DepthBuffer[pos] = z;
			return true;
		}
		return false;
}

void Graphics::DrawFlatTop(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c)
{
	const float dy = std::ceil(v2.y - v0.y);
	const float dx12 = v2.x - v1.x, dx02 = v2.x - v0.x;
	float zstep = 0.0f;
	if (v0.x >= v1.x)
	{
		const float xstep0 = dx12 / dy, xstep1 = dx02 / dy,
					zstep0 = (v2.w - v0.w) / dy;
		if (std::floor(v0.x) != std::floor(v1.x))
		{
			zstep = (v0.w - v1.w) / (v0.x - v1.x);
		}
		float x0 = std::floor(v1.x), x1 = std::floor(v0.x),
			  z0 = std::floor(v1.w);
		int x, y; float z = z0;
		for (y = v0.y; y < v2.y; y++)
		{
			z = z0;
			for (x = x0 - 1; x < x1 + 1; x++)
			{
				if (DepthTest(z, x, y))
				{
					PutPixel(x, y, int(std::abs(z) * 10) << 16);
				}
				z += zstep;
			}
			x0 += xstep0;
			x1 += xstep1;
			z0 += zstep0;
		}
	}
	else
	{
		const float xstep0 = dx02 / dy, xstep1 = dx12 / dy,
					zstep0 = (v2.w - v1.w) / dy;
		float x0 = std::floor(v0.x), x1 = std::floor(v1.x),
		      z0 = std::floor(v0.w);
		if (std::floor(v0.x) != std::floor(v1.x))
		{
			zstep = (v1.w - v0.w) / (v1.x - v0.x);
		}
		int x, y; float z = z0;
		for (y = v0.y; y < v2.y; y++)
		{
			z = z0;
			for (x = x0 - 1; x < x1 + 1; x++)
			{
				if (DepthTest(z, x, y))
				{
					PutPixel(x, y, int(std::abs(z) * 10) << 16);
				}
				z += zstep;
			}
			x0 += xstep0;
			x1 += xstep1;
			z0 += zstep0;
		}
	}
}

void Graphics::DrawFlatBot(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c)
{
	const float dy = std::ceil(v2.y - v0.y), dx01 = v1.x - v0.x, dx02 = v2.x - v0.x;
	float zstep = 0.0f;
	if (v1.x >= v2.x)
	{
		const float xstep0 = dx02 / dy, xstep1 = dx01 / dy, zstep0 = (v2.w - v0.w) / dy;
		float x0 = std::floor(v0.x), x1 = x0, z0 = std::floor(v0.w);
		if (std::floor(v1.x) != std::floor(v2.x))
		{
			zstep = (v1.w - v2.w) / (v1.x - v2.x);
		}
		int x, y; float z = z0;
		for (y = v0.y; y < v2.y; y++)
		{
			z = z0;
			for (x = x0 - 1; x <= x1 + 1; x++)
			{
				if (DepthTest(z, x, y))
				{
					PutPixel(x, y, int(std::abs(z) * 10) << 16);
				}
				z += zstep;
			}
			x0 += xstep0;
			x1 += xstep1;
			z0 += zstep0;
		}
	}
	else
	{
		const float xstep0 = dx01 / dy, xstep1 = dx02 / dy, zstep0 = (v1.w - v0.w) / dy;
		float x0 = std::floor(v0.x), x1 = x0, z0 = std::floor(v0.w);
		if (std::floor(v1.x) != std::floor(v2.x))
		{
			zstep = (v2.w - v1.w) / (v2.x - v1.x);
		}
		int x, y; float z = z0;
		for (y = v0.y; y < v2.y; y++)
		{
			z = z0;
			for (x = x0 - 1; x <= x1 + 1; x++)
			{
				if (DepthTest(z, x, y))
				{
					PutPixel(x, y, int(std::abs(z) * 10) << 16);
				}
				z += zstep;
			}
			x0 += xstep0;
			x1 += xstep1;
			z0 += zstep0;
		}
	}
}

void Graphics::DrawFlatBotInterp(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c)
{
	float xstart, xend, zstart, zend, t, z;

	for (int y = v0.y; y < v2.y; y++)
	{
		t = std::ceil(y - v0.y) / std::ceil(v1.y - v0.y);
		xstart = v0.x + t * (v1.x - v0.x);
		xend = v0.x + t * (v2.x - v0.x);
		zstart = v0.w + t * (v1.w - v0.w);
		zend = v0.w + t * (v2.w - v0.w);
		if (xstart > xend)std::swap(xstart, xend);
		for (float x = xstart; x < xend; x++)
		{
			t = std::ceil(x - xstart) / std::ceil(xend - xstart);
			z = zstart + t * (zend - zstart);
			if (DepthTest(z, x, y))
			{
				PutPixel(x, y, c/*int( (std::abs(z)) * 80) << 16*/);
			}
		}
	}
}

void Graphics::DrawFlatTopInterp(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c)
{
	
	float xstart, xend, zstart, zend, t, z;

	for (int y = v0.y; y < v2.y; y++)
	{
		t = std::ceil(y - v0.y) / std::ceil(v2.y - v0.y);
		xstart = v0.x + t * (v2.x - v0.x);
		xend = v1.x + t * (v2.x - v1.x);
		zstart = v0.w + t * (v2.w - v0.w);
		zend = v0.w + t * (v2.w - v1.w);
		if (xstart > xend)std::swap(xstart, xend);
		for (int x = xstart; x < xend; x++)
		{
			t = std::ceil(x - xstart) / std::ceil(xend - xstart);
			z = zstart + t * (zend - zstart);
			if (DepthTest(z, x, y))
			{
				PutPixel(x, y, c/*int( (std::abs(z)) * 80) << 16*/);
			}
		}
	}
}

void Graphics::DrawFlatTopRetarded(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c)
{
	const float dy = (v2.y - v0.y);
	if (v0.x > v1.x)
	{
		const float xstep0 = (v2.x - v1.x) / dy;
		const float xstep1 = (v2.x - v0.x) / dy;
		float x0 = v1.x, x1 = v0.x;
		int x, y;
		for (y = v0.y; y < v2.y; y++)
		{
			x0 += xstep0;
			x1 += xstep1;
			for (x = v1.x; x < v0.x; x++)
			{
				PutPixel(x, y, c);
			}
		}
	}
	else
	{
		const float xstep0 = (v2.x - v0.x) / dy;
		const float xstep1 = (v2.x - v1.x) / dy;
		float x0 = v0.x, x1 = v1.x;
		int x, y;
		for (y = v0.y; y < v2.y; y++)
		{
			x0 += xstep0;
			x1 += xstep1;
			for (x = v0.x; x < v1.x; x++)
				PutPixel(x, y, c);
		}
	}
}

void Graphics::DrawFlatBotRetarded(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c)
{
	const float dy = (v2.y - v0.y);
	if (v2.x > v1.x)
	{
		const float xstep0 = (v1.x - v0.x) / dy;
		const float xstep1 = (v2.x - v0.x) / dy;
		float x0 = v0.x, x1 = v1.x;
		int x, y;
		for (y = v0.y; y < v2.y; y++)
		{
			x0 += xstep0;
			x1 += xstep1;
			for (x = v1.x; x < v0.x; x++)
			{
				PutPixel(x, y, c);
			}
		}
	}
	else
	{
		const float xstep0 = (v2.x - v0.x) / dy;
		const float xstep1 = (v1.x - v0.x) / dy;
		float x0 = v0.x, x1 = v1.x;
		int x, y;
		for (y = v0.y; y < v2.y; y++)
		{
			x0 += xstep0;
			x1 += xstep1;
			for (x = v0.x; x < v1.x; x++)
				PutPixel(x, y, c);
		}
	}
}

void Graphics::DrawLine(int x0, int y0, int x1, int y1, uint c)
{
	const uint w = view.width;
	const uint h = view.height;

	Clamp(&x0, 0, w);
	Clamp(&x1, 0, w);
	Clamp(&y0, 0, h);
	Clamp(&y1, 0, h);

	bool steep = false;
	if (abs((double)x0 - x1) < abs((double)y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derr = 2 * abs(dy);
	int err = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep) {
			PutPixel(y, x, c);
		}
		else {
			PutPixel(x, y, c);
		}
		err += derr;
		if (err > dx) {
			y += (y1 > y0 ? 1 : -1);
			err -= 2 * dx;
		}
	}
}

void Graphics::DrawTri(int x0, int y0, int x1, int y1, int x2, int y2, uint c)
{
	const uint w = view.width;
	const uint h = view.height;

	Clamp(&x0, 0, w);
	Clamp(&x1, 0, w);
	Clamp(&x2, 0, w);
	Clamp(&y0, 0, h);
	Clamp(&y1, 0, h);
	Clamp(&y2, 0, h);

	DrawLine(x0, y0, x1, y1, c);
	DrawLine(x1, y1, x2, y2, c);
	DrawLine(x2, y2, x0, y0, c);
}

void Graphics::FillTri(int x0, int y0, int x1, int y1, int x2, int y2, uint c, bool wireframe)
{
	const uint w = view.width;
	const uint h = view.height;

	Clamp(&x0, 0, w);
	Clamp(&x1, 0, w);
	Clamp(&x2, 0, w);
	Clamp(&y0, 0, h);
	Clamp(&y1, 0, h);
	Clamp(&y2, 0, h);


	if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); };
	if (y0 > y2) { std::swap(y0, y2); std::swap(x0, x2); };
	if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); };

	if (y0 == y1 && y1 == y2)
	{
		if (x0 > x1)std::swap(x0, x1);
		if (x0 > x2)std::swap(x0, x2);
		if (x1 > x2)std::swap(x1, x2);
		DrawLine(x0, y0, x2, y0, c);
	}
	else if (y0 == y1)
	{
		int y;
		float xstep2, xstep3, xstart = x0, xend = x1;
		const float dx2 = (x2 - x1);
		const float dx3 = (x2 - x0);
		const float dy2 = (y2 - y1);
		const float dy3 = (y2 - y0);
		xstep2 = dx2 / dy2;
		xstep3 = dx3 / dy3;
		if (xstart > xend)std::swap(xstart, xend);
		if (xstep2 < xstep3)std::swap(xstep2, xstep3);
		for (y = y1; y < y2; y++)
		{
			xstart += xstep2;
			xend += xstep3;
			for (int x = xstart; x < xend; x++)
			{
				PutPixel(x, y, c);
			}
		}
	}
	else if (y1 == y2)
	{
		int y;
		float xstep1, xstep3, xstart = x0, xend = x0;
		const float dx1 = (x1 - x0);
		const float dx3 = (x2 - x0);
		const float dy1 = (y1 - y0);
		const float dy3 = (y2 - y0);
		xstep1 = dx1 / dy1;
		xstep3 = dx3 / dy3;
		if (xstep1 > xstep3)std::swap(xstep1, xstep3);
		for (y = y0; y < y1; y++)
		{
			xstart += xstep1;
			xend += xstep3;
			for (int x = xstart; x < xend; x++)
			{
				PutPixel(x, y, c);
			}
		}
	}
	else
	{
		int y;
		float xstep1, xstep2, xstep3, xstart = x0, xend = x0;
		bool leftMajor = false;
		const float dx1 = (x1 - x0);
		const float dx2 = (x2 - x1);
		const float dx3 = (x2 - x0);

		const float dy1 = (y1 - y0);
		const float dy2 = (y2 - y1);
		const float dy3 = (y2 - y0);


		xstep1 = dx1 / dy1;
		xstep2 = dx2 / dy2;
		xstep3 = dx3 / dy3;

		if (xstep1 > xstep3) {
			std::swap(xstep1, xstep3);
			leftMajor = true;
		}
		for (y = y0; y < y1; ++y)
		{
			xstart += xstep1;
			xend += xstep3;
			for (int x = xstart; x < xend; x++)
			{
				PutPixel(x, y, c);
			}
		}
		if (leftMajor) {
			std::swap(xstep3, xstep1);
			std::swap(xstep3, xstep2);
		}
		for (y = y1; y < y2; ++y)
		{
			xstart += xstep2;
			xend += xstep3;
			for (int x = xstart; x < xend; x++)
			{
				PutPixel(x, y, c);
			}
		}
	}
		if(wireframe)DrawTri(x0, y0, x1, y1, x2, y2, 0xffffff);
	
}

void Graphics::FillTriRetarded(vec4f v0, vec4f v1, vec4f v2, uint c)
{
	const uint w = view.width;
	const uint h = view.height;


	Clamp(&v0.x, 0, w);
	Clamp(&v1.x, 0, w);
	Clamp(&v2.x, 0, w);
	Clamp(&v0.y, 0, h);
	Clamp(&v1.y, 0, h);
	Clamp(&v2.y, 0, h);



	if (v0.y > v1.y) { std::swap(v0, v1); };
	if (v0.y > v2.y) { std::swap(v0, v2); };
	if (v1.y > v2.y) { std::swap(v1, v2); };

	if (std::floor(v0.y) == std::floor(v1.y) && std::floor(v1.y) == std::floor(v2.y))
	{
		if (v0.x > v1.x)std::swap(v0, v1);
		if (v0.x > v2.x)std::swap(v0, v2);
		if (v1.x > v2.x)std::swap(v1, v2);
		DrawLine(std::floor(v0.x), std::floor(v0.y),
			std::floor(v2.x), std::floor(v0.y), c);
	}
	if (std::floor(v0.y) == std::floor(v1.y)) // Flat top
	{
		DrawFlatTopRetarded(v0, v1, v2, c);
	}
	else if (std::floor(v1.y) == std::floor(v2.y)) //Flat bot
	{
		DrawFlatBotRetarded(v0, v1, v2, c);
	}
	else
	{
		const vec4f p = v0 + ((v1.y - v0.y) / (v2.y - v0.y)) * (v2 - v0);
		if (p.x < v1.x) //Right major
		{
			DrawFlatBotRetarded(v0, v1, p, c);
			DrawFlatTopRetarded(v1, p, v2, c);
		}
		else//Left major
		{
			DrawFlatBotRetarded(v0, p, v1, c);
			DrawFlatTopRetarded(p, v1, v2, c);
		}
	}
}

void Graphics::FillTri(vec4f v0, vec4f v1, vec4f v2, uint c, bool wireframe)
{
	const uint w = view.width;
	const uint h = view.height;


	Clamp(&v0.x, 0, w);
	Clamp(&v1.x, 0, w);
	Clamp(&v2.x, 0, w);
	Clamp(&v0.y, 0, h - 1);
	Clamp(&v1.y, 0, h - 1);
	Clamp(&v2.y, 0, h - 1);



	if (v0.y > v1.y) { std::swap(v0, v1); };
	if (v0.y > v2.y) { std::swap(v0, v2); };
	if (v1.y > v2.y) { std::swap(v1, v2); };

	if (std::floor(v0.y) == std::floor(v1.y) && std::floor(v1.y) == std::floor(v2.y))
	{
		if (v0.x > v1.x)std::swap(v0, v1);
		if (v0.x > v2.x)std::swap(v0, v2);
		if (v1.x > v2.x)std::swap(v1, v2);
		DrawLine(std::floor(v0.x), std::floor(v0.y),
				 std::floor(v2.x), std::floor(v0.y), c);
	}
	if (std::floor(v0.y) == std::floor(v1.y)) // Flat top
	{
		if (v0.x >= v1.x) 
		{
			DrawFlatTop(v1, v0, v2, c);
		}
		else
		{

			DrawFlatTop(v0, v1, v2, c);
		}
	}
	else if (std::floor(v1.y) == std::floor(v2.y)) //Flat bot
	{
		if (v1.x >= v2.x)
		{
			DrawFlatBot(v0, v2, v1, c);
		}
		else
		{
			DrawFlatBot(v0, v1, v2, c);
		}
	}
	else
	{
		const vec4f p = v0 + ((v1.y - v0.y) / (v2.y - v0.y)) * (v2 - v0);
		if (p.x < v1.x) //Right major
		{
			DrawFlatBot(v0, v1, p, c);
			DrawFlatTop(v1, p, v2, c);
		}
		else//Left major
		{
			DrawFlatBot(v0, p, v1, c);
			DrawFlatTop(p, v1, v2, c);
		}
	}
	if (wireframe)DrawTri(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, 0xffffff);
}

void Graphics::FillTriInterp(vec4f v0, vec4f v1, vec4f v2, uint c, bool wireframe)
{
	const uint w = view.width;
	const uint h = view.height;


	Clamp(&v0.x, 0, w);
	Clamp(&v1.x, 0, w);
	Clamp(&v2.x, 0, w);
	Clamp(&v0.y, 0, h);
	Clamp(&v1.y, 0, h);
	Clamp(&v2.y, 0, h);



	if (v0.y > v1.y) { std::swap(v0, v1); };
	if (v0.y > v2.y) { std::swap(v0, v2); };
	if (v1.y > v2.y) { std::swap(v1, v2); };

	if (std::floor(v0.y) == std::floor(v1.y)) // Flat top
	{
		if (v0.x >= v1.x)
		{
			DrawFlatTopInterp(v1, v0, v2, c);
		}
		else
		{
			DrawFlatTopInterp(v0, v1, v2, c);
		}
	}
	else if (std::floor(v1.y) == std::floor(v2.y)) //Flat bot
	{
		if (v1.x >= v2.x)
		{
			DrawFlatBotInterp(v0, v2, v1, c);
		}
		else
		{
			DrawFlatBotInterp(v0, v1, v2, c);
		}
	}
	else
	{
		const vec4f p = v0 + ((v1.y - v0.y) / (v2.y - v0.y)) * (v2 - v0);
		if (p.x < v1.x) //Right major
		{
			DrawFlatBotInterp(v0, v1, p, c);
			DrawFlatTopInterp(v1, p, v2, c);
		}
		else//Left major
		{
			DrawFlatBotInterp(v0, p, v1, c);
			DrawFlatTopInterp(p, v1, v2, c);
		}
	}
	if (wireframe)DrawTri(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, 0xffffff);
}


