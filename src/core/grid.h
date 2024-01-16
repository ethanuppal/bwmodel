// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <vector>

namespace bwmodel {
    /** A grid row. */
    template<typename T>
    using GridRow = std::vector<T>;

    template<typename T>
    using Grid = std::vector<GridRow<T>>;

    /** A generic grid with width and height. */
    template<typename T, typename Index>
    class GridInterface {
    protected:
        /** Internally retrieves the backing (nested vector) for helper function
         * generation in `GridInterface`. */
        virtual const Grid<T>& backing() const = 0;

        /** Ensures that all rows in the grid have the same length. */
        void assert_grid_invariant() {
            for (const GridRow<T>& row: backing()) {
                assert(row.size() == backing()[0].size());
            }
        }

    public:
        Index width() const {
            return backing()[0].size();
        }

        Index height() const {
            return backing().size();
        }
    };
}
