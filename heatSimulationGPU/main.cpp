#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include "grid.h"

#define FULLSCREEN 0

#if FULLSCREEN
#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080
#else
#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#endif

std::string windowTitle = "Heat";

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void processKeyboardInput(GLFWwindow* window);

unsigned int frames = 0;
double lastFPSupdate = 0.0;

#define PIXELS_PER_CELL 2

int mouseX = SCR_WIDTH / 2;
int mouseY = SCR_HEIGHT / 2;
int mousePressed = 0;
double radius = 100.0 / PIXELS_PER_CELL;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowTitle.c_str(), FULLSCREEN ? glfwGetPrimaryMonitor() : NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);

	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Grid grid(SCR_WIDTH / PIXELS_PER_CELL, SCR_HEIGHT / PIXELS_PER_CELL);

	while (!glfwWindowShouldClose(window)) {
		processKeyboardInput(window);

		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::dmat4 projection = glm::ortho(0.0, 1.0, 0.0, 1.0);
		glm::dmat4 model;

		grid.step(5, mouseX / PIXELS_PER_CELL, (SCR_HEIGHT - mouseY) / PIXELS_PER_CELL, mousePressed, int(radius));
		grid.updateCellTemperaturesVBO();

		grid.draw(projection, model);

		glfwSwapBuffers(window);
		glfwPollEvents();

		frames++;

		if (glfwGetTime() - lastFPSupdate > 1.0) {
			glfwSetWindowTitle(window, (windowTitle + " (" + std::to_string(frames) + " FPS)").c_str());
			frames = 0;
			lastFPSupdate = glfwGetTime();
		}
	}

	glfwTerminate();
}

void processKeyboardInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	mouseX = xpos;
	mouseY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_RELEASE) {
		mousePressed = 0;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mousePressed = 1;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		mousePressed = 2;
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	radius += yoffset * 5 / PIXELS_PER_CELL;

	if (radius < 0.0) {
		radius = 0.0;
	}
}