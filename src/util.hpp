#pragma once
#include <string>
#include <iostream>

#include "config.hpp"

enum class FileMode
{
    Binary,
    Normal,
};
void replace_substr(std::string &tagetStr, const std::string find, const std::string replace);
std::string read_file(const std::string filename, FileMode mode);
std::string get_base_path();
std::string expand_path(const std::string virPath);
