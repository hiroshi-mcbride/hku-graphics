#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// forward declaration
void processInput(GLFWwindow* window);
int init(GLFWwindow*& window);
void createTriangle(GLuint &VAO, int &size);
void createShaders();
void createProgram(GLuint& programID, const char* vertex, const char* fragment);
bool loadFile(const char* filename, char*& output);

GLuint simpleProgram;

int main() {

	GLFWwindow* window;
	int result = init(window);
	if (result != 0) {
		return result;
	}

	GLuint triangleVAO;
	int triangleSize;
	
	createTriangle(triangleVAO, triangleSize);
	createShaders();

	// create viewport
	glViewport(0, 0, 1280, 720);

	// game render loop
	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.0, 0.5, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(simpleProgram);

		glBindVertexArray(triangleVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, triangleSize);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// terminate
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int init(GLFWwindow*& window) {
	glfwInit();

	// tell glfw which profile & openGL version to use
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// make window
	window = glfwCreateWindow(1280, 720, "hku-graphics", nullptr, nullptr);
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
	return 0;
}

void createTriangle(GLuint& VAO, int& size) {
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	size = sizeof(vertices);
}

void createShaders() {
	createProgram(simpleProgram, "shaders/sampleVertex.shader", "shaders/sampleFragment.shader");
}

void createProgram(GLuint& programID, const char* vertex, const char* fragment) {
	char* vertexSrc;
	char* fragmentSrc;

	loadFile(vertex, vertexSrc);
	loadFile(fragment, fragmentSrc);

	GLuint vertexShaderID, fragmentShaderID;

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexSrc, nullptr);
	glCompileShader(vertexShaderID);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderID, 512, nullptr, infoLog);
		std::cerr << "error compiling vertex shader\n" << infoLog << std::endl;
	}

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentSrc, nullptr);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderID, 512, nullptr, infoLog);
		std::cerr << "error compiling vertex shader\n" << infoLog << std::endl;
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programID, 512, nullptr, infoLog);
		std::cerr << "error linking program\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	delete vertexSrc;
	delete fragmentSrc;
}

bool loadFile(const char* filename, char*& output) {
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return false;
	}

	// Get the length of the file
	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);

	// Allocate memory for the text
	output = new char[length + 1]; // +1 for null terminator

	// Read the file into the buffer
	file.read(output, length);
	output[length] = '\0'; // Add null terminator

	// Close the file
	file.close();

	return true;
}