// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include "game.h"
#include "util/logger.h"

namespace bwmodel {
    Game::Game(std::unique_ptr<Map> map): _map(std::move(map)) {
        std::fill(_player_beds.begin(), _player_beds.end(), true);
        for (int i = 0; i < PLAYER_COUNT; i++) {
            _players_left.insert(static_cast<PlayerColor>(i));
        }
    }

    const Map& Game::map() const {
        return *_map;
    }

    uint64_t Game::time() const {
        auto now = std::chrono::steady_clock::now();
        auto duration_since_start = now - start_time;
        return (uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(
            duration_since_start)
            .count();
    }

    const std::set<PlayerColor>& Game::players_left() const {
        return _players_left;
    }

    bool Game::player_has_bed(PlayerColor color) {
        return _player_beds[*color];
    }

    void Game::attach(std::shared_ptr<GameDelegate> delegate) {
        delegates.push_back(delegate);
        BW_Log << "attached delegate " << delegate << '\n';
    }

    void Game::notify_start() {
        for (auto delegate: delegates) {
            delegate->on_game_start(*this);
        }
        start_time = std::chrono::steady_clock::now();
        BW_Log << "(0ms) game started\n";
    }

    void Game::notify_end() {
        for (auto delegate: delegates) {
            delegate->on_game_end(*this);
        }
        BW_Log << "(" << time() << "ms) "
               << "game ended\n";
    }

    bool Game::should_end() const {
        return _players_left.size() == 1;  // TODO: or time run out
    }

    void Game::cycle() {
        for (auto delegate: delegates) {
            delegate->tick(*this);
        }
    }

    void Game::notify_player_kill(PlayerColor victor, PlayerColor loser) {
        assert(victor != loser);
        assert(_players_left.count(victor));
        assert(_players_left.count(loser));

        // handle final kills
        bool was_final = false;
        if (!_player_beds[*loser]) {
            _players_left.erase(loser);
            was_final = true;
        }

        BW_Log << "(" << time() << "ms) " << PlayerColorHelper::name(victor)
               << " killed " << PlayerColorHelper::name(loser)
               << (was_final ? " (final kill!)" : "") << '\n';

        for (auto delegate: delegates) {
            delegate->on_players_match(*this, victor, loser, was_final);
        }
    }

    void Game::notify_break_bed(PlayerColor breaker, PlayerColor bed) {
        assert(breaker != bed);
        assert(_players_left.count(breaker));
        assert(_player_beds[*breaker]);

        // update bed status
        _player_beds[*bed] = false;

        BW_Log << "(" << time() << "ms) " << PlayerColorHelper::name(breaker)
               << " broke " << PlayerColorHelper::name(bed) << "'s bed\n";

        for (auto delegate: delegates) {
            delegate->on_bed_broken(*this, breaker, bed);
        }
    }
}
