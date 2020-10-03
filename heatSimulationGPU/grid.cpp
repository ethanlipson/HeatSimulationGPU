#include "grid.h"

Grid::Grid(unsigned int xDim, unsigned int yDim) {
	this->xDim = xDim;
	this->yDim = yDim;

	shader = Shader("shader.vert", "shader.frag");
	computeShader = ComputeShader("shader.comp");

	std::vector<float> cellTemperatures;

	for (unsigned int x = 0; x < xDim; x++) {
		for (unsigned int y = 0; y < yDim; y++) {
			for (int i = 0; i < 6; i++) {
				cellTemperatures.push_back(0);
			}
		}
	}

	/*****************************/

	std::vector<float> cellPositions;

	for (int x = 0; x < xDim; x++) {
		for (int y = 0; y < yDim; y++) {
			float p1[2] = { (float(x) / xDim), (float(y) / yDim) };
			float p2[2] = { (float(x + 1) / xDim), (float(y) / yDim) };
			float p3[2] = { (float(x) / xDim), (float(y + 1) / yDim) };
			float p4[2] = { (float(x + 1) / xDim), (float(y + 1) / yDim) };

			cellPositions.insert(cellPositions.end(), p1, p1 + 2);
			cellPositions.insert(cellPositions.end(), p2, p2 + 2);
			cellPositions.insert(cellPositions.end(), p4, p4 + 2);

			cellPositions.insert(cellPositions.end(), p1, p1 + 2);
			cellPositions.insert(cellPositions.end(), p3, p3 + 2);
			cellPositions.insert(cellPositions.end(), p4, p4 + 2);
		}
	}

	glGenVertexArrays(1, &cellVAO);
	glGenBuffers(1, &cellPositionsVBO);
	glGenBuffers(1, &cellTemperaturesVBO);

	glBindVertexArray(cellVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cellPositionsVBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * xDim * yDim * sizeof(float), cellPositions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, cellTemperaturesVBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * xDim * yDim * sizeof(float), cellTemperatures.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/**************************/

	glGenBuffers(1, &cellTemperaturesSSBO);
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, cellTemperaturesSSBO);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 6 * xDim * yDim * sizeof(float), cellTemperatures.data(), GL_DYNAMIC_READ);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cellTemperaturesSSBO);
}

void Grid::draw(glm::dmat4 projection, glm::dmat4 model) {
	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("model", model);

	glBindVertexArray(cellVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * xDim * yDim);
}

void Grid::updateCellTemperaturesVBO() {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, cellTemperaturesSSBO);
	glBindBuffer(GL_ARRAY_BUFFER, cellTemperaturesVBO);
	glCopyBufferSubData(GL_SHADER_STORAGE_BUFFER, GL_ARRAY_BUFFER, 0, 0, 6 * xDim * yDim * sizeof(float));
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Grid::step(unsigned int steps, unsigned int mouseX, unsigned int mouseY, int mousePressed, unsigned int radius) {
	computeShader.use();
	computeShader.setUVec2("mousePos", mouseX, mouseY);
	computeShader.setInt("mousePressed", mousePressed);
	computeShader.setUInt("radius", radius);
	computeShader.setFloat("alpha", 0.3);

	for (int i = 0; i < steps; i++) {
		glDispatchCompute(xDim, yDim, 1);
	}
}