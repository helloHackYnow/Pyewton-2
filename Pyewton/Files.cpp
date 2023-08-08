#include "Files.h"

std::filesystem::path shaders_dir("shader_dir");

std::filesystem::path GetShaderPath(const char* path)
{
	std::filesystem::path current = std::filesystem::current_path();
	std::filesystem::path shader = current / shaders_dir / std::filesystem::path(path);
	//const char* shader_path = &();
	return shader;
}