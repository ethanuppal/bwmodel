// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include "game/map.h"

namespace bwmodel {
    /** Models a bedwars game. */
    class Game {
        std::unique_ptr<Map> _map;

    public:
        /** Constructs a new game from `map`, taking ownership of it. */
        Game(std::unique_ptr<Map> map);

        /** @returns The game map. */
        const Map& map() const;
    };
}
