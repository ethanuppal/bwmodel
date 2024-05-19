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
    /**
     * Models a bedwars game.
     *
     * \par Modeling Instructions
     *
     * When a player `player` (`bwmodel::PlayerColor`) enters a bedwars game on
     * a `map` (as a `std::unique_ptr` to bwmodel::Map), the game `Game(map,
     * player)` will model that game until it ends.
     *
     * Before beginning the game lifecycle (see next section), you may attach
     * delegates via Game::attach. In fact, you may do this at any point, but it
     * likely you wish to do this at the start.
     *
     * \par Game Lifecycle
     *
     * 1. Call Game::notify_start once when the game begins.
     * 2. Call Game::cycle as often as possible. In addition:
     *     * Call Game::notify_player_kill when a player kills another.
     *     * Call Game::notify_break_bed when a player breaks a bed.
     * 3. Call Game::notify_end when the game ends (Game::should_end).
     */
    class Game {
        std::unique_ptr<Map> _map;
        PlayerColor _me;
        std::array<bool, PLAYER_COUNT> _player_beds;
        std::set<PlayerColor> _players_left;
        std::vector<std::shared_ptr<GameDelegate>> delegates;
        std::chrono::time_point<std::chrono::steady_clock> start_time;

    public:
        /** Constructs a new game from `map`, taking ownership of it. See
         * Game::me() for information about `me`. */
        Game(std::unique_ptr<Map> map, PlayerColor me);

        /** @returns The game map. */
        const Map& map() const;

        /** The time in milliseconds since the game started. */
        uint64_t time() const;

        /** The player whose perspective this game is played from. */
        PlayerColor me() const;

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
