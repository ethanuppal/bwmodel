#include <optional>
#include <functional>
#include "game/game.h"

namespace bwmodel {
    using Event = std::function<void(Game&)>;

    struct ColorSection {
        std::string color;
        std::string text;
    };

    std::optional<Event> parse_line(const std::string& line);
}