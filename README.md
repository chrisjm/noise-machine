# Noise Machine

This is a simple C++ project that generates white noise using the [miniaudio](https://github.com/mackron/miniaudio) library and the C++ standard random engine `std::minstd_rand`.

## Features

- Real-time white noise generation
- Uses miniaudio for cross-platform audio playback
- Simple, single-file implementation

## Requirements

- C++17 or newer compiler
- [miniaudio.h](https://github.com/mackron/miniaudio) (included in `include/`)

## Build Instructions

You can build the project using `g++`:

```sh
g++ -std=c++17 -Iinclude src/main.cpp -o noise-machine
```

Or use the provided `Makefile`:

```sh
make
```

## Run

```sh
./noise-machine
```

Press ENTER to quit the program.

## License

This project is provided as-is for educational purposes.
