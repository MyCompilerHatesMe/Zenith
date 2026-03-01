# Zenith

[![License](https://img.shields.io/github/license/MyCompilerHatesMe/Zenith?style=for-the-badge)](https://opensource.org/licenses/MIT)
[![Status](https://img.shields.io/badge/status-in%20development-blue.svg?style=for-the-badge)](https://github.com/MyCompilerHatesMe/Zenith)

A simple, statically typed toy programming language built from scratch in C++ for learning and experimentation.

---

## Table of Contents

- [About](#about)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Building from Source](#building-from-source)
- [Usage](#usage)
- [Language Guide](#language-guide)
  - [Types](#types)
  - [Variables](#variables)
  - [Operators](#operators)
  - [Control Flow](#control-flow)
  - [Loops](#loops)
  - [Display](#display)
  - [Comments](#comments)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

---

## About

Zenith is a toy programming language built from scratch — lexer, parser, and tree-walking interpreter all written in C++. The goal is to explore language design, parsing, and interpretation while keeping the syntax clean and easy to read.

---

## Getting Started

### Prerequisites

A modern C++ compiler and CMake.

```sh
sudo apt-get install build-essential cmake
```

### Building from Source

```sh
git clone https://github.com/MyCompilerHatesMe/Zenith.git
cd Zenith
cmake -S . -B build
cmake --build build
```

The `zenith` executable will be in `build/bin/`.

---

## Usage

Run any `.zen` file:

```sh
./zenith program.zen
```

---

## Language Guide

### Types

Zenith is statically typed. Every variable must be declared with an explicit type.

| Keyword  | Description         |
|----------|---------------------|
| `int`    | Integer number      |
| `string` | String of text      |
| `bool`   | `true` or `false`   |
| `char`   | Single character    |

### Variables

Variables are declared with a type keyword, a name, and an initializer. Redeclaring a variable in the same scope is a runtime error.

```js
int x = 10;
string name = "zenith";
bool flag = true;
```

Assignment to an existing variable:

```js
x = 20;
```

### Operators

**Arithmetic** — integers only, except `+` which also concatenates strings.

```js
int a = 10 + 3;   // 13
int b = 10 - 3;   // 7
int c = 10 * 3;   // 30
int d = 10 / 3;   // 3  (integer division)

string s = "hello, " + "world";  // "hello, world"
```

**Comparison**

```js
x > y
x >= y
x < y
x <= y
x == y
x != y
```

**Logical**

```js
flag and true
flag or false
!flag
```

### Control Flow

Braces are required for all branches.

```js
if (x > 5) {
    display("big");
} else if (x == 5) {
    display("five");
} else {
    display("small");
}
```

### Loops

**while**

```js
int i = 0;
while (i < 5) {
    display(i);
    i = i + 1;
}
```

**for** — uses existing variables for init and increment.

```js
int i = 0;
for (i = 0; i < 5; i = i + 1) {
    display(i);
}
```

### Blocks and Scope

Blocks create a new scope. Variables declared inside a block are not accessible outside it.

```js
int x = 1;
{
    int y = 2;
    display(x);  // 1
    display(y);  // 2
}
display(x);  // 1
// display(y); -- error: undefined variable
```

### Display

`display` prints a value followed by a newline.

```js
display("hello, world");
display(42);
display(true);
```

### Comments

Single-line comments start with `//`.

```js
// this is a comment
```

---

## Roadmap

- [x] Lexer
- [x] Parser
- [x] Tree-walking interpreter
- [x] Static types (`int`, `string`, `bool`, `char`)
- [x] Variable declarations and assignment
- [x] Arithmetic and string concatenation
- [x] Comparison and logical operators
- [x] `if` / `else if` / `else`
- [x] `while` and `for` loops
- [x] Block scoping
- [ ] Functions (`fun`)
- [ ] Arrays and hashmaps
- [ ] Standard library

---

## Contributing

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## License

Distributed under the MIT License. See `LICENSE.md` for more information.

Copyright (c) 2025 AJ. & PranavNihal

---

## Acknowledgements

- [Crafting Interpreters](https://craftinginterpreters.com/) by Robert Nystrom
- Anyone whose code was an inspiration!