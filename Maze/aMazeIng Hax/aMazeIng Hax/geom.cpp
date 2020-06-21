#include "stdafx.h"
#include "geom.h"

vec3 Add(vec3 src, vec3 dst)
{
	vec3 sum;
	sum.x = src.x + dst.x;
	sum.x = src.y + dst.y;
	sum.x = src.z + dst.z;
	return sum;
}