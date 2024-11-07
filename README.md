# ascii-love

A collection of ASCII art animations rendered in the terminal, including spinning donuts and hearts.

## Features

- **Spinning Donut**: Renders a rotating three-dimensional donut using ASCII characters.
- **Spinning Heart**: Displays a heart shape that rotates, creating a mesmerizing animation.

## Requirements

- **Operating System**: Linux
- **Compiler**: GCC
- **Libraries**: `math.h`, `stdio.h`

## Compilation

To compile the programs, use the following commands:

```sh
gcc -o donut tests/donut.c -lm
gcc -o heart heart.c -lm
```

## Usage

Run the executables in the terminal:

```sh
./donut
./heart
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
