// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <memory>
#include <vector>
#include <utility>
#include "grid.h"
#include "game/player.h"
#include "game/map.h"

namespace bwmodel {
    /** A two-dimensional probability distribution. */
    class HeatMap : public GridInterface<double, int> {
        Grid<double> distribution;

        /** @pre Each row in `distribution` must have the same length. */
        HeatMap(Grid<double>& distribution);

        /**
         * @returns The average of all points within the rectangle from (`x1`,
         * `y1`) to (`x2`, `y2`).
         *
         * @pre `x1 < x2` and `y1 < y2`.
         */
        double average(int x1, int y1, int x2, int y2);

    protected:
        const Grid<double>& backing() const override;

    public:
        /** Concentrates all of the distribution at the given points. */
        void concentrate_at(std::vector<std::pair<int, int>> points);

        /** Assigns each point in the heat map to the average of its neighbors
         * within a square of size `kernel_radius`. */
        void filter(int kernel_radius);

        /** TODO: document. basically this is when you have one player kill
         * another so you can know they are in the same location but you don't
         * know where so you just adjust both players location heat maps to
         * reflect this. the other player obviously just gets put back at their
         * base but you get the idea. */
        void merge_with(HeatMap& other);

        /** Creates a uniformly-distributed heat map with dimensions `width` and
         * `height`. */
        static std::unique_ptr<HeatMap> uniform(int width, int height);
    };
}
