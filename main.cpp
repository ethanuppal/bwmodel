// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <iostream>
#include <cassert>
#include "map.h"

int main() {
    try {
        std::unique_ptr<bwmodel::Map> map =
            bwmodel::Map::load_from("./data/example.bwmap");
        assert(map->regions_at(0, 2)
               == (bwmodel::RegionSet::PINK | bwmodel::RegionSet::BASE));
        assert(map->regions_at(4, 2) == (bwmodel::RegionSet::MIDDLE));
        assert(map->regions_at(7, 7) == (bwmodel::RegionSet::DIAMOND));
    } catch (const bwmodel::MapLoadError& error) {
        std::cerr << "MapLoadError: " << error.what() << '\n';
    }
    return 0;
}
