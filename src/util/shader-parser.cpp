#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <string>

#include "shader-parser.hpp"

std::string parseShader(const char* path){

	std::ifstream file(path);
	std::string result;

	std::stringstream vShaderStream;;
	vShaderStream << file.rdbuf();
	file.close();
    result = vShaderStream.str();
    return result;
}
