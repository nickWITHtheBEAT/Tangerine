#pragma once
#include "Utils.h"
#include <thread>
#include <string>
#include <strstream>
#include "Graphics.h"



class _Window
{
public:
	_Window(const _Window&) = delete;
	_Window operator= (const _Window&) = delete;
	_Window(const char* title, const char* name, uint width, uint height);
	~_Window();
	int RenderVp();
	const char* GetName();
	const char* GetTitle();
	void GetSize(int* w, int* h);
	HWND GetHandle();
	DWORD GetStyle();
	Graphics* GetGfx();
	void GetPos(int* x, int* y);
	void SetSize(int w, int h);
	void SetPos(int x, int y);
	void SetTitle(const char* text);
	void GetCursor(uint* x, uint* y);
	void GetCursorAbs(uint* x, uint* y);

private:
	HWND hwnd;
	const char* name, * title;
	DWORD dwStyle;
	std::unique_ptr<Graphics> pGfx;
	bool Minimized;
private:
	bool Create(int width, int height);
	static LRESULT CALLBACK WndProcSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT CALLBACK WndProcFwd(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	virtual LRESULT MsgProc(UINT msg, WPARAM wp, LPARAM lp) {
		return DefWindowProc(GetHandle(), msg, wp, lp);
	};
};