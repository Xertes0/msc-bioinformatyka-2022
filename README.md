# MSC Bioinformatyka 2022

## Table of Contents
 - [About](#about)
 - [Application features](#application-features)
 - [GitHub pages](#github-pages)
 - [Building from source](#building-from-source)
 - [Building the documentation](#building-the-documentation)
 - [Running the tests](#running-the-tests)

## About
This application is made for Motorola's science cup.

## Application features
 - Translates DNA/RNA sequences to protein sequences.
 - Highlights and analyzes peptides.
 - Draws a skeletal formula of a peptide.
 - Displays various information about peptides.

## GitHub pages
This project is publicly accessible on GitHub pages [under this url](https://Xertes0.github.io/msc-bioinformatyka-2022).

## Building from source

### Prerequisites
These are the dependencies required to build this project.

#### Shared dependencies
These are common dependencies required to build the project.
 - `git`
 - `CMake`
 - `Ninja` or `make`
 - [`Conan`](https://conan.io/) can be installed using `pip`

#### Web dependencies
These are the dependencies required to build and deploy this project on a web server.
 - [`Latest emsdk`](https://github.com/emscripten-core/emsdk)
 - [`Yarn`](https://yarnpkg.com/)

#### Testing dependencies
These are the dependencies required only to build the c++ library for unit testing.
 - C++ compiler (only one)
   - `g++` >= 12
   - `clang++` >= 16
 - `ctest` (not required but recommended)

#### Optional dependencies
These dependencies are optional.
 - `Doxygen` For [building the documentation](#building-the-documentation) as html and man pages

### Building the web application

Build the c++ library using emscripten.
```bash
# Source the `emsdk`.
source <emsdk_installation_path>/emsdk_env.sh

# Clone and cd into the repository
git clone https://www.github.com/Xertes0/msc-bioinformatyka-2022 && cd msc-bioinformatyka-2022

# Create the build directory and configure CMake
mkdir build-emsdk && cd build-emsdk
emcmake cmake -DCMAKE_BUILD_TYPE:STRING="Release" -DDOXYGEN_ENABLED:BOOL=OFF -DTESTS_ENABLED:BOOL=OFF ..

# Build the project
cmake --build . --parallel $(($(nproc)+1))
```

Build the web application and generate static HTML pages.
```bash
# cd into web directory.
cd <project_dir>/web

# Install dependencies using `yarn`
yarn

# Build static HTML pages.
yarn build
```

Static html pages will be put in `<project_dir>/web/dist` which can be now served on a web server.
In case you want to run this application on your own web server
remember to copy the `dist` onto the webserver and
rename it to `msc-bioinformatyka-2022` so the url looks like this: `<address>/msc-bioinformatyka-2022/index.html`.
If you want to change this, modify the vite configuration file `<project_dir>/web/vite.config.ts`.

### Building for unit tests

Build the c++ library and test suite for your system.
```bash
# Clone and cd into the repository
git clone https://www.github.com/Xertes0/msc-bioinformatyka-2022 && cd msc-bioinformatyka-2022

# Create the build directory and configure CMake
mkdir build-native && cd build-native
emcmake cmake -DCMAKE_BUILD_TYPE:STRING="Release" -DDOXYGEN_ENABLED:BOOL=OFF -DTESTS_ENABLED:BOOL=ON ..

# Build the project
cmake --build . --parallel $(($(nproc)+1))
```
[How to run the tests](#running-the-tests)

### Building the documentation

You can optionally build the documentation as html and man pages with `Doxygen`.
For this use any of the two previous build methods
but change the `-DDOXYGEN_ENABLED:BOOL=OFF` variable to `ON` when configuring `CMake`.
Then run:
```bash
# Build the doxygen documentation
cmake --build . --parallel $(($(nproc)+1)) --target docs
```
Documentation will be put in `<build_dir>/docs/html` for static html pages and `<build_dir>/docs/man` for man pages.

## Running the tests

After [building the project for unit tests](#building-for-unit-tests) run these commands.
```bash
# cd into the build directory
cd <project_dir>/build-native

# Run the tests with ctest (recommended)
ctest

# Or run the tests through the catch2 binary
./tests/tests
```