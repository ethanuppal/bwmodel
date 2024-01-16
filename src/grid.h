// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#pragma once

#include <vector>

namespace bwmodel {
    template<typename T>
    using GridRow = std::vector<T>;

    template<typename T>
    using Grid = std::vector<GridRow<T>>;

    template<typename T, typename Index>
    class GridInterface {
    protected:
        virtual const Grid<T>& backing() const = 0;

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
