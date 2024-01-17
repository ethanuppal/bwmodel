# bwmodel: A model for Minecraft Bedwars

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
