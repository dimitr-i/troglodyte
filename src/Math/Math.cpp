#include "Math.h"

namespace trog
{
	float Math::hypo(float a, float b)
	{
		float result = pow(a, 2) + pow(b, 2);

		return sqrt(result);
	}
}