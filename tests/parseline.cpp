#include "bwmodel.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace bwmodel;

int main() {
    std::unique_ptr<Map> map = Map::load_from("./data/example.bwmap");
    Game game(std::move(map));

    // TODO: add game start test
    std::ifstream file("./data/example_chat.htm");

    if (file.fail()) {
        std::cout << "Failed to open file\n";
        file.close();
        return 1;
    }

    std::string line;

    while (std::getline(file, line)) {
        auto event = parse_line(line);
        if (event.has_value()) {
            event.value()(game);
        }
    }

    file.close();

    assert(!game.player_has_bed(PlayerColor::RED));
    assert(game.players_left().size() == 7);
    assert(game.players_left().find(PlayerColor::RED)
           == game.players_left().end());

    return 0;
}