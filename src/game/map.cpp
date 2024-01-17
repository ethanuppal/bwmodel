// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <fstream>
#include <iostream>
#include "game/map.h"

namespace bwmodel {
    Map::Map(std::shared_ptr<Grid<RegionSet>> grid): grid(grid) {
        assert(!grid->empty());
        assert_grid_invariant();
    }

    const Grid<RegionSet>& Map::backing() const {
        return *grid;
    }

    bool Map::update_region(blocks_t x, blocks_t y, RegionSet regions) {
        RegionSet current = regions_at(x, y);
        if (current == RegionSet::INVALID) {
            return false;
        } else {
            (*grid)[y / REGION_H][x / REGION_W] |= regions;
            return true;
        }
    }

    RegionSet Map::regions_at(blocks_t x, blocks_t y) const {
        if (x < 0 || y < 0 || x >= REGION_W * width()
            || y >= REGION_H * height()) {
            return RegionSet::INVALID;
        }
        assert((*grid)[y / REGION_H][x / REGION_W] != RegionSet::INVALID);
        return (*grid)[y / REGION_H][x / REGION_W];
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
        GridRow<RegionSet> grid_row(width, RegionSet::INVALID);
        std::shared_ptr<Grid<RegionSet>> grid =
            std::make_shared<Grid<RegionSet>>(height, grid_row);
        std::unique_ptr<Map> result(new Map(grid));

        /** Parse grid */
        for (blocks_t j = 0; j < height; j++) {
            for (blocks_t i = 0; i < width; i++) {
                std::string entry;
                in >> entry;

                if (in.fail() || in.bad()) {
                    throw MapLoadError(
                        "Error reading entry at (col=" + std::to_string(i)
                        + ", row=" + std::to_string(j) + ") in file: " + path);
                }

                for (char region: entry) {
                    switch (toupper(region)) {
                        case 'B':
                            (*grid)[j][i] |= RegionSet::BASE;
                            break;
                        case 'E':
                            (*grid)[j][i] |= RegionSet::EMPTY;
                            break;
                        case 'D':
                            (*grid)[j][i] |= RegionSet::DIAMOND;
                            break;
                        case 'M':
                            (*grid)[j][i] |= RegionSet::MIDDLE;
                            break;
                        default: {
                            if (region >= '1' && region <= '8') {
                                RegionSetBacking color =
                                    1 << (8 + (region - '1'));
                                (*grid)[j][i] |= static_cast<RegionSet>(color);
                            }
                            break;
                        }
                    }
                }

                if ((*grid)[j][i] == RegionSet::INVALID) {
                    throw MapLoadError("Error parsing entry '" + entry
                                       + "' at (col=" + std::to_string(i)
                                       + ", row=" + std::to_string(j)
                                       + ") in file: " + path);
                }
            }
        }

        return result;
    }

    RegionSet RegionSetHelper::from(PlayerColor color) {
        return static_cast<RegionSet>(1 << (8 + *color));
    }
}
