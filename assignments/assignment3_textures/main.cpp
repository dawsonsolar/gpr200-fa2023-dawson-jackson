//Debugging help from Sierra Blume and Ben Emag.

#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <dj/shader.h>
#include <dj/texture.cpp>


struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

Vertex vertices[4] = {
	{-1.0, -1.0, 0.0, 0.0, 0.0},
	{1.0, -1.0, 0.0, 1.0, 0.0},
	{1.0, 1.0, 0.0, 1.0, 1.0},
	{-1.0, 1.0, 0.0, 0.0, 1.0}
};
unsigned short indices[6] = {
	0, 1, 2,
	2, 3, 0
};

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	ew::Shader backgroundShader("assets/background.vert", "assets/background.frag");
	ew::Shader characterShader("assets/character.vert", "assets/character.frag");

	// Load Textures
	unsigned int brickTexture = loadTexture("assets/brickwall.png", 2, 2);
	unsigned int noiseTexture = loadTexture("assets/noise.png", 1, 1);
	unsigned int characterTexture = loadTexture("assets/The_Kid.png", 1, 0);

	// Parameters for textures used in .frag and vert
	int imageSizeWidth = 128;
	int imageSizeHeight = 128;
	float speed = 0.5;
	float alphaRate = 0.5;
	float noiseRate = 5.0;
	float scrollSpeed = -0.5;
	

	


	// Put background textures into shader
	backgroundShader.use();
	backgroundShader.setInt("_BrickTexture", 0);
	backgroundShader.setInt("_NoiseTexture", 1);

	// Same with character shader
	characterShader.use();
	characterShader.setInt("characterTexture", 2);


	unsigned int quadVAO = createVAO(vertices, 4, indices, 6);
	glBindVertexArray(quadVAO);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float setTime = (float)glfwGetTime();

		// Draw the background 
		backgroundShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		backgroundShader.setInt("_BrickTexture", 0);


		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, noiseTexture);
		backgroundShader.setInt("noiseTexture", 1);

		backgroundShader.setFloat("noiseRate", noiseRate);
		backgroundShader.setFloat("time", glfwGetTime());
		backgroundShader.setFloat("scrollSpeed", scrollSpeed);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);


		// Draw the Character
		characterShader.use();
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, characterTexture);
		characterShader.setInt("characterTexture", 5);
		characterShader.setFloat("time", glfwGetTime());
		characterShader.setVec2("imgSize", imageSizeWidth, imageSizeHeight);
		characterShader.setVec2("aspectRatio", SCREEN_WIDTH, SCREEN_HEIGHT);
		characterShader.setFloat("speed", speed);
		characterShader.setFloat("alphaRate", alphaRate);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);



		//Render UI, im not adding to this.
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Vertex Buffer Object 
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertexData, GL_STATIC_DRAW);

	//Element Buffer Object
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndices, indicesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,x));
	glEnableVertexAttribArray(0);

	//UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

