// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "game/gamedelegate.h"

namespace bwmodel {
    void GameDelegate::on_player_move(const Game& game, int x, int y) {}

    void GameDelegate::on_player_sight(const Game& game, PlayerColor spotted,
        RegionSet location) {}
}
