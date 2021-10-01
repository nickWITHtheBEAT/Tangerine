#include "Window.h"

_Window::_Window(const char* WindowTitle, const char* ClassName, uint width, uint height)
	: hwnd(nullptr), name(ClassName), title(WindowTitle),
	dwStyle(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MINIMIZEBOX ^ WS_MINIMIZE ^ WS_MAXIMIZE ^ WS_MAXIMIZEBOX),
	Minimized(false)
{
	Create(width, height);
	pGfx = std::make_unique<Graphics>(width, height);
};

_Window::~_Window()
{
	UnregisterClassA(GetName(), GetModuleHandle(nullptr));
}

int _Window::RenderVp()
{
	BITMAPINFO bmpInfo;
	int x, y, w, h;
	GetPos(&x, &y);
	GetSize(&w, &h);
	int res = StretchDIBits(GetDC(GetHandle()), 0, 0, w, h, 0, 0, w, h, pGfx->view.mem,
		&pGfx->bmpInfo, DIB_RGB_COLORS, SRCCOPY);
	pGfx->ClearDepth();
	return res;
};

const char* _Window::GetName()
{
	return name;
};

const char* _Window::GetTitle()
{
	return title;
}

void _Window::GetSize(int* w, int* h)
{
	RECT r;
	GetClientRect(GetHandle(), &r);
	*w = r.right;
	*h = r.bottom;
}

HWND _Window::GetHandle()
{
	return hwnd;
}

DWORD _Window::GetStyle()
{
	return dwStyle;
}

Graphics* _Window::GetGfx()
{
	return pGfx.get();
}

void _Window::GetPos(int* x, int* y)
{
	RECT r;
	GetWindowRect(GetHandle(), &r);
	*x = r.left;
	*y = r.top;
}

void _Window::SetSize(int w, int h)
{
	RECT rcClient, rcWind;
	POINT ptDiff;
	GetClientRect(GetHandle(), &rcClient);
	GetWindowRect(GetHandle(), &rcWind);
	ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
	ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;
	MoveWindow(GetHandle(), rcWind.left, rcWind.top, w + ptDiff.x, h + ptDiff.y, FALSE);
	GetGfx()->ResizeVp(w, h);
}

void _Window::SetPos(int xpos, int ypos)
{
	int w, h;
	GetSize(&w, &h);
	MoveWindow(GetHandle(), xpos, ypos, w, h, FALSE);
}

void _Window::SetTitle(const char* text)
{
	SetWindowTextA(GetHandle(), text);
	title = text;
}

void _Window::GetCursor(uint* x, uint* y)
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(GetHandle(), &p);
	*x = p.x;
	*y = p.y;
}

void _Window::GetCursorAbs(uint* x, uint* y)
{
	POINT p;
	GetCursorPos(&p);
	*x = p.x;
	*y = p.y;

}

bool _Window::Create(int width, int height)
{
	WNDCLASSA w;
	ZeroMemory(&w, sizeof(WNDCLASSA));
	//USELESS GARBAGE
	w.cbClsExtra = 0;
	w.cbWndExtra = sizeof(_Window*);
	w.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	w.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	w.lpszMenuName = nullptr;
	w.lpszClassName = GetName();
	//KINDA USEFUL GARBAGE
	w.hInstance = GetModuleHandle(nullptr);
	w.lpfnWndProc = WndProcSetup;
	w.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	w.hCursor = LoadCursor(nullptr, IDC_ARROW);

	if (!RegisterClassA(&w))
	{
		PopUp("Failed to register Window class");
		return false;
	}

	//WINDOW PROPER SIZE SETUP
	RECT r = { 0, 0, width, height };
	AdjustWindowRect(&r, GetStyle(), FALSE);
	int _width = r.right - r.left;
	int _height = r.bottom - r.top;
	int _x = GetSystemMetrics((SM_CXSCREEN) / 2 - width / 2);
	int _y = GetSystemMetrics((SM_CYSCREEN) / 2 - height / 2);

	hwnd = CreateWindowA
	(	GetName(), GetTitle(), GetStyle(), _x, _y, _width, _height, nullptr, nullptr,
		GetModuleHandle(nullptr), this	);

	if (hwnd == nullptr)
	{
		PopUp("Failed to create the russian spy window,"
			"blyat. You are very bad at programming you useless western spy!!!");
		return false;
	}
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	//pGfx = std::make_unique<Graphics>(width, height);

	return true;
}

LRESULT CALLBACK _Window::WndProcSetup(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	if (msg == WM_CREATE)
	{
		_Window* p = nullptr;
		const CREATESTRUCTA* const pC = (CREATESTRUCTA*)lp;
		p = (_Window*)pC->lpCreateParams;
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)p);
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WndProcFwd);
		return p->MsgProc(msg, wp, lp);
	}
	else
	{
		return DefWindowProcA(hwnd, msg, wp, lp);
	}
}

LRESULT _Window::WndProcFwd(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	_Window* p = (_Window*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
	return p->MsgProc(msg, wp, lp);
}
