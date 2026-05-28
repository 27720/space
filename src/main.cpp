#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

// Define the necessary shaders to allow drawing --

const char* vertexShaderSrc = "#version 330 core\n"
	"layout(location = 0) in vec3 aPos;\n"
	"void main() {\n"
    "	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0"; 

const char* fragShaderSrc = "#version 330 core\n"
	"out vec4 fragColour;\n"
	"void main() {\n"
	"	fragColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\0";

// --

// Callback in case the window's size changes
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Self-explanatory
// Will eventually include movement keys
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {

	if (!glfwInit()) return -1; // Initialise GLFW

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "balls", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Initialise the GLAD loader by passing GLFW's function that knows how to fetch the addresses of OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

	glViewport(0, 0, 800, 600);

	// Declare Vertex Shader to help GL determine vertex positions
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);

	// Checks if compilation was successful --
	int success;
	char errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cout << "Vertex shader compilation failed" << errorLog << std::endl;
	}
	// --

	// Declare Fragment Shader to determine each pixel's colour as vertices are mapped onto the window
	unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// Checks if compilation was successful --
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragShader, 512, NULL, errorLog);
		std::cout << "Fragment shader compilation failed" << errorLog << std::endl;
	}
	// --

	// Link our two shaders
	unsigned int shader;
	shader = glCreateProgram();
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragShader);
	glLinkProgram(shader);
	
	// Checks if link was successful --
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	if (!success) {
    	glGetProgramInfoLog(shader, 512, NULL, errorLog);
	}
	// --

	// Get rid of old shaders after linking them (not needed anymore)
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	// Setup vertex data
	float triangle1[] = {
		-0.875f, -0.5f, 0.0f,
		-0.125f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	float triangle2[] = {
		0.125f, -0.5f, 0.0f,
		0.875f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

	// Declare a buffer object to store vertices in the GPU
	// Declare an array object to store VBO configurations
	unsigned int VBO1, VBO2, VAO1, VAO2;
	glGenVertexArrays(1, &VAO1);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO1);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO1);

	// Select it, put shit into it, tell GL how to interpret it, and enable it
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO2);

	// Select it, put shit into it, tell GL how to interpret it, and enable it 
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Deselect everything
	// Not necessary whatsoever asides from maybe deselecting the VBO since the VAO handles its selection in the render loop
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// janitor (slow rn fixing later)
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
	glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}