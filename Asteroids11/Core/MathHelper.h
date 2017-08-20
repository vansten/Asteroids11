#pragma once

#include <ctime>

#include "glm/glm.hpp"

class MathHelper
{
public:
	static glm::vec3 Right;
	static glm::vec3 Up;
	static glm::vec3 Forward;

public:
	inline static void InitRand()
	{
		srand((unsigned int)(time(NULL)));
	}

	//Generates number in [min, max] range
	inline static float RandomRange(float min, float max)
	{
		return min + (rand() / (float)RAND_MAX) * (max - min);
	}

	//Generates number in [0,1] range
	inline static float Random()
	{
		return (rand() / (float)RAND_MAX);
	}

	inline static float MoveTowards(float from, float to, float maxDelta)
	{
		float delta = to - from;
		if(glm::abs(delta) > maxDelta)
		{
			delta = glm::sign(delta) * maxDelta;
		}

		return from + delta;
	}
};

