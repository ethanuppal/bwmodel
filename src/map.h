// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <cstdint>
#include <cstdbool>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

namespace bwmodel {
    /** A numeric value in the unit of Minecraft blocks, each of which
     * represents a meter. */
    using blocks_t = int32_t;

    /** Region width in blocks. */
    constexpr blocks_t REGION_W = 8;

    /** Region height in blocks. */
    constexpr blocks_t REGION_H = 8;

    using RegionSetBacking = uint8_t;

    /** Defines the properties of a region on the map. */
    enum class RegionSet : RegionSetBacking {
        BASE = 1 << 0,
        EMPTY = 1 << 1,
        DIAMOND = 1 << 2,
        MIDDLE = 1 << 3,
        INVALID = 0
    };

    class MapLoadError : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    /** Region information. */
    class Map {
        using MapRow = std::vector<RegionSet>;
        using MapGrid = std::vector<MapRow>;

        /** The internal grid representation of the map. */
        MapGrid grid;

        /**
         * Constructs a new map from the given `grid`.
         *
         * @pre `grid` has at least one row.
         */
        Map(MapGrid grid);

    public:
        /** Width of the map. */
        blocks_t width() const;

        /** Height of the map. */
        blocks_t height() const;

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
}
