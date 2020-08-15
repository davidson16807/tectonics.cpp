

#define GLM_FORCE_PURE  // disable SIMD support for glm so we can work with webassembly
#include <glm/vec3.hpp> // *vec3

#include <composite>
#include <iostream>

int main(int argc, char const *argv[])
{
	auto a = std::valarray<glm::vec3> { glm::vec3(1,2,3) };
	auto b = std::valarray<glm::vec3> { glm::vec3(1,1,3) };
	auto c = a + b;
	std::cout << c[0].x << " " << c[0].y << " " << c[0].z << std::endl;
}
