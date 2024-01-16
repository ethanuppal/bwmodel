// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <array>
#include "player.h"
#include "map.h"

namespace bwmodel {
    class HeatMap {
        MapGrid<double> distribution;
    };
    class Model {
        std::array<HeatMap, PLAYER_COUNT> location;

    public:
    };
}
