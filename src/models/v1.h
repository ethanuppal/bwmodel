// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <array>
#include <optional>
#include "game/gamedelegate.h"
#include "game/player.h"
#include "core/heatmap.h"

namespace bwmodel {
    namespace Model {
        class v1 : public GameDelegate {
            std::array<std::unique_ptr<HeatMap>, PLAYER_COUNT> location;

            v1();

        public:
            void on_game_start(const Game& game) override;

            void on_players_match(const Game& game, PlayerColor victor,
                PlayerColor loser, bool was_final) override;

            void on_bed_broken(const Game& game, PlayerColor breaker,
                PlayerColor bed) override;

            void on_game_end(const Game& game) override;

            void tick(const Game& game) override;

            static std::shared_ptr<v1> make();
        };
    }
}
