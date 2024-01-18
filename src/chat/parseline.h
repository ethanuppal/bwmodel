#include <optional>
#include <functional>
#include "game/game.h"

namespace bwmodel {
    using Event = std::function<void(Game&)>;

    std::optional<Event> parse_line(const std::string& line);
}