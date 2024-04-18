#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
	glfwInit();

	// tell glfw which profile & openGL version to use
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// make window
	GLFWwindow *window = glfwCreateWindow(1280, 720, "eat ass", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "failed to create GLFW window" << std::endl;
		return -1;
	}

	// set context
	glfwMakeContextCurrent(window);

	// GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to load GLAD" << std::endl;
		glfwTerminate();
		return -2;
	}

	// create viewport
	// glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// game render loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(1.0, 0.5, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// terminate
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}