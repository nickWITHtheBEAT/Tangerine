#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <strstream>
#include "Utils.h"
#include "mafs.h"


class Model
{
public:
	Model(const char* file);
	uint tag;
	vec3f pos;
	vec3f rot;
	std::vector<vec4f> pts;
	std::vector<uint> index;
	std::vector <tri> tris;
	bool LoadObj(const char* file);
	~Model();
};

