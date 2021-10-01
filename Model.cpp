#include "Model.h"

Model::Model(const char* file)
{
	LoadObj(file);
};

bool Model::LoadObj(const char* file)
{
	std::ifstream f(file);
	if (!f.is_open())
		return false;

	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		std::strstream s;
		s << line;

		char junk;

		if (line[0] == 'v')
		{
			vec4f v;
			s >> junk >> v.x >> v.y >> v.z;
			pts.push_back(v);
		}

		if (line[0] == 'f')
		{
			int f[3];
			s >> junk >> f[0] >> f[1] >> f[2];
			index.push_back(f[0] - 1);
			index.push_back(f[1] - 1);
			index.push_back(f[2] - 1);

		}
	}

	return true;
};

Model::~Model() {};