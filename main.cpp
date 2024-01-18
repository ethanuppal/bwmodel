// Copyright (C) 2024 Ethan Uppal. All rights reserved.

#include <iostream>
#include <cassert>
#include <fstream>
#include "bwmodel.h"
#include "efsw/include/efsw/efsw.hpp"
using namespace bwmodel;
namespace fs = std::filesystem;

class FileWatcherListener : public efsw::FileWatchListener {
    Game& game;
    const std::filesystem::path& filename;
    std::ifstream stream;

public:
    FileWatcherListener(Game& game, const fs::path& dir, const fs::path& file)
        : game(game), filename(file) {
        stream.open(dir / file);
        if (stream.fail()) throw std::runtime_error("Failed to open file");

        stream.seekg(0, std::ios::end);
    }

    ~FileWatcherListener() {
        stream.close();
    }

    void handleFileAction(efsw::WatchID watchid, const std::string& dir,
        const std::string& filename, efsw::Action action,
        std::string oldFilename) override {
        if (action != efsw::Action::Modified || filename != this->filename)
            return;

        stream.clear();

        std::string line;
        while (std::getline(stream, line)) {
            auto event = parse_line(line);
            if (event.has_value()) {
                event.value()(game);
            }
        }
    }
};

int main() {
    try {
        // create game for map
        std::unique_ptr<Map> map = Map::load_from("./data/example.bwmap");
        Game game(std::move(map));

        // we want to test our v1 model
        std::shared_ptr<GameDelegate> model = Model::v1::make();
        game.attach(model);

        std::cout << "Chat log directory: ";
        std::string path;
        std::getline(std::cin, path);

        std::cout << "Chat log file name: ";
        std::string name;
        std::getline(std::cin, name);

        efsw::FileWatcher watcher;
        FileWatcherListener listener(game, path, name);

        watcher.addWatch(path, &listener, false);

        watcher.watch();

        std::cout << "Watching for changes...\n";
        std::cout << "Press enter to exit.\n";
        std::cin.get();

    } catch (const MapLoadError& error) {
        std::cerr << "MapLoadError: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
