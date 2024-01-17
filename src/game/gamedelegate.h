// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include "game/player.h"
#include "game/map.h"

namespace bwmodel {
    class Game;

    /** An interface for game events. */
    class GameDelegate {
    public:
        /** Called when the game is initialized. */
        virtual void on_game_start(const Game& game) = 0;

        /** Called when `victor` has defeated `loser`. */
        virtual void on_players_match(const Game& game, PlayerColor victor,
            PlayerColor loser, bool was_final) = 0;

        /** Called when `breaker` has broken `bed`. */
        virtual void on_bed_broken(const Game& game, PlayerColor breaker,
            PlayerColor bed) = 0;

        /** Called when the game ends. */
        virtual void on_game_end(const Game& game) = 0;

        /** Called repeatedly from after `on_game_start` until before
         * `on_game_end`. */
        virtual void tick(const Game& game) = 0;

        /** Called when the player position are updated. */
        virtual void on_player_move(const Game& game, int x, int y);

        /** Called when the player sees player `spotted` at `location`. */
        virtual void on_player_sight(const Game& game, PlayerColor spotted,
            RegionSet location);
    };
}
