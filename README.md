# Descent

[![LoC](https://tokei.rs/b1/github/m-cat/descent)](https://github.com/m-cat/descent)

![Descent](http://i.imgur.com/i5so9L3.png)

## What is Descent?

Descent is a work-in-progress [roguelike](http://en.wikipedia.org/wiki/Roguelike) game. It will differ from most convential roguelikes by foregoing archaic and dated gameplay and instead offering an intuitive and user-friendly experience.

## Building

Building Descent consists of simply installing libtcod (version 1.6.3 was used for this project) and running `make`.

### Mac

Download libtcod using Homebrew:

```
brew install libtcod
```

Build and run the project:

```
make
```

### Linux/Windows

To get libtcod, follow the instructions [here](https://bitbucket.org/libtcod/libtcod).

Build and run the project:

```
make
```

## Current State (2-27-2017)

This project is likely discontinued. I intend to start a new game from scratch, using a different language, more organized code, and fewer initial features/scope.

### To Do

#### General

- Equipment screen

- Enemy AI

- Have walls drop gems with low chance when mined.

- Add dungeon generation

- Add river & lake generation (using heightmaps)

- Continue work on the menu and UI

#### Misc

- Implement picking up specific items from the ground.

## Credits

All code in "src" folder by Marcin Swieczkowki, except where noted.

Notable exception: priority.* from Rosetta Code.
