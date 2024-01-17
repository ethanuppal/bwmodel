// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <memory>
#include <vector>
#include <array>
#include <chrono>
#include <cstdbool>
#include <set>
#include "game/map.h"
#include "game/player.h"
#include "game/gamedelegate.h"

namespace bwmodel {
    /** Models a bedwars game. */
    class Game {
        std::unique_ptr<Map> _map;
        std::array<bool, PLAYER_COUNT> _player_beds;
        std::set<PlayerColor> _players_left;
        std::vector<std::shared_ptr<GameDelegate>> delegates;
        std::chrono::time_point<std::chrono::steady_clock> start_time;

    public:
        /** Constructs a new game from `map`, taking ownership of it. */
        Game(std::unique_ptr<Map> map);

        /** @returns The game map. */
        const Map& map() const;

        /** The time in milliseconds since the game started. */
        uint64_t time() const;

        /** A list of remaining players. */
        const std::set<PlayerColor>& players_left() const;

        /** Whether the player whose color is `color` currently has a bed. */
        bool player_has_bed(PlayerColor color);

        /** Attaches the given `delegate` to notify for game events. */
        void attach(std::shared_ptr<GameDelegate> delegate);

        /** Begins the game. */
        void notify_start();

        /** Ends the game. */
        void notify_end();

        /** Whether the game should be ended. */
        bool should_end() const;

        /** Cycle the game. */
        void cycle();

        /** Player `victor` kills player `loser`. */
        void notify_player_kill(PlayerColor victor, PlayerColor loser);

        /** Player `breaker` breaks the bed for player whose color is given by
         * `bed`. */
        void notify_break_bed(PlayerColor breaker, PlayerColor bed);
    };
}
