#include <iostream>     // cout

#define GLM_FORCE_PURE  // disable anonymous structs so we can build with ISO C++
#include <glm/glm.hpp>  // *vec3

#include <many/many.hpp>
#include <rasters/mesh.hpp>

using namespace rasters;
using namespace rasters::meshes;

int main(int argc, char const *argv[])
{
	std::cout << meshes::icosahedron.vertices.size() << std::endl;
	std::cout << meshes::icosahedron.faces.size() << std::endl;
	std::cout << std::endl;

	mesh subdivided1 = meshes::subdivide(meshes::icosahedron);
	std::cout << subdivided1.vertices.size() << std::endl;
	std::cout << subdivided1.faces.size() << std::endl;
	std::cout << std::endl;

	mesh subdivided2 = meshes::subdivide(subdivided1);
	std::cout << subdivided2.vertices.size() << std::endl;
	std::cout << subdivided2.faces.size() << std::endl;
	std::cout << std::endl;

	mesh subdivided3 = meshes::subdivide(subdivided2);
	std::cout << subdivided3.vertices.size() << std::endl;
	std::cout << subdivided3.faces.size() << std::endl;
	std::cout << std::endl;
}
