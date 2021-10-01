#pragma once
#include "RetardWindows.h"
#include "Buffer.h"
#include "mafs.h"
#include "Colors.h"


class Graphics
{
public:
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;
	Graphics(unsigned int width, unsigned int height);
	void ResizeVp(unsigned int nwidth, unsigned int nheight)
	{ 
		view.Resize(nwidth, nheight);
		free(DepthBuffer);
		DepthBuffer = (float*)malloc(view.size_b);
		bmpInfo.bmiHeader.biWidth = nwidth;
		bmpInfo.bmiHeader.biHeight = (-1) * nheight;
	};
	void Fill(unsigned int c);
	void Clear();
	int PutPixel(int x, int y, uint c);
	bool DepthTest(const float z, const int& x, const int& y);
	void DrawTri(int x0, int y0, int x1, int y1, int x2, int y2, uint c);
	void DrawLine(int x0, int y0, int x1, int y1, uint c);
	void FillTri(int x0, int y0, int x1, int y1, int x2, int y2, uint c, bool wireframe = false);
	void FillTri(vec4f v0, vec4f v1, vec4f v2, uint c, bool wireframe = false);
	void FillTriInterp(vec4f v0, vec4f v1, vec4f v2, uint c, bool wireframe = false);
	void FillTriRetarded(vec4f v0, vec4f v1, vec4f v2, uint c);
	void ClearDepth();
	~Graphics() { free(DepthBuffer); };
public:
	Buffer view;
	float* DepthBuffer;
	BITMAPINFO bmpInfo;
private:
	void DrawFlatTop(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c);
	void DrawFlatBot(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c);
	void DrawFlatTopInterp(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c);
	void DrawFlatBotInterp(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c);
	void DrawFlatTopRetarded(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c);
	void DrawFlatBotRetarded(const vec4f& v0, const vec4f& v1, const vec4f& v2, uint c);

};