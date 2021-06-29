#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <Shader.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//viewport/window dimensions
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

int main()
{
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Window", NULL, NULL);

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

	Shader shader("shader.vs", "shader.fs");

	unsigned int shaderProgram = shader.ID;

	//-------------------------------------------
	//set up vertex data and buffers, configure vertex attributes
	//-------------------------------------------

	//vertex data
	const float vertices[] = {
		 0.5f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
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
	while (!glfwWindowShouldClose(window))
	{
		//process input
		processInput(window);

		//set background color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//activate the shader program
		glUseProgram(shaderProgram);

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
}