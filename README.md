# POGLA

## Planet Generator

Generates planets / moon like objects using simplex noise
and compute shaders.

All settings can be interacted using ImGUI.

## Authors

* Victor Guichard
* Guillaume Valente

## Compile

Using cmake to compile.

### Windows

```
cmake --config Release --build "buildDir"
```

### Unix

```
mkdir build
cd build
cmake ..
make
```

## Usage

Run from working folder otherwise assets and shaders won't be found.

### Windows

```
./[buildDir]/moon_generator.exe
```

### UNIX

```
./moon_generator
```