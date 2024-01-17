// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <cstdint>
#include <cstdbool>
#include <string>
#include <memory>
#include <cassert>
#include <stdexcept>
#include "core/grid.h"
#include "game/player.h"

namespace bwmodel {
    /** A numeric value in the unit of Minecraft blocks, each of which
     * represents a meter. */
    using blocks_t = int32_t;

    /** Region width in blocks. */
    constexpr blocks_t REGION_W = 8;

    /** Region height in blocks. */
    constexpr blocks_t REGION_H = 8;

    using RegionSetBacking = uint16_t;

    /** Defines the properties of a region on the map. */
    enum class RegionSet : RegionSetBacking {
        BASE = 1 << 0,
        EMPTY = 1 << 1,
        DIAMOND = 1 << 2,
        MIDDLE = 1 << 3,

        RED = 1 << (8 + *PlayerColor::RED),
        BLUE = 1 << (8 + *PlayerColor::BLUE),
        GREEN = 1 << (8 + *PlayerColor::GREEN),
        AQUA = 1 << (8 + *PlayerColor::AQUA),
        YELLOW = 1 << (8 + *PlayerColor::YELLOW),
        WHITE = 1 << (8 + *PlayerColor::WHITE),
        BLACK = 1 << (8 + *PlayerColor::BLACK),
        PINK = 1 << (8 + *PlayerColor::PINK),

        INVALID = 0
    };

    inline RegionSet operator|(RegionSet lhs, RegionSet rhs) {
        return static_cast<RegionSet>(static_cast<RegionSetBacking>(lhs)
                                      | static_cast<RegionSetBacking>(rhs));
    }

    inline void operator|=(RegionSet& lhs, RegionSet rhs) {
        lhs = lhs | rhs;
    }

    /** @see Map::load_from */
    class MapLoadError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    /** Defines the regions in a bedwars map. */
    class Map : public GridInterface<RegionSet, blocks_t> {
        /** The internal grid representation of the map in row-major order. */
        std::shared_ptr<Grid<RegionSet>> grid;

        /**
         * Constructs a new map from the given `grid`.
         *
         * @pre `grid` is in row-major order and has at least one row.
         */
        Map(std::shared_ptr<Grid<RegionSet>> grid);

    protected:
        const Grid<RegionSet>& backing() const override;

    public:
        /**
         * Adds the regions given by `regions` to the the given (`x`, `y`)
         * coordinates.
         *
         * @returns Whether the addition was in-range.
         */
        bool update_region(blocks_t x, blocks_t y, RegionSet regions);

        /**
         * Returns the region properties at the given (`x`, `y`) coordinates.
         *
         * @returns `RegionSet::INVALID` if out of range.
         */
        RegionSet regions_at(blocks_t x, blocks_t y) const;

        /**
         * Loads a map from a file, idiomatically with extension `.bwmap`.
         *
         * @throws MapLoadError on parsing error.
         */
        static std::unique_ptr<Map> load_from(const std::string& path);
    };

    namespace RegionSetHelper {
        /** Retrieves the region mask representing the player color `color`. */
        RegionSet from(PlayerColor color);

        /** Retrieves the region mask represented by the character `value`. */
        RegionSet from(char value);
    }
}
