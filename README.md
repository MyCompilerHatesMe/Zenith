# Zenith

[![License](https://img.shields.io/github/license/MyCompilerHatesMe/Zenith?style=for-the-badge)](https://opensource.org/licenses/MIT)
[![Status](https://img.shields.io/badge/status-in%20development-blue.svg?style=for-the-badge)](https://github.com/MyCompilerHatesMe/Zenith)

A modern, simple, and fun toy programming language built for learning and experimentation.

---

## Table of Contents

- [About The Project](#about-the-project)
- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building from Source](#building-from-source)
- [Usage](#usage)
  - [A Taste of Zenith](#a-taste-of-zenith)
- [Language Guide](#language-guide)
  - [Variables](#variables)
  - [Functions](#functions)
  - [Control Flow](#control-flow)
  - [Comments](#comments)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

---

## About The Project

Zenith is a toy programming language created from scratch. The primary goal of this project is to explore the world of language design, parsing, and interpretation/compilation. It aims to have a clean and intuitive syntax, making it easy for anyone to pick up and write simple programs.

Whether you're a student, a developer curious about compilers, or just looking for a fun project, Zenith provides a great starting point.

---

## Features

While still in early development, Zenith aims to support:

- **Simple & Clean Syntax:** Inspired by modern languages to be readable and easy to write.
- **Statically Typed:** Catch errors at compile-time for more robust code.
- **Cross-Platform:** Written in portable C/C++, it can be compiled on Windows, macOS, and Linux.
- **Interpreter-based:** For rapid development and easy debugging.
- **Extensible:** Designed to be easily extendable with new features and data types.

---

## Getting Started

Follow these instructions to get a copy of Zenith up and running on your local machine.

### Prerequisites

The plan is to build Zenith using C++. To contribute to the core language, you will eventually need a modern C++ compiler and `make`.

- **GCC/G++**

    ```sh
    sudo apt-get install build-essential
    ```

### Building from Source

This project uses CMake to generate build files.

1. **Clone the repository:**

    ```sh
    git clone https://github.com/MyCompilerHatesMe/Zenith.git
    cd Zenith
    ```

2. **Configure and build with CMake:**

    ```sh
    cmake -S . -B build
    cmake --build build
    ```

3. The `zenith` executable will be located in the `build/bin/` directory.

---

## Usage

Once the interpreter is ready, you will be able to run Zenith programs from source files (e.g., with a `.zen` extension).

### A Taste of Zenith

Here is what "Hello, World!" might look like in Zenith. *Note: This syntax is provisional and may change as development progresses.*

```cpp
// hello.zen
print("Hello, World!");
```

---

## Language Guide

This section outlines the proposed syntax and features for Zenith.

### Variables

Variables will be declared with the `let` keyword.

```js
let message = "This is a string.";
let count = 100;

```

### Functions

Functions will be first-class citizens, declared with the `fn` keyword.

```js
fn add(a, b) {
    return a + b;
}

let result = add(5, 10);
print(result); // Outputs: 15
```

### Control Flow

Standard `if`/`else` blocks will be used for conditional logic.

```js
let number = 10;
if (number > 5) {
    print("Number is greater than 5.");
} else {
    print("Number is not greater than 5.");
}
```

### Comments

Single-line comments start with `//`.

```js
// This is a comment and will be ignored by the interpreter.
```

---

## Roadmap

- [ ] Basic arithmetic operations
- [ ] Support for loops (`for`, `while`)
- [ ] More complex data types (arrays, hashmaps)
- [ ] Standard library for common tasks
- [ ] A JIT or AOT compiler

See the open issues for a full list of proposed features (and known bugs).

---

## Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## License

Distributed under the MIT License. See `LICENSE.md` for more information.

Copyright (c) 2025 AJ. & PranavNihal

---

## Acknowledgements

- Crafting Interpreters
- Awesome Readme Templates
- Anyone whose code was an inspiration!
