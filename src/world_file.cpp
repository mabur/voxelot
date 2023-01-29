#include "world_file.hpp"

#include <fstream>
#include <iostream>

#include "map.hpp"
#include "world.hpp"

namespace {

std::string buildingBlockfileName(BuildingBlock b) {
    return "block" + std::to_string(static_cast<int>(b) + 1) + ".csv";
}

void writeMap(const std::string& file_path, const Map& map) {
    using namespace std;
    ofstream file_csv(file_path);
    if (file_csv.fail()) {
        cout << "Could not read " << file_path;
        exit(1);
    }
    file_csv << "type,x,y,z\n";
    for (auto z = 0; z < map.depthGrid(); ++z) {
        for (auto y = 0; y < map.heightGrid(); ++y) {
            for (auto x = 0; x < map.widthGrid(); ++x) {
                const auto block = map.voxels(x, y, z);
                if (block) {
                    file_csv
                        << block << ","
                        << x << ","
                        << y << ","
                        << z << "\n";
                }
            }
        }
    }
    file_csv.close();
}

Map readMap(const std::string& file_path) {
    auto map = Map{};
    using namespace std;
    ifstream file(file_path);
    if (file.fail()) {
        cout << "Could not read " << file_path;
        std::exit(1);
    }
    string header;
    getline(file, header, '\n');
    while (true) {
        string type;
        getline(file, type, ',');
        if (type.empty()) break;
        string x;
        getline(file, x, ',');
        string y;
        getline(file, y, ',');
        string z;
        getline(file, z, '\n');

        const auto zi = std::stoi(z);
        const auto yi = std::stoi(y);
        const auto xi = std::stoi(x);
        const auto block = std::stoi(type);

        map.voxels(xi, yi, zi) = block;
    }
    file.close();
    return map;
}

} // namespace

void writeWorld(const std::string& dir_path, const World& world) {
    for (const auto& item : world.building_blocks) {
        const auto file_name = buildingBlockfileName(item.first);
        const auto file_path = dir_path + file_name;
        writeMap(file_path, item.second);
    }
}

void readWorld(const std::string& dir_path, World& world) {
    for (const auto& item : world.building_blocks) {
        const auto file_name = buildingBlockfileName(item.first);
        const auto file_path = dir_path + file_name;
        world.building_blocks[item.first] = readMap(file_path);
    }
}
