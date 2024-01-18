#include "parseline.h"
#include <regex>
#include "game/player.h"

namespace bwmodel {
    std::optional<Event> parse_line(const std::string& line) {
        if (line.find("<span style=\"color: #000;\">Bed Wars</span>")
            != std::string::npos) {
            return [](Game& game) { game.notify_start(); };
        }

        std::regex color_regex("<span style=\"color: #(.*?);\">.*?<\\/span>",
            std::regex_constants::ECMAScript);
        std::smatch matches;

        std::vector<std::string> colors;

        while (std::regex_search(line, matches, color_regex)) {
            assert(matches.size() == 2);
            colors.push_back(matches[1]);
        }

        if (colors.size() == 7
            && line.find("BED DESTRUCTION") != std::string::npos) {
            auto opt_bed_color = color_from_str(colors[2]);
            auto opt_player_color = color_from_str(colors[5]);

            if (!opt_bed_color.has_value() || !opt_player_color.has_value()) {
                return {};
            }

            return [opt_bed_color, opt_player_color](Game& game) {
                game.notify_break_bed(opt_player_color.value(),
                    opt_bed_color.value());
            };
        }

        if (colors.size() == 4) {
            auto opt_killed = color_from_str(colors[0]);
            auto opt_killer = color_from_str(colors[2]);

            if (!opt_killed.has_value() || !opt_killer.has_value()) {
                return {};
            }

            return [opt_killed, opt_killer](Game& game) {
                game.notify_player_kill(opt_killer.value(), opt_killed.value());
            };
        }

        return {};
    }

    // TODO: incomplete/untested
    std::optional<PlayerColor> color_from_str(const std::string& str) {
        if (str == "555") return PlayerColor::GRAY;
        if (str == "55F") return PlayerColor::BLUE;
        if (str == "5F5") return PlayerColor::GREEN;
        if (str == "5FF") return PlayerColor::AQUA;
        if (str == "F55") return PlayerColor::RED;
        if (str == "F5F") return PlayerColor::PINK;
        if (str == "FF5") return PlayerColor::YELLOW;
        if (str == "FFF") return PlayerColor::WHITE;
        return {};
    }
}