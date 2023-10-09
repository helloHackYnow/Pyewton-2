#pragma once

#include <string>
#include <filesystem>

std::filesystem::path GetShaderPath(const char* path);
std::filesystem::path GetModelPath(const char* path);
std::filesystem::path GetJsonFolder();