#include "Pipeline.h"

Pipeline::Pipeline(Model* models, const uint count, Graphics* gfx)
    :m(models), gfx(gfx), count(count)
{
    for (Model* ptr = &m[0]; ptr <= &m[count - 1]; ptr++)
    {
        for (int i = 0; i < ptr->index.size(); i += 3)
        {
            ptr->tris.push_back({ ptr->pts[ptr->index[i]], ptr->pts[ptr->index[i + 1]],
                ptr->pts[ptr->index[i + 2]] });
        }
    }
}

void Pipeline::Present(float a)
{
    Color color[6] = { Colors::Blue, Colors::Green, Colors::LightGray, Colors::Magenta
                        , Colors::Yellow, Colors::Cyan };
    mat4f rotY, rotZ, rotX, proj, scale, translate;
    vec4f vCam = { 0.0f, 0.0f , 0.0f, 1.0f };
    vec4f vLight = { 0.0f, 0.0f , -1.0f, 1.0f };
    MakeScaleUni(scale, 0.4f);
    MakeProj(proj, 0.8f, 100.0f, 45.0f, gfx->view.width / gfx->view.height);

    mat4f world;
    float dot = 0.0f; vec4f e1, e2, n; float light_dot = 0.0f;
    float zinv;
    for (Model* ptr = &m[0]; ptr <= &m[count - 1]; ptr++)
    {
        MakeRotY(rotY, a * ptr->rot.x);
        MakeRotX(rotX, 0 * PI / 3);
        MakeTranslation(translate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
        world = rotY * rotX * scale * translate;
        int index = 0;
        for (tri t : ptr->tris)
        {

            vmmult(world, t.pts[0]);
            vmmult(world, t.pts[1]);
            vmmult(world, t.pts[2]);

            n = Cross4f(t.pts[2] - t.pts[0], t.pts[1] - t.pts[0]);
            Normalize(n);
            dot = Dot4f(n, t.pts[0] - vCam);
            light_dot = Dot4f(n, vLight);

            //if (dot >= -0.5f)
            {
                for (int i = 0; i < 3; i++)
                {

                    //vmmult(proj, t.pts[i]);
                    //
                    //zinv = t.pts[i].w;
                    //
                    //t.pts[i].x /= t.pts[i].w;
                    //t.pts[i].y /= t.pts[i].w;
                    //t.pts[i].z /= t.pts[i].w;
                    t.pts[i].w = t.pts[i].z;
                    
                    t.pts[i].x += 1.0f;
                    t.pts[i].y += 1.0f;
                    
                    t.pts[i].x *= 0.5f * (float)gfx->view.width;
                    t.pts[i].y *= 0.5f * (float)gfx->view.height;
                }
                Color grayscale = Colors::Red;
                //grayscale.SetA(0xff);
                grayscale.SetB(int(Colors::LightGray.GetB() * light_dot));
                grayscale.SetG(int(Colors::LightGray.GetG() * light_dot));
                grayscale.SetR(int(Colors::LightGray.GetR() * light_dot));
                                                                       
                gfx->FillTriInterp(t.pts[0], t.pts[1], t.pts[2], color[index].uint/*grayscale.uint*/, false);
                if (index == 5)index = 0;
                index++;
            }
        }
    }
}