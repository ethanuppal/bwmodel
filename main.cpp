// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <iostream>
#include <cassert>
#include "bwmodel.h"
using namespace bwmodel;

int main() {
    try {
        // create game for map
        std::unique_ptr<Map> map = Map::load_from("./data/example.bwmap");
        PlayerColor me = PlayerColor::BLUE;
        Game game(std::move(map), me);

        // we want to test our v1 model
        std::shared_ptr<GameDelegate> model = Model::v1::make();
        game.attach(model);

        Simulator simulator;

        // we are about to POP OFF
        int off = 1;
        for (int i = 0; i < PLAYER_COUNT; i++) {
            if (i != *me) {
                int local_i = i;
                simulator.sequence(500 * (i + off), [me, local_i](Game& game) {
                    game.notify_break_bed(me,
                        static_cast<PlayerColor>(local_i));
                    game.notify_player_kill(me,
                        static_cast<PlayerColor>(local_i));
                });
            } else {
                off--;
            }
        }

        simulator.simulate(game);
    } catch (const MapLoadError& error) {
        std::cerr << "MapLoadError: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
