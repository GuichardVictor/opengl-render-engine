# `OpenGL Render Engine`

This is an attempt of 3d rendering engine.
Many features are implemented and the engine is quite easy to use.
As a project / example using this engine I've built a procedural moon generator (using compute shaders).
The example is available under: `src/scenes/custom_scenes/moon/`

## Example: Planet Generator

Generates planets / moon like objects using simplex noise
and compute shaders.

All settings can be interacted using ImGUI.

## Authors

* Victor Guichard

## Results

### Noise

![Alt text](images/noise.png?raw=true "Noise Layers")

### Craters

![Alt text](images/craters.png?raw=true "Craters Layers")

### Without Textures

![Alt text](images/missing_texture.png?raw=true "Noise and Crater Layers")

### Complete Generation

![Alt text](images/final.png?raw=true "Generated Moon Example")

## Settings / Features

* Basic noise settings can be changed such as lacunarity, number of layers, weight, frequency.
* Number of craters, craters position, craters dimension repartition and craters smoothness can be changed.
* Textures are applied using triplanar mapping
