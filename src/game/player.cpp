// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <string>
#include "player.h"

namespace bwmodel {
    std::string PlayerColorHelper::name(PlayerColor color) {
        static const std::string color_names[] = {
            "red", "blue", "green", "aqua", "yellow", "white", "black", "pink"};
        return color_names[*color];
    }
}
