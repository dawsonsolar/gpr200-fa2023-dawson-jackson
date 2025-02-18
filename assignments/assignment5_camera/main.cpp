// Debugging help from Sierra Blume
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <dj/camera.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void moveCamera(GLFWwindow* window, dj::Camera* camera, dj::CameraControls* controls, float deltaTime);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];
dj::Camera camera;
dj::CameraControls controls;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	// Default values
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0, 0, 0);
	camera.aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	camera.fov = 60;
	camera.orthoSize = 6;
	camera.nearPlane = 0.1;
	camera.farPlane = 100;
	bool orbit = false;
	float orbitSpeed = 1.0;

	float previousFrame = (float)glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();
		shader.setMat4("_View", camera.ViewMatrix());
		shader.setMat4("_Projection", camera.ProjectionMatrix());

		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		float currentFrame = (float)glfwGetTime(); 
		float deltaTime = currentFrame - previousFrame;
		previousFrame = currentFrame;

		moveCamera(window, &camera, &controls, deltaTime);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Camera");
			ImGui::Checkbox("Orbit", &orbit);

			if (orbit)
			{
				ImGui::DragFloat("Orbit Speed:", &orbitSpeed, 0.05f);

				ew::Vec3 forward = camera.target - camera.position;
				forward = ew::Normalize(forward);

				
				ew::Vec3 right = ew::Cross(forward, ew::Vec3(0, 1, 0));
				right = ew::Normalize(right);

				camera.position += right * orbitSpeed;

				/* Commented since this was for check in
				camera.position.x = cosf((float)glfwGetTime() * orbitSpeed) * 5.0;
				camera.position.z = sinf((float)glfwGetTime() * orbitSpeed) * 5.0;
				*/
			}

			ImGui::DragFloat3("Position:", &camera.position.x, 0.5f);
			ImGui::DragFloat3("Target:", &camera.target.x, 0.5f);
			ImGui::Checkbox("Orthographic", &camera.orthographic);

			if (camera.orthographic == true)
			{
				ImGui::DragFloat("Orthographic Height:", &camera.orthoSize, 0.05f);
			}
			else
			{
				ImGui::SliderFloat("Field Of View:", &camera.fov, 0.0, 180.0);
			}

			ImGui::DragFloat("Near Plane:", &camera.nearPlane, 0.05f, 0.001);
			ImGui::DragFloat("Far Plane:", &camera.farPlane, 0.05f, 0.001);

			
			
			// Reset Button
			if (ImGui::Button("Reset"))
			{
				camera.position = ew::Vec3(0, 0, 5);
					camera.target = ew::Vec3(0, 0, 0);
					camera.aspectRatio = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
					camera.fov = 60;
					camera.orthoSize = 6;
					camera.nearPlane = 0.1;
					camera.farPlane = 100;
					bool orbit = false;
					orbitSpeed = 1.0;
			}
		
			for (size_t i = 0; i < NUM_CUBES; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) 
				{
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}
			ImGui::Text("Camera");
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void moveCamera(GLFWwindow* window, dj::Camera* camera, dj::CameraControls* controls, float deltaTime)
{
	//If right mouse is not held, release cursor and return early
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) 
	{
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}

	//GLFW_CURSOR_DISABLED hides the cursor, but the position will still be changed as we move our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Get screen mouse position this frame
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//If we just started right clicking, set prevMouse values to current position
	//This prevents a bug where the camera moves as soon as we click
	if (controls->firstMouse)
	{
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	//TODO: Get mouse position delta for this frame
	float deltaX = mouseX - controls->prevMouseX;
	float deltaY = mouseY - controls->prevMouseY;

	///TODO: Add to yaw and pitch
	controls->yaw += deltaX * 0.1;
	controls->pitch -= deltaY * 0.1;

	//TODO: Clamp pitch between -89 and 89 degrees
	float pitchMin = -89.0;
	float pitchMax = 89.0;
	controls->pitch = dj::clamp(controls->pitch, pitchMin, pitchMax);

	//Remember previous mouse position
	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;

	//Construct forward vector using yaw and pitch. Don't forget to convert to radians!
	float yawRad = ew::Radians(controls->yaw);
	float pitchRad = ew::Radians(controls->pitch);

	ew::Vec3 forward = ew::Vec3(sin(yawRad) * cos(pitchRad), sin(pitchRad), -cos(yawRad) * cos(pitchRad));

	//TODO: Using camera forward and world up (0,1,0), construct camera right and up vectors. Graham-schmidt process!
	ew::Vec3 worldUp = ew::Vec3(0, 1, 0);
	ew::Vec3 right = ew::Normalize(ew::Cross(forward, worldUp));
	ew::Vec3 up = ew::Normalize(ew::Cross(right, forward));

	//TODO: Keyboard controls for moving along forward, back, right, left, up, and down
	if (glfwGetKey(window, GLFW_KEY_W))
	{
		camera->position += forward * controls->moveSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_S))
	{
		camera->position -= forward * controls->moveSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_D)) 
	{
		camera->position += right * controls->moveSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_A)) 
	{
		camera->position -= right * controls->moveSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_Q)) 
	{
		camera->position += up * controls->moveSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_E)) 
	{
		camera->position -= up * controls->moveSpeed * deltaTime;
	}

	//By setting target to a point in front of the camera along its forward direction, our LookAt will be updated accordingly when rendering
	camera->target = camera->position + forward;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.aspectRatio = (float)(width) / (float)(height);
}

