# bwmodel: A modeling environment for Minecraft Bedwars

[![CodeFactor](https://www.codefactor.io/repository/github/ethanuppal/bwmodel/badge)](https://www.codefactor.io/repository/github/ethanuppal/bwmodel)
![CI Badge](https://github.com/ethanuppal/bwmodel/actions/workflows/ci.yaml/badge.svg)


This project is an API for creating predictive models for Minecraft Bedwars—as well as an implementation of such models—using only information that would be available to a player.
The idea is that a computer can process much more than a human brain can.
For example, when one player kills another, it is likely that it was in hand-to-hand combat.
If a probability distribution of player locations is maintained, we can update them to reflect this new information.

## Usage

You can use this run through the `Makefile`. You must have C++ 17.
.
```bash
make        # builds the ./main script
make run    # builds and runs the ./main script
make clean  # removes auxillary and executable files
make test   # runs the tests
```

`make run` will run a testing script for the latest model.
You can see all models in [`src/models/`](./src/models/).
Developing a new model is as simple as implementing the [`GameDelegate`](./src/game/gamedelegate.h) interface.
