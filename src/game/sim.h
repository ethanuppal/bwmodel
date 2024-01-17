// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <queue>
#include <vector>
#include <functional>
#include <utility>
#include "game.h"

namespace bwmodel {
    class Simulator {
        using Event = std::pair<uint64_t, std::function<void(Game&)>>;

        struct EventCompare {
            bool operator()(const Event& a, const Event& b) const {
                return a.first > b.first;
            }
        };

        std::priority_queue<Event, std::vector<Event>, EventCompare> events;

    public:
        Simulator();

        /** Sequences `event` to occur at the given millisecond offset `time`.
         */
        void sequence(uint64_t time, std::function<void(Game&)> event);

        /** Simulates the given `game`, **consuming** previously sequenced
         * events as they are carried out. */
        void simulate(Game& game);
    };
}
