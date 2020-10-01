#include "mesh/Mesh.h"
#include <iostream>

using namespace ees2d::Mesh;


int main() {
	std::cout << "Euler2D Software" << std::endl;
	::std::string path = "../../tests/mesh/naca0012_euler_65x65x1_O_1B.su2";
	std::unique_ptr<Su2Mesh> mymesh(std::make_unique<Su2Mesh>(path));
	mymesh->Parse();
}
