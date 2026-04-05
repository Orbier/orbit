# Orbit

Holy Gd refrence
---

Hello there! Today i present you one of my useful programming stuff, Orbit.
Always had a pain about "omg idek this cmake hell" ? Well, fear not! 

I made this as an 'own personal use thingy' but oh well. I just had to github.

---

# Features

- Simple wrapper around CMake for easy project building
- Multiple build commands: `config`, `make`, `build`
- Ninja generator support for fast builds
- Multiple compiler support: GCC, Clang, Clang-CL, MSVC
- Unity build mode for faster compilation

---

# How to use this? 

Preety simple, actually

## Installation

To use this,
- place in a direcotry
- add that directory into SYSTEM PATH!
- viola! use it universally!

## Commands

- `config` Confugures the project (so you could have your buildfiles)
- `make`  builds the thing
- `build` Config+make

## Options/features

- `ninja` use it like this `--ninja`
- **Specific compiler** - Yes you can, By default its clang. Refer to Compiler Options for this one
  Use it like - `--compiler-id=<id>` OR `--use-compiler=<name>`
- Unity - use this `--unity`

## Compiler Options

Compiler ID And their names are as follows - 

- ID 1 - `gcc`
- ID 2 - `clang` Default
- ID 3 - `clang-cl`
- ID 4 - `msvc`
> Note - For some reason whenever i try to `--use-compiler=msvc` it just quits on me bro
> you can but for MSVC its recommended (by me) that you use Compiler ID. 
> Thank you

## Examples

```bash
./ORBIT build
./ORBIT config --ninja
./ORBIT make --use-compiler=gcc
./ORBIT build --compiler-id=4 --unity
```

---

 Please join my discord server! https://discord.gg/kx8km2V7kz
