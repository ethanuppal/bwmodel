// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "v1.h"
#include "game/game.h"

namespace bwmodel {
    namespace Model {
        v1::v1() {}

        void v1::on_game_start(const Game& game) {
            blocks_t width = game.map().width();
            blocks_t height = game.map().height();

            // set all players to somewhere on their base
            for (int i = 0; i < PLAYER_COUNT; i++) {
                location[i] = HeatMap::uniform(width, height);

                RegionSet base_region =
                    RegionSet::BASE
                    | RegionSetHelper::from(static_cast<PlayerColor>(i));

                std::vector<std::pair<int, int>> region_points;
                for (blocks_t y = 0; y < height; y++) {
                    for (blocks_t x = 0; x < width; x++) {
                        if (game.map().regions_at(x, y) == base_region) {
                            region_points.push_back({x, y});
                        }
                    }
                }

                location[i]->concentrate_at(region_points);
            }
        }

        void v1::on_players_match(const Game& game, PlayerColor victor,
            PlayerColor loser, bool was_final) {}

        void v1::on_bed_broken(const Game& game, PlayerColor breaker,
            PlayerColor bed) {}

        void v1::on_game_end(const Game& game) {}

        void v1::tick(const Game& game) {}

        std::shared_ptr<v1> v1::make() {
            return std::shared_ptr<v1>(new v1());
        }
    }
}
