// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "sim.h"

namespace bwmodel {
    Simulator::Simulator() {}

    void Simulator::sequence(uint64_t time, std::function<void(Game&)> event) {
        events.push({time, event});
    }

    void Simulator::simulate(Game& game) {
        game.notify_start();
        while (!game.should_end()) {
            while (!events.empty() && game.time() >= events.top().first) {
                events.top().second(game);
                events.pop();
            }
            game.cycle();
        }
        game.notify_end();
    }
}
