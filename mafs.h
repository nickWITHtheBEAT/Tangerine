#pragma once
#include <corecrt_math.h>
#include "Colors.h"

constexpr float PI = 3.14159f;

struct vec2i {
	int x = 0, y = 0;
};

struct vec2f {
	float x = 0.0f, y = 0.0f;
};

struct vec3f {
	float x = 0.0f, y = 0.0f, z = 0.0f;
};

struct vec4f {
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;
};

struct tri {
	vec4f pts[3];
	Color c;
};

struct mat4f
{
	float m[4][4] = { 0 };
};



mat4f operator * (const mat4f& a, const mat4f& b);

vec4f operator + (const vec4f& a, const vec4f& b);

vec4f operator - (const vec4f& a, const vec4f& b);

vec4f operator * (const vec4f& v, const float x);

vec4f operator * (const float x, const vec4f& v);

float invRoot(float x, bool pres = false);

mat4f Matrix_MakeIdentity();

void MakeRotX(mat4f& res, float f);

void MakeRotY(mat4f& res, float f);

void MakeRotZ(mat4f& res, float f);

void MakeProj(mat4f& res, float Near, float Far, float fov, float aspectratio);

void vmmult(mat4f& m, vec4f& i);

void MakeTranslation(mat4f& res, float x, float y, float z);

mat4f mmmult(const mat4f& a, const mat4f& b);

void NormalizeSlow(vec4f& i);

 void Normalize(vec4f& res, bool pres = false);

 void MakeScale(mat4f& res, float a, float b, float c);
 
 void MakeScaleUni(mat4f& res, float u);

 float Dot4f(const vec4f& v1, const vec4f& v2);

 vec4f Cross4f(const vec4f& v1, const vec4f& v2);

 void NormalizeO(const vec4f& in, vec4f& out, bool pres = false);
