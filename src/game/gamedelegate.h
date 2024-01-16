// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include "game/player.h"
#include "game/game.h"

namespace bwmodel {
    /** An interface for game events. */
    class GameDelegate {
        /** Called when the game is initialized. */
        virtual void on_game_start(const Game& game) = 0;

        /** Called when `victor` has defeated `loser`. */
        virtual void on_players_match(const Game& game, PlayerColor victor,
            PlayerColor loser) = 0;

        /** Called when `breaker` has broken `bed`. */
        virtual void on_bed_broken(PlayerColor breaker, PlayerColor bed) = 0;

        /** Called when the game ends. */
        virtual void on_game_end(const Game& game) = 0;
    };
}
