#include "parseline.h"
#include <regex>
#include <algorithm>
#include "game/player.h"

namespace bwmodel {
    // TODO: incomplete/untested
    std::optional<PlayerColor> color_from_str(const std::string& str) {
        if (str == "000") return PlayerColor::BLACK;
        if (str == "55F") return PlayerColor::BLUE;
        if (str == "5F5") return PlayerColor::GREEN;
        if (str == "5FF") return PlayerColor::AQUA;
        if (str == "F55") return PlayerColor::RED;
        if (str == "F5F") return PlayerColor::PINK;
        if (str == "FF5") return PlayerColor::YELLOW;
        if (str == "AAA") return PlayerColor::WHITE;
        return {};
    }

    std::vector<ColorSection> parse_color_sections(const std::string& line) {
        std::regex color_regex("<span style=\"color: #(.*?);\">(.*?)<\\/span>",
            std::regex_constants::ECMAScript);
        std::smatch matches;

        std::vector<ColorSection> sections;

        // StackOverflow Answer: https://stackoverflow.com/a/35026140
        std::string::const_iterator search_start(line.cbegin());
        while (std::regex_search(search_start, line.cend(), matches,
            color_regex)) {
            assert(matches.size() == 3);
            sections.push_back({matches[1], matches[2]});
            search_start = matches.suffix().first;
        }

        return sections;
    }

    bool is_white_space(const std::string& str) {
        return std::all_of(str.cbegin(), str.cend(),
            [](char c) { return std::isspace(c); });
    }

    std::optional<Event> parse_line(const std::string& line) {
        auto sections = parse_color_sections(line);

        if (sections.size() == 2 && sections[0].color == "000"
            && is_white_space(sections[0].text) && sections[1].color == "000"
            && sections[1].text == "Bed Wars") {
            return [](Game& game) { game.notify_start(); };
        }

        if (sections.size() == 7 && sections[0].color == "000"
            && sections[0].text.find("BED DESTRUCTION") != std::string::npos) {
            auto opt_bed_color = color_from_str(sections[2].color);
            auto opt_player_color = color_from_str(sections[5].color);

            if (!opt_bed_color.has_value() || !opt_player_color.has_value()) {
                return {};
            }

            return [opt_bed_color, opt_player_color](Game& game) {
                game.notify_break_bed(opt_player_color.value(),
                    opt_bed_color.value());
            };
        }

        if (sections.size() == 4
            || (sections.size() == 5 && sections[4].text == "FINAL KILL!")) {
            auto opt_killed = color_from_str(sections[0].color);
            auto opt_killer = color_from_str(sections[2].color);

            if (!opt_killed.has_value() || !opt_killer.has_value()) {
                return {};
            }

            return [opt_killed, opt_killer](Game& game) {
                game.notify_player_kill(opt_killer.value(), opt_killed.value());
            };
        }

        return {};
    }
}