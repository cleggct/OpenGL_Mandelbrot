#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <Shader.h>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_cursor_callback(GLFWwindow*, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

//viewport/window dimensions
unsigned int WIDTH = 800;
unsigned int HEIGHT = 800;

//input settings
const double DRAG_SPEED = 1.4; //adjusts mouse-drag speed
const double INC_SPEED = 0.025; //adjusts arrow key speed
const double ZOOM_SPEED = 0.2;

//view settings
float center_x = 0;
float center_y = 0;
float zoom = 1;

int main()
{

	std::string resp1 = " ";
	do {
		std::cout << "Fullscreen? (y/n)\n";
		std::getline(std::cin, resp1);
	} while ((tolower(resp1[0]) != 'y') && (tolower(resp1[0]) != 'n'));

	bool fullscreen = tolower(resp1[0]) == 'y' ? true : false;

	//-------------------------------------------
	//initialize GLFW
	//-------------------------------------------

	glfwInit();

	//set version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//use core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//include this line if on OSX
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//-------------------------------------------
	//create GLFW window
	//-------------------------------------------
	
	//create the window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Mandelbrot", NULL, NULL);


	//if the user requested full screen mode
	if (fullscreen) {
		//make it fullscreen
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		WIDTH = mode->width;
		HEIGHT = mode->height;
	}

	//if the window creation failed, send a message and terminate
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//make the window context that of the current thread
	glfwMakeContextCurrent(window);

	//set the resize callback function of the window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//set the mouse cursor callback function
	glfwSetCursorPosCallback(window, mouse_cursor_callback);

	//set the scroll wheel callback function
	glfwSetScrollCallback(window, scroll_callback);

	//------------------------------------------
	//initialize GLAD
	//------------------------------------------

	//pass the function pointers to glad and return a message if this fails
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//set the size of the viewport
	glViewport(0, 0, WIDTH, HEIGHT);

	//-------------------------------------------
	//set up shader program
	//-------------------------------------------

	//here you can make different choices for the fragment shader
	const std::string vs_source =
#include "shader.vs"
		;

	const std::string fs_source =
#include "shader_3.fs"
		;

	Shader shader(vs_source.c_str(), fs_source.c_str());

	unsigned int shaderProgram = shader.ID;

	//-------------------------------------------
	//set up vertex data and buffers, configure vertex attributes
	//-------------------------------------------

	//vertex data
	const float vertices[] = {
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f
	};

	//triangle vertex indices
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	unsigned int VAO, VBO, EBO;

	//get a vertex array object
	glGenVertexArrays(1, &VAO);
	//get buffer objects
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind the vertex array object
	glBindVertexArray(VAO);

	//bind the vertex buffer object to the array buffer and send the vertex data to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind the element buffer object to the element buffer and send the index data to the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set the vertex attribute pointer(s) and enable the attribute(s)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//unbind vertex array object
	glBindVertexArray(0);

	//unbind the element buffer object (this must happen after we unbind the VAO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//-------------------------------------------
	//main render loop
	//-------------------------------------------

	//enable alpha blending
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);

	while (!glfwWindowShouldClose(window))
	{
		//process input
		processInput(window);

		//set background color
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//get uniform locations
		int dimsLocation = glGetUniformLocation(shaderProgram, "dims");
		int settingsLocation = glGetUniformLocation(shaderProgram, "settings");

		//activate the shader program
		glUseProgram(shaderProgram);

		//set uniform values
		glUniform2f(dimsLocation, WIDTH, HEIGHT);
		glUniform3f(settingsLocation, center_x, center_y, zoom);

		//activate vertex array
		glBindVertexArray(VAO);

		//draw the quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glfw: swap buffers and check events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//de-allocate resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	//clean up resources
	glfwTerminate();
	return 0;
}

//This function is called whenever the window is resized.
//@param window Pointer to the window being resized.
//@param width The width of the resized window.
//@param height The height of the resized window.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//resize the viewport to match the window
	glViewport(0, 0, width, height);
	//update dimensions
	WIDTH = width;
	HEIGHT = height;
}

//Mouse cursor callback function. Handles click-and-drag functionality.
//@param window Pointer to the window to which the mouse is active.
//@param xpos X coordinate of cursor.
//@param ypos Y coordinate of cursor.
void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool mouse_down = false;
	static double last_x = NULL;
	static double last_y = NULL;

	//do nothing if the left button is not being pressed.
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		last_x = NULL;
		last_y = NULL;
		return;
	}

	double corrected_x = xpos / WIDTH;
	double corrected_y = ypos / HEIGHT;

	//do nothing if the previous mouse location has not been set
	if (!(last_x == NULL || last_y == NULL)) {
		center_x += (last_x - corrected_x)/zoom*DRAG_SPEED;
		center_y += (corrected_y - last_y)/zoom*DRAG_SPEED;
	}
	last_x = corrected_x;
	last_y = corrected_y;
}

//Scroll wheel callback function. Handles zoom functionality.
//@param window The currently active window.
//@param xoffset The offset of the scroll wheel on the x axis. 0 for most mice.
//@param yoffset The offset of the scroll wheel on the y axis.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoom *= pow(2, yoffset*ZOOM_SPEED);
}

//This function is called whenever the window receives an input.
//@param The window receiving input.
void processInput(GLFWwindow* window)
{
	//if the escape key is pressed then tell the window to close
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	//if the z key is pressed then increase the zoom
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		zoom *= pow(2, ZOOM_SPEED);
	}

	//if the x key is pressed then decrease the zoom
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		zoom *= pow(2, -ZOOM_SPEED);
	}


	//adjust center of screen when arrow keys are pressed
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		center_y += INC_SPEED / zoom;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		center_y -= INC_SPEED / zoom;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		center_x += INC_SPEED / zoom;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		center_x -= INC_SPEED / zoom;
	}
}