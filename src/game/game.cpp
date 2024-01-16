// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "game.h"

namespace bwmodel {
    Game::Game(std::unique_ptr<Map> map): _map(std::move(map)) {}

    const Map& Game::map() const {
        return *_map;
    }
}
