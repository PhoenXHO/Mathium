# Mathium Programming Language

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Version](https://img.shields.io/badge/version-0.2.0--alpha.1-orange.svg)](https://github.com/PhoenXHO/Mathium)
[![Status](https://img.shields.io/badge/status-active%20development-brightgreen.svg)](https://github.com/PhoenXHO/Mathium)

> A programming language designed for mathematical computations with precise arithmetic and symbolic expressions.

Mathium is a custom programming language that combines mathematical notation with modern programming language features. Built with arbitrary-precision arithmetic (GMP/MPFR), it's designed for applications requiring exact numerical computations.

**Current Status:** Complete rewrite from prototype. Core interpreter functional with variables, functions, and basic expressions. Undergoing systematic refactoring and documentation before feature expansion.

---

## 🔗 Quick Links

- **[Documentation](docs/)** - Language specification and design documents
- **[Devlog Series](#)** - Follow the development journey *(coming soon)*
- **[Contributing](CONTRIBUTING.md)** - How to contribute *(coming soon)*

---

## Table of Contents
- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Project Status](#project-status)
- [Development](#development)
- [License](#license)

---

## Features

### ✅ Currently Implemented
- **Arbitrary-precision arithmetic** using GMP (integers) and MPFR (reals)
- **Interactive REPL** with multi-line input support
- **Variable declarations** with type inference and explicit typing
  ```mathium
  let x := 42              // Type inferred as Integer
  let y: Real := 3.14159   // Explicit type annotation
  ```
- **Function calls** (builtin functions)
- **Expression evaluation** with proper operator precedence
- **Error handling** with source location tracking

### 🚧 In Progress (v0.2.0-alpha.1)
- File-based script execution (`.mthx` files)
- Additional builtin operators (`-`, `*`, `/`, `^`, etc.)
- User-defined functions
- Classes and object-oriented features
- Comprehensive builtin function library

### 🔮 Planned Features
- **Rich type system** with inference, subtyping, and qualifiers (`const`, `ref`)
- **Object model** - Everything is an object, classes are objects
- **Operator overloading** for user-defined types
- **Control flow** - `if`, `while`, `for`, `match` statements
- **Pattern matching** and destructuring
- **Symbolic expressions** and computer algebra features
- **Module system** for code organization

---

## Getting Started

### Prerequisites
- **CMake** 3.29.7 or higher
- **C++20 compatible compiler**
- **GMP** - GNU Multiple Precision Arithmetic Library
- **MPFR** - Multiple Precision Floating-Point Reliable Library
- **Boost** 1.74.0 or higher

### Installation

#### Linux (Ubuntu/Debian)
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential cmake libboost-all-dev libgmp-dev libmpfr-dev

# Clone and build
git clone https://github.com/PhoenXHO/Mathium.git
cd Mathium
mkdir build && cd build
cmake ..
make
```

#### Linux (Fedora)
```bash
sudo dnf install cmake boost-devel gmp-devel mpfr-devel gcc-c++
# Then follow clone and build steps above
```

#### macOS
```bash
# Install dependencies via Homebrew
brew install cmake boost gmp mpfr

# Clone and build
git clone https://github.com/PhoenXHO/Mathium.git
cd Mathium
mkdir build && cd build
cmake ..
make
```

#### Windows
1. Install [CMake](https://cmake.org/download/)
2. Install dependencies via [vcpkg](https://vcpkg.io/):
   ```cmd
   vcpkg install boost gmp mpfr
   ```
3. Clone and build:
   ```cmd
   git clone https://github.com/PhoenXHO/Mathium.git
   cd Mathium
   mkdir build && cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
   cmake --build .
   ```

---

## Usage

### Interactive REPL
Start the Mathium REPL:
```bash
./Mathium
```

Example session:
```mathium
Mathium> let x := 42
Mathium> let y := 3.14159
Mathium> x + y
45.14159
Mathium> print(add(x, 8))
50
```

### REPL Commands
- **`quit`** or **`exit`** - Exit the interpreter
- **`\`** at end of line - Continue input on next line
- Press **Enter** on empty line to cancel multi-line input

### Command Line Options
```bash
Usage: Mathium [<options>]
Options:
  -h [ --help ]         Display this help message
  -v [ --version ]      Display interpreter version and additional information
  -V [ --verbose ]      Enable verbose output
  -P [ --print-all ]    Print all expressions that do not have a semicolon at 
                        the end (if this option is not enabled, only the last 
                        expression will be printed)
  -b [ --benchmark ]    Print the time taken to execute the program at the end 
                        of execution (in microseconds)
  -W [ --no-warn ]      Disable warnings
```

<!-- ### Example Script — hidden until file execution (.mthx) is implemented.

Create a file `hello.mthx`:
```mathium
let greet := \name -> "Hello, " ++ name ++ "!"
greet("World")
```

Run it:
```bash
./Mathium -f hello.mthx
```
-->

## Development

### Project Structure
```
src/
├── frontend/        # Lexer, parser, AST
├── semantic/        # Type checking, symbol resolution
├── compiler/        # Bytecode generation
├── runtime/         # VM, object model, builtins
└── util/            # Error handling, config, debugging
```

See [`docs/`](docs/) for detailed architecture documentation.

### Contributing

Contributions are welcome! This project is currently undergoing major refactoring. If you'd like to contribute:

1. **Check the roadmap** *(coming soon in `docs/contributing/`)*
2. **Open an issue** to discuss your proposed changes
3. **Follow the style guide** *(coming soon in `docs/contributing/`)*
4. **Write tests** for new features

---

## Version History
- **0.2.0-alpha.1** (In Progress) - Complete rewrite with improved architecture
- **0.1.x** - Initial prototype (deprecated)

## License

This project is licensed under the **GNU General Public License v3.0** - see the [LICENSE](LICENSE) file for details.

---

## Acknowledgments

- **GMP** - Arbitrary precision integer arithmetic
- **MPFR** - Arbitrary precision floating-point arithmetic
- **Boost** - C++ utility libraries

---

## Contact & Follow

- **GitHub:** [@PhoenXHO](https://github.com/PhoenXHO)
- **Medium:** Follow for devlogs *(coming soon)*
- **LinkedIn:** Connect for updates *(coming soon)*

---

*Last Updated: September 19, 2026*