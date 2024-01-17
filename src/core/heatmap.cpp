// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <cassert>
#include "heatmap.h"

namespace bwmodel {
    HeatMap::HeatMap(Grid<double>& distribution): distribution(distribution) {
        assert_grid_invariant();
    }

    const Grid<double>& HeatMap::backing() const {
        return distribution;
    }

    double HeatMap::average(int x1, int y1, int x2, int y2) {
        assert(x1 < x2);
        assert(y1 < y2);

        double sum = 0;
        int count = 0;
        for (int y = y1; y <= y2; y++) {
            for (int x = x1; x <= x2; x++) {
                if (y >= 0 && y <= height() && x >= 0 && x < width()) {
                    sum += distribution[y][x];
                    count++;
                }
            }
        }
        return sum / count;
    }

    void HeatMap::concentrate_at(std::vector<std::pair<int, int>> points) {
        for (int j = 0; j < height(); j++) {
            for (int i = 0; i < width(); i++) {
                distribution[j][i] = 0;
            }
        }
        const double fill = 1.0 / (double)points.size();
        for (std::pair<int, int>& point: points) {
            distribution[point.second][point.first] = fill;
        }
    }

    void HeatMap::filter(int kernel_radius) {
        Grid<double> result = distribution;
        for (int j = 0; j < height(); j++) {
            for (int i = 0; i < width(); i++) {
                result[j][i] = average(i - kernel_radius, i + kernel_radius,
                    j - kernel_radius, j + kernel_radius);
            }
        }
        distribution = result;
    }

    void HeatMap::merge_with(HeatMap& other) {
        Grid<double> result = distribution;
        double sum = 0;
        for (int j = 0; j < height(); j++) {
            for (int i = 0; i < width(); i++) {
                result[j][i] *= other.distribution[j][i];
                sum += result[j][i];
            }
        }
        for (int j = 0; j < height(); j++) {
            for (int i = 0; i < width(); i++) {
                result[j][i] /= sum;
            }
        }
        distribution = result;
        other.distribution = result;
    }

    std::unique_ptr<HeatMap> HeatMap::uniform(int width, int height) {
        // TODO: fix implementation for numerical stability
        const double fill = 1.0 / (double)(width * height);

        GridRow<double> grid_row(width, fill);
        Grid<double> grid(height, grid_row);
        std::unique_ptr<HeatMap> result(new HeatMap(grid));

        return result;
    }
}
