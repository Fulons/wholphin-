#include "Noise.h"


namespace wholphin {

	float sinPiOver4 = 0.70710678118f;


	static glm::vec2 gradients2D[] = {
		glm::vec2(+sinPiOver4, +sinPiOver4), glm::vec2(+1, 0), glm::vec2(sinPiOver4, -sinPiOver4), glm::vec2(0, -1),
		glm::vec2(-sinPiOver4, -sinPiOver4), glm::vec2(-1, 0), glm::vec2(-sinPiOver4, sinPiOver4), glm::vec2(0, 1)
	};

	static glm::vec3 gradients3d[] = {
		glm::vec3(1, 1, 0), glm::vec3(-1, +1, 0), glm::vec3(1, -1, +0), glm::vec3(-1, -1, +0),
		glm::vec3(1, 0, 1), glm::vec3(-1, +0, 1), glm::vec3(1, +0, -1), glm::vec3(-1, +0, -1),
		glm::vec3(0, 1, 1), glm::vec3(+0, -1, 1), glm::vec3(0, +1, -1), glm::vec3(+0, -1, -1)
	};

	//TODO: create a shuffle algorithm that shuffles this values good depending on a seed
	static int permutation[] = {
		151,160,137,91,90,15,
		131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
		190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
		88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
		77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
		102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
		135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
		5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
		223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
		129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
		251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
		49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
		151,160,137,91,90,15,
		131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
		190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
		88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
		77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
		102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
		135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
		5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
		223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
		129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
		251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
		49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};

	int fastFloor(float x) { return x >= 0 ? (int)x : (int)(x - 1); }
	glm::ivec2 fastFloor(glm::vec2 x) { return glm::ivec2(fastFloor(x.x), fastFloor(x.y)); }
	glm::ivec3 fastFloor(glm::vec3 x) { return glm::ivec3(fastFloor(x.x), fastFloor(x.y), fastFloor(x.z)); }
	float lerp(float a, float b, float t) { return (1 - t)*a + b * t; }
	float fade(float x) { return x*x*x*(x*(x * 6 - 15) + 10); }

	float Perlin(glm::vec2 in)	{
		glm::ivec2 floor = fastFloor(in);
		glm::vec2 relative = in - (glm::vec2)floor;
		glm::ivec2 floorWrapped = glm::ivec2(floor.x & 255, floor.y & 255);

		int gradientIndex00 = permutation[floorWrapped.x + 0 + permutation[floorWrapped.y + 0]] % 8;
		int gradientIndex01 = permutation[floorWrapped.x + 0 + permutation[floorWrapped.y + 1]] % 8;
		int gradientIndex10 = permutation[floorWrapped.x + 1 + permutation[floorWrapped.y + 0]] % 8;
		int gradientIndex11 = permutation[floorWrapped.x + 1 + permutation[floorWrapped.y + 1]] % 8;
		
		float noiseContribution00 = glm::dot(gradients2D[gradientIndex00], glm::vec2(relative.x, relative.y));
		float noiseContribution01 = glm::dot(gradients2D[gradientIndex01], glm::vec2(relative.x, relative.y - 1));
		float noiseContribution10 = glm::dot(gradients2D[gradientIndex10], glm::vec2(relative.x - 1, relative.y));
		float noiseContribution11 = glm::dot(gradients2D[gradientIndex11], glm::vec2(relative.x - 1, relative.y - 1));

		float u = fade(relative.x);
		float v = fade(relative.y);

		float xInterpolated0 = lerp(noiseContribution00, noiseContribution10, u);
		float xInterpolated1 = lerp(noiseContribution01, noiseContribution11, u);

		return lerp(xInterpolated0, xInterpolated1, v);
	}

	float Perlin(glm::vec3 in)	{
		glm::ivec3 floor = fastFloor(in);
		glm::vec3 relative = in - (glm::vec3)floor;
		glm::ivec3 floorWrapped = glm::ivec3(floor.x & 255, floor.y & 255, floor.z & 255);

		int gradientIndex000 = permutation[floorWrapped.x + 0 + permutation[floorWrapped.y + 0 + permutation[floorWrapped.z + 0]]] % 12;
		int gradientIndex001 = permutation[floorWrapped.x + 0 + permutation[floorWrapped.y + 0 + permutation[floorWrapped.z + 1]]] % 12;
		int gradientIndex010 = permutation[floorWrapped.x + 0 + permutation[floorWrapped.y + 1 + permutation[floorWrapped.z + 0]]] % 12;
		int gradientIndex011 = permutation[floorWrapped.x + 0 + permutation[floorWrapped.y + 1 + permutation[floorWrapped.z + 1]]] % 12;
		int gradientIndex100 = permutation[floorWrapped.x + 1 + permutation[floorWrapped.y + 0 + permutation[floorWrapped.z + 0]]] % 12;
		int gradientIndex101 = permutation[floorWrapped.x + 1 + permutation[floorWrapped.y + 0 + permutation[floorWrapped.z + 1]]] % 12;
		int gradientIndex110 = permutation[floorWrapped.x + 1 + permutation[floorWrapped.y + 1 + permutation[floorWrapped.z + 0]]] % 12;
		int gradientIndex111 = permutation[floorWrapped.x + 1 + permutation[floorWrapped.y + 1 + permutation[floorWrapped.z + 1]]] % 12;

		float noiseContribution000 = glm::dot(gradients3d[gradientIndex000], glm::vec3(relative.x - 0, relative.y - 0, relative.z - 0));
		float noiseContribution001 = glm::dot(gradients3d[gradientIndex001], glm::vec3(relative.x - 0, relative.y - 0, relative.z - 1));
		float noiseContribution010 = glm::dot(gradients3d[gradientIndex010], glm::vec3(relative.x - 0, relative.y - 1, relative.z - 0));
		float noiseContribution011 = glm::dot(gradients3d[gradientIndex011], glm::vec3(relative.x - 0, relative.y - 1, relative.z - 1));
		float noiseContribution100 = glm::dot(gradients3d[gradientIndex100], glm::vec3(relative.x - 1, relative.y - 0, relative.z - 0));
		float noiseContribution101 = glm::dot(gradients3d[gradientIndex101], glm::vec3(relative.x - 1, relative.y - 0, relative.z - 1));
		float noiseContribution110 = glm::dot(gradients3d[gradientIndex110], glm::vec3(relative.x - 1, relative.y - 1, relative.z - 0));
		float noiseContribution111 = glm::dot(gradients3d[gradientIndex111], glm::vec3(relative.x - 1, relative.y - 1, relative.z - 1));

		float u = fade(relative.x);
		float v = fade(relative.y);
		float w = fade(relative.z);

		float xInterpolated00 = lerp(noiseContribution000, noiseContribution100, u);
		float xInterpolated10 = lerp(noiseContribution010, noiseContribution110, u);
		float xInterpolated01 = lerp(noiseContribution001, noiseContribution101, u);
		float xInterpolated11 = lerp(noiseContribution011, noiseContribution111, u);

		float xyInterpolated0 = lerp(xInterpolated00, xInterpolated10, v);
		float xyInterpolated1 = lerp(xInterpolated01, xInterpolated11, v);

		return lerp(xyInterpolated0, xyInterpolated1, w);
	}
}