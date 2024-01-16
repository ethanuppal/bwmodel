// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <fstream>
#include <iostream>
#include "map.h"

namespace bwmodel {
    Map::Map(MapGrid& grid): grid(grid) {
        assert(!grid.empty());
        for (MapRow& row: grid) {
            assert(row.size() == grid[0].size());
        }
    }

    blocks_t Map::width() const {
        return grid[0].size();
    }

    blocks_t Map::height() const {
        return grid.size();
    }

    bool Map::update_region(blocks_t x, blocks_t y, RegionSet regions) {
        RegionSet current = regions_at(x, y);
        if (current == RegionSet::INVALID) {
            return false;
        } else {
            if (current == RegionSet::UNINIT) {
                grid[y][x] = regions;
            } else {
                grid[y][x] = current | regions;
            }
            return true;
        }
    }

    RegionSet Map::regions_at(blocks_t x, blocks_t y) const {
        if (x < 0 || y < 0 || x >= width() || y >= height()) {
            return RegionSet::INVALID;
        }
        return grid[y][x];
    }

    std::unique_ptr<Map> Map::load_from(const std::string& path) {
        std::ifstream in(path);

        blocks_t width, height;
        in >> width >> height;

        if (in.fail() || in.bad()) {
            throw MapLoadError("Error reading width and height from file: "
                               + path);
        }

        /** Prepare map to return */
        MapRow grid_row(width, RegionSet::UNINIT);
        MapGrid grid(height, grid_row);
        std::unique_ptr<Map> result(new Map(grid));

        /** Parse grid */
        for (blocks_t j = 0; j < height; j++) {
            for (blocks_t i = 0; i < width; i++) {
                std::string entry;
                in >> entry;

                if (in.fail() || in.bad()) {
                    throw MapLoadError(
                        "Error reading entry at (x=" + std::to_string(i)
                        + ", y=" + std::to_string(j) + ") in file: " + path);
                }

                for (char region: entry) {
                    switch (toupper(region)) {
                        case 'B':
                            assert(result->update_region(i, j,
                                RegionSet::BASE));
                            break;
                        case 'E':
                            assert(result->update_region(i, j,
                                RegionSet::EMPTY));
                            break;
                        case 'D':
                            assert(result->update_region(i, j,
                                RegionSet::DIAMOND));
                            break;
                        case 'M':
                            assert(result->update_region(i, j,
                                RegionSet::MIDDLE));
                            break;
                        default: {
                            if (region >= '1' && region <= '8') {
                                RegionSetBacking color =
                                    1 << (8 + (region - '1'));
                                assert(result->update_region(i, j,
                                    static_cast<RegionSet>(color)));
                            }
                            break;
                        }
                    }
                }

                if (result->regions_at(i, j) == RegionSet::UNINIT) {
                    throw MapLoadError("Error parsing entry '" + entry
                                       + "' at (x=" + std::to_string(i)
                                       + ", y=" + std::to_string(j)
                                       + ") in file: " + path);
                }
            }
        }

        return result;
    }
}
