// this is getting very frustrating figuring out how to commit.

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

<<<<<<< HEAD
float vertices[21] =
=======
float vertices[21] = 
>>>>>>> 711c11d25a49e722dfc5af2941f70e4d7d5a59d8
{
	//x   //y   //z  //r  //g  //b  //a
	-0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0, //Bottom left
	 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, //Bottom right
	 0.0,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0  //Top center
};
<<<<<<< HEAD
// Vertex shader source
const char* vertexShaderSource = R"(
=======
	// Vertex shader source
	const char* vertexShaderSource = R"(
>>>>>>> 711c11d25a49e722dfc5af2941f70e4d7d5a59d8
		#version 450
		layout(location = 0) in vec3 vPos;
		layout(location = 1) in vec4 vColor;
		out vec4 Color;
		uniform float _Time;
		void main() 
		{
			Color = vColor;
			vec3 offset = vec3(0, sin(vPos.x + _Time), 0) * 0.5;
			gl_Position = vec4(vPos + offset, 1.0);
		}
	)";

<<<<<<< HEAD
// Fragment shader source
const char* fragmentShaderSource = R"(
=======
	// Fragment shader source
	const char* fragmentShaderSource = R"(
>>>>>>> 711c11d25a49e722dfc5af2941f70e4d7d5a59d8
		#version 450
		out vec4 FragColor;
		in vec4 Color;
		uniform float _Time;
		void main()
		{
			FragColor = Color * abs(sin(_Time));
		}
	)";

<<<<<<< HEAD
// New VAO with vertex data.
unsigned int createVAO(float* vertexData, int numVertices)
{
	//Define position attributes (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	// VBO

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for and send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// VAO

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// VAO pulls vertex data from VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Define position attributes (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)0);
	glEnableVertexAttribArray(0);

	// Color attributes
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	return vao;
}

// Creates a new shader of a given type.
// Possible types: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc)
// Returns id of the shader object.
unsigned int createShader(GLenum shaderType, const char* sourceCode)
{
	// Create a new shader object, give it source code, and compile.
	unsigned int shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &sourceCode, NULL);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// 500 characters for the error message. (may be a little too much)
		char infoLog[500];
		glGetShaderInfoLog(shader, 500, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
	}

	return shader;
}

// Creates a new shader program with vertex + fragment stages
// Returns id of new shader program if successful, 0 if failed
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	// Create a new vertex shader object
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);

	// Create a new fragment shader object
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	// Attach shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for errors
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

=======
	// New VAO with vertex data.
	unsigned int createVAO(float* vertexData, int numVertices)
	{
		//Define position attributes (3 floats)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
		glEnableVertexAttribArray(0);

		// VBO

		//Define a new buffer id
		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//Allocate space for and send vertex data to GPU.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		// VAO

		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		// VAO pulls vertex data from VBO
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//Define position attributes (3 floats)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)0);
		glEnableVertexAttribArray(0);

		// Color attributes
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(1);

		return vao;
	}

	// Creates a new shader of a given type.
	// Possible types: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc)
	// Returns id of the shader object.
	unsigned int createShader(GLenum shaderType, const char* sourceCode)
	{
		// Create a new shader object, give it source code, and compile.
		unsigned int shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &sourceCode, NULL);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			// 500 characters for the error message. (may be a little too much)
			char infoLog[500];
			glGetShaderInfoLog(shader, 500, NULL, infoLog);
			printf("Failed to compile shader: %s", infoLog);
		}

		return shader;
	}

	// Creates a new shader program with vertex + fragment stages
// Returns id of new shader program if successful, 0 if failed
	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		// Create a new vertex shader object
		unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);

		// Create a new fragment shader object
		unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		unsigned int shaderProgram = glCreateProgram();
		// Attach shaders
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// Check for errors
		int success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			printf("Failed to link shader program: %s", infoLog);
		}
		
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		return shaderProgram;
	}

>>>>>>> 711c11d25a49e722dfc5af2941f70e4d7d5a59d8
int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

<<<<<<< HEAD



=======
	

	
>>>>>>> 711c11d25a49e722dfc5af2941f70e4d7d5a59d8




	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//Tell vao to pull vertex data from vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Define position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	// Shader Program
	unsigned int shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	// Create new VAO with vertex data
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// current time in frames
		float time = (float)glfwGetTime();

		// Find the uniform location
		int location = glGetUniformLocation(shader, "_Time");

		// Set the value of the variable at the location
		glUniform1f(location, time);

		// Construct sequence of primitives
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draw the uniform triangle
		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
