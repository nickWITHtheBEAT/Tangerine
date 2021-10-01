#include "Window.h"
#include <iostream>
#include "Pipeline.h"
#include "Timer.h"

class Window : public _Window
{
public:
    Window(const char* t, const char* n, uint w = 640, uint h = 480) : _Window(t, n, w, h) { };
private:
    LRESULT MsgProc(UINT msg, WPARAM wp, LPARAM lp)
    {
        switch (msg)
        {
        case WM_SIZE:
            break;
        case WM_MOUSEWHEEL:
            Print(GET_WHEEL_DELTA_WPARAM(wp));
            break;
        case WM_KEYUP:
            break;
        case WM_KEYDOWN:
            uint x, y;
            GetCursor(&x, &y);
            //Print(this->GetGfx()->view.mem[y * this->GetGfx()->view.width + x]);
            break;
        case WM_CLOSE:
        {
            DestroyWindow(GetHandle());
            PostQuitMessage(69);
        }
            break;
        default:
            return DefWindowProc(GetHandle(), msg, wp, lp);
        }
    }
};

int main()
{
    Model m0("teapot.obj");
    Model m1("monke.obj");
    Model m2("cube.obj");
    Window w("Monkey Nipplez", "w1");
    Model m[4] = { m0, m1, m2, m2 };
    for(Model& mi : m)
        mi.pos = { 0.0f, 0.0f, -4.0f };
    m[2].rot.x = PI / 4;
    m[3].rot.x = -PI / 3;
    MSG msg = { 0 };

    w.SetSize(1200, 800);

    Pipeline p(&m[2], 1, w.GetGfx());

    vec4f pts1[3] = { { 200, 200, 0, 5.0f },
                      { 200, 400, 0, 16.0f },
                      { 800, 400, 0, -8.0f } };
    vec4f pts2[3] = { { 200, 600, 0, 8.0f },
                      { 700, 300, 0, 8.0f },
                      { 200, 400, 0, 8.0f } };

    Timer::TimerStruct ts = { 0 };
    Timer::Setup(&ts);
    float a = 0.0f;
    uint x, y;
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Timer::GetDt(&ts);
            w.GetGfx()->Clear();
            a += 0.2f * ts.dt;
            p.Present(a);
            w.GetCursor(&x, &y);
            pts2[1].x = x;
            pts2[1].y = y;
            //m[2].pos.x = ((float(x) / 1920.0f) - 0.5f) * 2 * 4;
            //m[2].pos.y = ((float(y) / 1080.0f) - 0.5f) * 2 * 4;
            pts1[0].w += a;

            //w.GetGfx()->FillTri(pts1[0], pts1[1], pts1[2], 0xff0000, false);
            //w.GetGfx()->FillTriInterp(pts2[0], pts2[1], pts2[2], 0xffff00, false);

            //Present Call
            w.RenderVp();
            Timer::Reset(&ts);
        }
    }
    //std::cin.get();
    return (int)msg.wParam;
}

