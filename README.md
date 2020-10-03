# HeatSimulationGPU
A 2D heat diffusion simulation that runs on the GPU (as opposed to the CPU).

## Method
The world is broken into a grid of discrete cells, with the temperature value of each cell representing the temperature at the center of the cell. This data is stored in a buffer on the graphics card (an SSBO).
If one wanted to write this program entirely on the CPU, they would loop over each cell, calculating the gradient at each cell in sequence.
This program dispatches one instance of the computer shader for each cell, computing the gradients in parallel.

## Controls
Add heat around the mouse with left click.  
Remove heat around the mouse with right click.  
Increase/decrease the radius of heat addition/removal by scrolling up/down.

## Dependencies
[GLFW](https://www.glfw.org/) for windowing and handling input.  
[GLAD](https://glad.dav1d.de/) for loading OpenGL function pointers.  
[GLM](https://glm.g-truc.net/0.9.9/index.html) for OpenGL vector and matrix operations.

## Images
