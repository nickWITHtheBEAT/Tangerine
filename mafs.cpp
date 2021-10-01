#include "mafs.h"

void MakeRotX(mat4f& res, float f)
{
	res.m[0][0] = 1.0f;
	res.m[1][1] = cosf(f);
	res.m[1][2] = sinf(f);
	res.m[2][1] = -sinf(f);
	res.m[2][2] = cosf(f);
	res.m[3][3] = 1.0f;
}

void MakeRotY(mat4f& res, float f)
{
	res.m[0][0] = cosf(f);
	res.m[0][2] = sinf(f);
	res.m[2][0] = -sinf(f);
	res.m[1][1] = 1.0f;
	res.m[2][2] = cosf(f);
	res.m[3][3] = 1.0f;
}

void MakeRotZ(mat4f& res, float f)
{
	res.m[0][0] = cosf(f);
	res.m[0][1] = sinf(f);
	res.m[1][0] = -sinf(f);
	res.m[1][1] = cosf(f);
	res.m[2][2] = 1.0f;
	res.m[3][3] = 1.0f;
}

mat4f Matrix_MakeIdentity()
{
	mat4f res;
	res.m[0][0] = 1.0f;
	res.m[1][1] = 1.0f;
	res.m[2][2] = 1.0f;
	res.m[3][3] = 1.0f;
	return res;
}

void MakeProj(mat4f& res, float Near, float Far, float fov, float aspectratio)
{
	float fnear = Near;
	float ffar = Far;
	float ffov = fov;
	float faspectratio = aspectratio;
	float ffovrad = 1.0f / tanf(ffov * 0.5f / 180.0f * 3.14159f);

	res.m[0][0] = faspectratio * ffovrad;
	res.m[1][1] = ffovrad;
	res.m[2][2] = ffar / (ffar - fnear);
	res.m[3][2] = (-ffar * fnear) / (ffar - fnear);
	res.m[2][3] = 1.0f;
	res.m[3][3] = 0.0f;
}

void vmmult(mat4f& m, vec4f& i)
{
	vec4f o = i;
	i.x = o.x * m.m[0][0] + o.y * m.m[1][0] + o.z * m.m[2][0] + o.w * m.m[3][0];
	i.y = o.x * m.m[0][1] + o.y * m.m[1][1] + o.z * m.m[2][1] + o.w * m.m[3][1];
	i.z = o.x * m.m[0][2] + o.y * m.m[1][2] + o.z * m.m[2][2] + o.w * m.m[3][2];
	i.w = o.x * m.m[0][3] + o.y * m.m[1][3] + o.z * m.m[2][3] + o.w * m.m[3][3];
}

void MakeTranslation(mat4f& res, float x, float y, float z)
{
	res.m[0][0] = 1.0f;
	res.m[1][1] = 1.0f;
	res.m[2][2] = 1.0f;
	res.m[3][3] = 1.0f;
	res.m[3][0] = x;
	res.m[3][1] = y;
	res.m[3][2] = z;
}

mat4f mmmult(const mat4f& a, const mat4f& b)
{
	mat4f res;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
		{
			res.m[r][c] = a.m[r][0] * b.m[0][c] +
				a.m[r][1] * b.m[1][c] +
				a.m[r][2] * b.m[2][c] +
				a.m[r][3] * b.m[3][c];
		}
	return res;
}

mat4f operator* (const mat4f& a, const mat4f& b)
{
	mat4f res;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
		{
			res.m[r][c] = a.m[r][0] * b.m[0][c] +
				a.m[r][1] * b.m[1][c] +
				a.m[r][2] * b.m[2][c] +
				a.m[r][3] * b.m[3][c];
		}
	return res;
}

vec4f operator+(const vec4f& a, const vec4f& b)
{
	vec4f res;
	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;
	return res;
}

vec4f operator-(const vec4f& a, const vec4f& b)
{
	vec4f res;
	res.x = a.x - b.x;
	res.y = a.y - b.y;
	res.z = a.z - b.z;
	return res;
}

vec4f operator*(const vec4f& v, const float x)
{
	vec4f res;
	res.x = v.x * x;
	res.y = v.y * x;
	res.z = v.z * x;
	return res;
}

vec4f operator*(const float x, const vec4f& v)
{
	vec4f res;
	res.x = v.x * x;
	res.y = v.y * x;
	res.z = v.z * x;
	return res;
}


float invRoot(float x, bool pres)
{
	long i;
	float number = x, x2, y;
	const float threehalfs = 1.5f;

	//Wiki copy pasta -_-
	x2 = number * 0.5f;
	y = number;
	i = *(long*)&y;                            // Floating point bit hack
	i = 0x5f3759df - (i >> 1);               // Magic number 
	y = *(float*)&i;
	y *= (threehalfs - (x2 * y * y));   // Newton 1st iteration
	if(pres)y *= ( threehalfs - ( x2 * y * y ) );   // 2nd iteration (disabled)
	return y;
}

void Normalize(vec4f& res, bool pres)
{
	float number = res.x * res.x + res.y * res.y + res.z * res.z;
	float y = invRoot(number, pres);

	res.x *= y;
	res.y *= y;
	res.z *= y;
}

void NormalizeO(const vec4f& in, vec4f& out, bool pres)
{

	float number = in.x * in.x + in.y * in.y+ in.z * in.z;
	float y = invRoot(number, pres);
	out = in;
	out.x *= y;
	out.y *= y;
	out.z *= y;
}

void NormalizeSlow(vec4f& i)
{
	float d = 1.0f / sqrtf(fabsf(i.x * i.x + i.y * i.y+ i.z * i.z));
	i.x *= d;
	i.y *= d;
	i.z *= d;
}

void MakeScale(mat4f& res, float a, float b, float c)
{
	res.m[0][0] = a;
	res.m[1][1] = b;
	res.m[2][2] = c;
	res.m[3][3] = 1.0f;
}

void MakeScaleUni(mat4f& res, float u)
{
	res.m[0][0] = u;
	res.m[1][1] = u;
	res.m[2][2] = u;
	res.m[3][3] = 1.0f;
}

float Dot4f(const vec4f& v1, const vec4f& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

vec4f Cross4f(const vec4f& v1, const vec4f& v2)
{
	vec4f v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}