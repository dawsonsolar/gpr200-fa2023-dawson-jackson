//im tired
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <dj/shader.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

std::string vertexShaderSource = dj::loadShaderSourceFromFile("assets/vertexShader.vert");
std::string fragmentShaderSource = dj::loadShaderSourceFromFile("assets/fragmentShader.frag");


struct Vertex {
	float x, y, z;
	float u, v;
};


dj::Vertex vertices[4] = {
	//x    y    z    u    v
	{ -1, -1, 0.0, 0.0, 0.0 }, //Bottom left
	{ 1, -1, 0.0, 1.0, 0.0 }, //Bottom right
	{ 1,  1, 0.0, 1.0, 1.0 },  //Top right
	{ -1, 1, 0.0, 0.0, 1.0 }  //Top left
};

unsigned int indices[6] = {
	0, 1, 2, 
	2, 3, 0  
};

float triangleColor[3] = { 1.0f, 0.5f, 0.0f };
float triangleBrightness = 1.0f;

bool showImGUIDemoWindow = false;
bool drawWireframe = false;

int main()
{
	printf("Initializing...");
	if (!glfwInit())
	{
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL)
	{
		printf("GLFW failed to create window");
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress))
	{
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	// Start ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	glDisable(GL_CULL_FACE);

	dj::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();

	unsigned int vao = dj::createVAO(vertices, 4, indices, 6);

	glBindVertexArray(vao);

	// Set values for shaders
	float sunColor[3] = { 0.76, 0.86, 0.14 };
	float skyColorDay[3] = { 0.67, 0.84, 0.90 };
	float groundColorDay[3] = { 0.91, 0.91, 0.57 };
	float skyColorNight[3] = { 0.01, 0.05, 0.19 };
	float groundColorNight[3] = { 0.12, 0.17, 0.20 };
	float sunSpeed = 1.0;
	float sunSize = 0.3;
	float buildingColor[3] = { 0.4, 0.4, 0.4 };

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		shader.setVec2("aspectRatio", SCREEN_WIDTH, SCREEN_HEIGHT);
		shader.setFloat("Time", glfwGetTime());
		shader.setVec3("sunColor", sunColor[0], sunColor[1], sunColor[2]);
		shader.setVec3("skyColorDay", skyColorDay[0], skyColorDay[1], skyColorDay[2]);
		shader.setVec3("groundColorDay", groundColorDay[0], groundColorDay[1], groundColorDay[2]);
		shader.setVec3("skyColorNight", skyColorNight[0], skyColorNight[1], skyColorNight[2]);
		shader.setVec3("groundColorNight", groundColorNight[0], groundColorNight[1], groundColorNight[2]);
		shader.setVec3("buildingColor", buildingColor[0], buildingColor[1], buildingColor[2]);
		shader.setFloat("sunSpeed", sunSpeed);
		shader.setFloat("sunSize", sunSize);

		if (drawWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			// Create UI for shader parameters
			if (ImGui::CollapsingHeader("Speed Settings"))
			{
				ImGui::SliderFloat("Sun Speed", &sunSpeed, 0.1f, 5.0f);
				ImGui::SliderFloat("Sun Size", &sunSize, 0.1f, 0.9f);
			}

			if (ImGui::CollapsingHeader("Color Settings"))
			{
				ImGui::ColorEdit3("Sun Color", sunColor);
				ImGui::ColorEdit3("Day Color Top", skyColorDay);
				ImGui::ColorEdit3("Day Color Bottom", groundColorDay);
				ImGui::ColorEdit3("Night Color Top", skyColorNight);
				ImGui::ColorEdit3("Night Color Bottom", groundColorNight);
				ImGui::ColorEdit3("Building Color", buildingColor);
			}
			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Triangle Color", triangleColor);
			ImGui::SliderFloat("Brightness", &triangleBrightness, 0.0f, 1.0f);
			ImGui::Checkbox("Draw Wireframe", &drawWireframe);
			ImGui::End();

			if (showImGUIDemoWindow)
			{
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	// bye
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

