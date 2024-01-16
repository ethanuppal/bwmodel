// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <iostream>
#include <cassert>
#include "bwmodel.h"
using namespace bwmodel;

int main() {
    try {
        std::unique_ptr<Map> map = Map::load_from("./data/example.bwmap");
        Game game(std::move(map));

    } catch (const MapLoadError& error) {
        std::cerr << "MapLoadError: " << error.what() << '\n';
    }
    return 0;
}
