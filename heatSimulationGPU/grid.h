#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include "shader.h"

class Grid {
private:
	unsigned int cellVAO;
	unsigned int cellPositionsVBO, cellTemperaturesVBO;
	unsigned int cellTemperaturesSSBO;

	unsigned int xDim;
	unsigned int yDim;

	ComputeShader computeShader;
	Shader shader;
public:
	Grid(unsigned int xDim, unsigned int yDim);

	void draw(glm::dmat4 projection, glm::dmat4 model);
	void updateCellTemperaturesVBO();
	void step(unsigned int steps, unsigned int mouseX, unsigned int mouseY, int mousePressed, unsigned int radius);
};