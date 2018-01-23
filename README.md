chip-seven
==========

A simple CHIP-8 interpreter for Linux written in C. Uses OpenGL ([GLFW](https://github.com/glfw/glfw) and [glad](https://github.com/Dav1dde/glad)) to render the display.

### Dependencies
#### GLFW
Follow [this](http://www.glfw.org/docs/latest/compile_guide.html) guide, or my tl;dr version:

Make sure `libglfw3`, `xorg-dev` and `cmake` is installed, then:
```
git clone git@github.com:glfw/glfw.git
cd glfw
cmake -DBUILD_SHARED_LIBS=ON .
make
sudo make install
```

#### glad
To install:
```
pip install glad
```

In the root directory, run:
```
python -m glad --generator c --no-loader --local-files --out-path lib/glad
```

### Build
In the root directory, run:
```
make
```

That's it, chip-seven should now appear in the root directory.

### Run
```
./chip-seven <path-to-program>
```

Keys are mapped to 1-4 down to Z-V.

### Get some games to run
- http://www.pong-story.com/chip8/
- https://github.com/badlogic/chip8/tree/master/roms
