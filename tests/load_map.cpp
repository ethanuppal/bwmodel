// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <iostream>
#include <cassert>
#include "bwmodel.h"

int main() {
    std::unique_ptr<bwmodel::Map> map =
        bwmodel::Map::load_from("./data/example.bwmap");
    assert(map->regions_at(0, 2 * bwmodel::REGION_H)
           == (bwmodel::RegionSet::PINK | bwmodel::RegionSet::BASE));
    assert(map->regions_at(4 * bwmodel::REGION_W, 2 * bwmodel::REGION_H)
           == (bwmodel::RegionSet::MIDDLE));
    assert(map->regions_at(7 * bwmodel::REGION_W, 7 * bwmodel::REGION_H)
           == (bwmodel::RegionSet::DIAMOND));
    return 0;
}
