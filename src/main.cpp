#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "math.hpp"

using namespace std;

// Define the necessary shaders to allow drawing --
const char* vertexShaderSrc = "#version 330 core\n"
	"layout(location = 0) in vec2 aPos;\n"
	"void main() {\n"
    "	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
	"}\0"; 

const char* fragShaderSrc = "#version 330 core\n"
	"uniform vec3 colour;\n"
	"out vec4 fragColour;\n"
	"void main() {\n"
	"	fragColour = vec4(colour, 1.0f);\n"
	"}\0";
// -- 

// Constants --
constexpr float UNIT_RATIO = 0.322f; // Ratio of screen units to AU, numerous trials suggested 0.322 is optimal  

// Simulation parameters relative to the units defined in math.hpp
constexpr double STAR_MASS = 1.0;
constexpr double A = 1.0; // Semi-major axis
constexpr double E = 0.0167; // Eccentricity
constexpr double TIME_SCALE = 3155760.0; // Adjusted so that 10 seconds = 1 year in game
// --

// Callbacks --
// Window size changes
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// Self-explanatory, will eventually include movement 
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

	// I should probably make this process a function
	// Declare Vertex Shader to help GL determine vertex positions
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);

	// Check if compilation was successful --
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

	// Check if compilation was successful --
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
	
	// Check if link was successful --
	glGetProgramiv(shader, GL_LINK_STATUS, &success);

	if (!success) {
    	glGetProgramInfoLog(shader, 512, NULL, errorLog);
	}
	// --

	// Get rid of the old shaders after linking them (not needed anymore)
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	// Define vertex data and the location of our uniform variable
	int colourLocation = glGetUniformLocation(shader, "colour");
	float vertices[] = {0.0f, 0.0f}; 

	// Declare a buffer object to store vertices in the GPU
	// Declare an array object to store VBO configurations
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	// Select it, put shit into it, tell GL how to interpret it, and enable it
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); // GL_DYNAMIC_DRAW tells GL to buffer it somewhere fast since we gonna be messing with it
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Deselect everything - a remnant of learngl.com cause i aint doing that
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindVertexArray(0);

	// Set up orbital parameters
	Orbit earth;
	earth.a = A;
	earth.e = E;

	double inGameTime = 0.0; // ingame the funniest sh ive ever invented and it aint even close
	double past = glfwGetTime(); 

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		
		double present = glfwGetTime();
		double dt = present - past;
		inGameTime += dt * TIME_SCALE;
		past = present; // w naming

		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shader);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		// Draw central star
		float starVertices[] = {0.0f, 0.0f};
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(starVertices), starVertices);
		glUniform3f(colourLocation, 1.0f, 0.0f, 0.0f); // p sure this is red
		glPointSize(25.0f);
		glDrawArrays(GL_POINTS, 0, 1);

		// Determine where the body (in this case, Earth) is
		glm::dvec2 earthPos = orbitPosition(earth, STAR_MASS, inGameTime);
		float earthVertices[] = {
			(float)(earthPos.x * UNIT_RATIO),
			(float)(earthPos.y * UNIT_RATIO)
        };

		// Draw body - might add a trail later
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(earthVertices), earthVertices);
		glUniform3f(colourLocation, 0.0f, 1.0f, 0.0f); // p sure this is blue
		glPointSize(10.0f); // Not to scale, accurate shape though
		glDrawArrays(GL_POINTS, 0, 1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// janitor
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
	glfwTerminate();
	
	return 0;
}