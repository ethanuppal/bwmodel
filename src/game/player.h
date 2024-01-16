// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

namespace bwmodel {
    /** Bedwars player/team color. */
    enum class PlayerColor {
        RED,
        BLUE,
        GREEN,
        AQUA,
        YELLOW,
        WHITE,
        BLACK,
        PINK,
        _PLAYER_COUNT
    };

    // likely not idiomatic/very hacky/bad practice
    /** Retreives the `int` value of the player color `color`. */
    constexpr int operator*(PlayerColor color) {
        return static_cast<int>(color);
    }

    /** The number of players in a bedwars game. */
    constexpr int PLAYER_COUNT = *PlayerColor::_PLAYER_COUNT;
}
