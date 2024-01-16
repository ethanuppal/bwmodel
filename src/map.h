// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <cstdint>
#include <cstdbool>
#include <string>
#include <vector>
#include <memory>
#include <cassert>
#include <stdexcept>

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
        BASE = 1 << 1,
        EMPTY = 1 << 2,
        DIAMOND = 1 << 3,
        MIDDLE = 1 << 4,

        RED = 1 << 8,
        BLUE = 1 << 9,
        GREEN = 1 << 10,
        AQUA = 1 << 11,
        YELLOW = 1 << 12,
        WHITE = 1 << 13,
        BLACK = 1 << 14,
        PINK = 1 << 15,

        INVALID = 0,
        UNINIT = 1
    };

    inline RegionSet operator|(RegionSet lhs, RegionSet rhs) {
        return static_cast<RegionSet>(static_cast<RegionSetBacking>(lhs)
                                      | static_cast<RegionSetBacking>(rhs));
    }

    class MapLoadError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    /** Region information. */
    class Map {
        using MapRow = std::vector<RegionSet>;
        using MapGrid = std::vector<MapRow>;

        /** The internal grid representation of the map in row-major order. */
        MapGrid grid;

        /**
         * Constructs a new map from the given `grid`.
         *
         * @pre `grid` is in row-major order and has at least one row.
         */
        Map(MapGrid& grid);

    public:
        /** Width of the map. */
        blocks_t width() const;

        /** Height of the map. */
        blocks_t height() const;

        /**
         * Adds the regions given by `regions` to the the given (`x`, `y`)
         * coordinates. If the current region is `RegionSet::UNINIT`, it gets
         * replaced.
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
}
