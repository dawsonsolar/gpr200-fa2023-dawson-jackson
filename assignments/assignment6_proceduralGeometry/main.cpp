//Debugging help from Sierra Blume.
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>
#include <dj/procGen.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float prevTime;

struct AppSettings 
{
	const char* shadingModeNames[6] = { "Solid Color","Normals","UVs","Texture","Lit","Texture Lit" };
	int shadingModeIndex;
	ew::Vec3 bgColor = ew::Vec3(0.1f);
	ew::Vec3 shapeColor = ew::Vec3(1.0f);
	bool wireframe = true;
	bool drawAsPoints = false;
	bool backFaceCulling = true;

	//Euler angles (degrees)
	ew::Vec3 lightRotation = ew::Vec3(0, 0, 0);
}appSettings;

ew::Camera camera;
ew::CameraController cameraController;

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
	glPointSize(3.0f);
	glPolygonMode(GL_FRONT_AND_BACK, appSettings.wireframe ? GL_LINE : GL_FILL);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg", GL_REPEAT, GL_LINEAR);

	float cubeSize = 0.5f;
	float pWidth = 1, pHeight = 1, pSegments = 5;
	float cHeight = 1, cRad = .5, cSegments = 8;
	float sRad = 1, sSegments = 16;
	float tRad = .5, tThickness = .3, tSegmentsOut = 10, tSegmentsIn = 8;

	//Initialize transformations
	ew::Transform cubeTransform;
	ew::Transform planeTransform;
	ew::Transform cylinderTransform;
	ew::Transform sphereTransform;
	ew::Transform torusTransform;

	//Lining up objects
	planeTransform.position = ew::Vec3(1, 0, .5);
	cylinderTransform.position = ew::Vec3(3, 0, 0);
	sphereTransform.position = ew::Vec3(5, 0, 0);
	torusTransform.position = ew::Vec3(8, 0, 0);

	resetCamera(camera, cameraController);

	while (!glfwWindowShouldClose(window)) 
	{
		glfwPollEvents();
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		cameraController.Move(window, &camera, deltaTime);

		//Render
		glClearColor(appSettings.bgColor.x, appSettings.bgColor.y, appSettings.bgColor.z, 1.0f);

		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setInt("_Mode", appSettings.shadingModeIndex);
		shader.setVec3("_Color", appSettings.shapeColor);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());

		//Euler angels to forward vector
		ew::Vec3 lightRot = appSettings.lightRotation * ew::DEG2RAD;
		ew::Vec3 lightF = ew::Vec3(sinf(lightRot.y) * cosf(lightRot.x), sinf(lightRot.x), -cosf(lightRot.y) * cosf(lightRot.x));
		shader.setVec3("_LightDir", lightF);

		//Create cubes
		ew::MeshData cubeMeshData = ew::createCube(cubeSize);
		ew::MeshData planeMeshData = dj::createPlane(pWidth, pHeight, pSegments);
		ew::MeshData cylinderMeshData = dj::createCylinder(cHeight, cRad, cSegments);
		ew::MeshData sphereMeshData = dj::createSphere(sRad, sSegments);
		ew::MeshData torusMeshData = dj::createTorus(tRad, tThickness, tSegmentsOut, tSegmentsIn);

		ew::Mesh cubeMesh(cubeMeshData);
		ew::Mesh planeMesh(planeMeshData);
		ew::Mesh cylinderMesh(cylinderMeshData);
		ew::Mesh sphereMesh(sphereMeshData);
		ew::Mesh torusMesh(torusMeshData);

		//Draw cube
		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		shader.setMat4("_Model", torusTransform.getModelMatrix());
		torusMesh.draw((ew::DrawMode)appSettings.drawAsPoints);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera"))
			{
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else 
				{
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset"))
				{
					resetCamera(camera, cameraController);
				}
			}

			if (ImGui::CollapsingHeader("Dynamic")) 
			{
				if (ImGui::CollapsingHeader("Cube"))
				{
					ImGui::DragFloat("Size", &cubeSize, .1, .5, 100000);
				}

				if (ImGui::CollapsingHeader("Plane")) 
				{
					ImGui::DragFloat("Width", &pWidth, .1, 1, 100000);
					ImGui::DragFloat("Height", &pHeight, .1, 1, 100000);
					ImGui::DragFloat("Segments", &pSegments, 1, 1, 100000);
				}

				if (ImGui::CollapsingHeader("Cylinder"))
				{
					ImGui::DragFloat("Height", &cHeight, .1, .5, 100);
					ImGui::DragFloat("Cylinder Radius", &cRad, .1, .5, 100);
					ImGui::DragFloat("Segments", &cSegments, 1, 3, 100000);
				}

				if (ImGui::CollapsingHeader("Sphere")) 
				{
					ImGui::DragFloat("Sphere Radius", &sRad, .1, .5, 100);
					ImGui::DragFloat("Segments", &sSegments, 1, 3, 100000);
				}

				if (ImGui::CollapsingHeader("Torus")) 
				{
					ImGui::DragFloat("Torus Radius", &tRad, .1, .5, 100);
					ImGui::DragFloat("Thickness", &tThickness, .1, .3, 100);
					ImGui::DragFloat("Outer Segments", &tSegmentsOut, 1, 3, 1000);
					ImGui::DragFloat("Inner Segments", &tSegmentsIn, 1, 3, 1000);
				}

				if (ImGui::Button("Reset"))
				{
					cubeSize = 0.5f;
					pWidth = 1, pHeight = 1, pSegments = 5;
					cHeight = 1, cRad = .5, cSegments = 8;
					sRad = 1, sSegments = 16;
					tRad = .5, tThickness = .3, tSegmentsOut = 10, tSegmentsIn = 8;
				}
			}

			ImGui::ColorEdit3("BG color", &appSettings.bgColor.x);
			ImGui::ColorEdit3("Shape color", &appSettings.shapeColor.x);
			ImGui::Combo("Shading mode", &appSettings.shadingModeIndex, appSettings.shadingModeNames, IM_ARRAYSIZE(appSettings.shadingModeNames));
			if (appSettings.shadingModeIndex > 3) 
			{
				ImGui::DragFloat3("Light Rotation", &appSettings.lightRotation.x, 1.0f);
			}
			ImGui::Checkbox("Draw as points", &appSettings.drawAsPoints);

			if (ImGui::Checkbox("Wireframe", &appSettings.wireframe))
			{
				glPolygonMode(GL_FRONT_AND_BACK, appSettings.wireframe ? GL_LINE : GL_FILL);
			}

			if (ImGui::Checkbox("Back-face culling", &appSettings.backFaceCulling))
			{
				if (appSettings.backFaceCulling)
					glEnable(GL_CULL_FACE);
				else
					glDisable(GL_CULL_FACE);
			}
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController)
{
	camera.position = ew::Vec3(0, 0, 3);
	camera.target = ew::Vec3(0);
	camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}