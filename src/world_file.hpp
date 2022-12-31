#pragma once

#include <string>

struct World;

void writeWorld(const std::string& dir_path, const World& world);
void readWorld(const std::string& dir_path, World& world);
