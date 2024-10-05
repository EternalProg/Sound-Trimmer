# Sound-Trimmer

This is a simple **command-line tool** for trimming audio files and normalizing sound volume. You can use it to remove leading and trailing silence from audio and trim the audio to a specified duration.

## Table of Contents

- [Features](#features)
- [Dependencies](#dependencies)
- [Installation](#installation)
   - [Install Dependencies](#install-dependencies)
   - [Installation Steps](#installation-steps)
   - [Installing to System Path](#installing-to-system-path)
- [Usage](#usage)
- [Example](#example)
- [Removing Sound-Trimmer](#removing-sound-trimmer)

## Features

- **Remove silence** at the beginning and end of the audio.
- **Normalize amplitude** to a specified value.
- **Trim audio** to a specified duration.

## Dependencies

- C++17 or newest version
- [libsndfile](http://www.mega-nerd.com/libsndfile/)

## Installation

To install the project, you will need [CMake](https://cmake.org/) and `libsndfile`. Follow the steps below:

### Install Dependencies

1. Install **CMake**(3.20 require):
    ```sh
    sudo apt-get install cmake
    ```
2. Install `libsndfile`:
    ```sh
    sudo apt-get install libsndfile1-dev
    ```

### Installation Steps

1. Clone the repository:
    ```sh
    git clone https://github.com/EternalProg/Sound-Trimmer
    cd Sound-Trimmer
    ```

2. Create a build directory:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to generate the Makefile:
    ```sh
    cmake ..
    ```

4. Build the project:
    ```sh
    cmake --build .
    ```

After these steps, an executable named `sound-trimmer` will be created in the `build` directory.

### Installing to System Path

To use the program directly from any terminal without specifying `./`, you can move the executable to `/usr/local/bin`:

   ``` shell
   sudo cp build/sound-trimmer /usr/local/bin/sound-trimmer
   ```   

## Usage

It trims the audio file to the specified duration. If the duration is not specified, the program will trim the audio to
1 second.

After compilation, the `sound-trimmer` program can be run with the following parameters:

```sh
./sound-trimmer <input_file> <output_file> [duration_in_seconds]
```

OR if you do **[Installing to System Path](#installing-to-system-path)**

```sh
sound-trimmer <input_file> <output_file> [duration_in_seconds]
```

### Example
To trim an audio file to 0.1 seconds and save it to a new file:

```sh
sound-trimmer input.wav output.wav 0.1
```
This will take the input file `input.wav`, trim it to 0.1 seconds, and save the result to `output.wav`.

## Removing Sound-Trimmer

If you decide to remove Sound-Trimmer from your system, simply delete the executable and directory with **build-files**:

``` shell
sudo rm /usr/local/bin/sound-trimmer
```
