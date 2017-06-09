#include <iostream>
#include <cxxopts.hpp>
#include "viewer.hpp"
using namespace w3dview;

int main(int argc, char** argv)
{
	cxxopts::Options options("W3D Viewer", "A program to view Westwood 3D Models");
	options.add_options()
		("f,file", "File name", cxxopts::value<std::string>())
		("w,width", "Width of the window", cxxopts::value<unsigned int>())
		("h,height", "Height of the window", cxxopts::value<unsigned int>())
		("a,animation", "Play back an animation", cxxopts::value<std::string>());

	options.parse(argc, argv);
	if (options.count("file") == 0)
	{
		std::cout << "Please specify an input file." << std::endl << options.help() << std::endl;
		return EXIT_FAILURE;
	}

	Viewer v;
	//Set input file
	v.SetInput(options["file"].as<std::string>());
	if (options.count("width") > 0)
		v.SetWidth(options["width"].as<unsigned int>());
	if (options.count("height") > 0)
		v.SetHeight(options["height"].as<unsigned int>());
	if (options.count("animation") > 0)
		v.SetAnimation(options["animation"].as<std::string>());
	v.Run();

	return 0;
}