#include "mesh/Mesh.h"
#include <iostream>

using namespace ees2d::Mesh;


int main() {
	std::cout << "Euler2D Software" << std::endl;
	std::string path = "../../tests/mesh/square.su2";
	Su2Mesh mymesh{path};
	mymesh.Parse();
}
