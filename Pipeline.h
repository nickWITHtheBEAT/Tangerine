#pragma once
#include "Graphics.h"
#include "Model.h"

class Pipeline
{
public:
	Pipeline(Model* models, const uint count, Graphics* gfx);
	void Present(float dt);
private:
	Model* m;
	Graphics* gfx;
	uint count;
};