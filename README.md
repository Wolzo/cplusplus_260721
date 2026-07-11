# C++ Directed Graph + Qt RGB Analyzer

University project for the C++ Programming course (8 CFU) at the University of Milano-Bicocca. It contains two independent parts: a generic directed graph class in standard C++, and a Qt desktop app that analyzes the RGB composition of an image.

## 1. Directed Graph (`graph<T, E>`)

A generic directed graph implemented with a boolean **adjacency matrix**, no STL containers. Nodes are identified by a generic type `T`; a second template parameter `E` is an equality functor that decides when two identifiers refer to the same node (this lets node identity be decoupled from `T::operator==`).

Highlights:

- Adjacency matrix stored as a single linearized `bool*` block (one allocation, row-major offset `i * N + j`).
- Add / remove nodes and edges, with custom exceptions for the error cases (`node_not_found`, `node_already_exists`, `edge_already_exists`, `edge_not_found`).
- Forward `const_iterator` over the node identifiers.
- Rule of three with copy-and-swap, strong exception safety on structural changes.
- Written in C++98 style; the only C++11 construct used is `nullptr`.

### Build and run

```bash
make          # builds main.exe (the test suite)
./main.exe    # runs the automated tests
make doc      # generates HTML docs with Doxygen
make clean    # removes build artifacts
```

The `main.cpp` file is a suite of automated tests covering the whole public interface, run on `int` and on two custom types. Memory has been checked with Valgrind (no leaks, no errors).

## 2. Qt RGB Analyzer (`Qt/`)

A Qt Widgets app that loads an RGB image and shows two charts:

- A pie chart of the distinct RGB triplets, each slice colored with its real color and sized by its normalized frequency. Clicking a slice highlights it and shows its RGB values and percentage.
- A line histogram of per-channel intensities (R, G, B) over the full 0–255 range, normalized by the total pixel count.

Triplets are stored deduplicated in reading order using a `QList` of records plus a `QHash` lookup; per-channel counts use three fixed-size `QList<int>` of 256 elements. The layout is adaptive to the window size.

### Build and run

Open `Qt/RgbAnalyzer.pro` in Qt Creator (built with **Qt 6.8.1**, MinGW 64-bit kit) and run, or from the command line:

```bash
cd Qt
qmake
make
```

## Repository layout

```
graph.hpp                 # the graph class (header-only)
custom_types.hpp          # custom types used in the tests
*_not_found.hpp / ...     # custom exception classes
main.cpp                  # automated test suite
Makefile                  # command-line build
Doxyfile                  # Doxygen config (HTML output)
Qt/                       # Qt RGB analyzer sources
```

## Notes

This repo is a personal copy of the project. The build targets and constraints (executable named `main.exe`, no external libraries in the graph class, C++98 except `nullptr`) follow the course requirements.
