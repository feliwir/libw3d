#include <libw3d/w3d.hpp>

int main(int argc, char** argv)
{
	if (argc != 2)
		return -1;

	libw3d::Model m = libw3d::Loader::FromFile(argv[1]);
	return 0;
}