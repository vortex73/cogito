# Cogito

Cogito is a high-performance lexer written in modern C++20, designed as the foundation for a fast and lightweight C99 compiler. This project is currently in early development, with the lexer module as the primary implemented feature.

## Building

### Prerequisites

- CMake
- A C++20-compatible compiler

### Steps

```bash
git clone git@github.com:vortex73/cogito.git
cd cogito
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

This will produce:

- `build/cogito`: the lexer executable
- `build/cogito_tests`: the test suite

## Running the Lexer

```bash
./build/cogito
```

cogito looks into a `sample_input.c` for the source (To be fixed)

## Running Tests

```bash
./build/cogito_tests
```


> *Cogito ergo sum*

