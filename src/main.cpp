#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "util/shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "util/stb_image.h"
#include "util/camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>	
using namespace std;

// ----------------------------------------------------- Global Variables -----------------------------------------------
const float SCREEN_WIDTH = 800;
const float SCREEN_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = (float)(SCREEN_WIDTH / 2);
float lastY = (float)(SCREEN_HEIGHT / 2);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// ------------------------ Function to properly resize the window -------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// ----------------------------- Contains all our code to process user input ---------------------------
void processInput(GLFWwindow* window) {

	// Close window if escape key pressed
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// W key
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	// A key
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	
	// S key
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	// D key
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

// ------------------------------------------------ Process Mouse Input -------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Reversed because y coords go from bottom to top

	lastX = xpos;
	lastY = ypos;
	
	camera.ProcessMouseMovement(xoffset, yoffset);

}

// -------------------------------------------------- Zoom Controls ------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}
// ------------------------------------------ Main -----------------------------------------------------
int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Graphics Engine", NULL, NULL);

	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	// ------------------------------------------------ Callbacks -------------------------------------------------------
	// Adjusts the viewport if the window is resized ensuring that proper coordinate mapping occurs
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// EVerytime mosue is moved, thise is called
	glfwSetCursorPosCallback(window, mouse_callback);

	// Scroll wheel for zoom
	glfwSetScrollCallback(window, scroll_callback);

	// ----------------------------------------- Shader Program -------------------------------------------
	Shader threeDShaderProgram("shaders/vertex/3dVertexShader.txt", "shaders/fragment/3dFragmentShader.txt");
	Shader simpleShader("shaders/vertex/simpleVertexShader.txt", "shaders/fragment/simpleFragmentShader.txt");
	Shader lightingShader("shaders/vertex/simpleVertexShader.txt", "shaders/fragment/lightingFragmentShader.txt");

	// Create VBO, an ID for our buffer, and assigns it to our variable VBO
	// A buffer object is an object that stores data in memory	
	unsigned int VBOs[2], VAOs[2];
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);

	// ------------------------------------------ tau cube ---------------------------------------------------------------
	float textured_cube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textured_cube), textured_cube, GL_STATIC_DRAW);

	// Pos coords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// ------------------------------------------ Dupe Cubes ------------------------------------------------------------
	glm::vec3 tau_cubes[] = {
		// glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// ---------------------------------------- Blank Cube --------------------------------------------
	float cube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	// pos coords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// -------------------------------------------- Textures -------------------------------------------
	unsigned int tau_texture, container_texture;
	glGenTextures(1, &tau_texture);
	glBindTexture(GL_TEXTURE_2D, tau_texture);

	int widthContainer, heightContainer, numColorChannelsContainer;
	// last argument = desired number of channels, leave at 0 to keep original
	unsigned char* dataTextureContainer = stbi_load("textures/cat.jpg", &widthContainer, &heightContainer, &numColorChannelsContainer, 0);

	// Setting texture wrapping params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Setting texture filtering params
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	if (dataTextureContainer) {
		// now that texture is bound, we can generate it
		// first target: texture target
		// 2nd arg: mipmap level, we leave it at the base val = 0
		// 3rd arg: what format we want to store the texture, we use RBG
		// 4 + 5th arg: width + height
		// 6th: legacy stuff, should always be 0
		// 7 + 8th: format + datatype of source image, we loaded image as RGB and stores as chars (bytes)
		// 9th: our actual image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthContainer, heightContainer, 0, GL_RGB, GL_UNSIGNED_BYTE, dataTextureContainer);
		// generate all the required mipmaps for the currently bound texture.
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture" << endl;
	}
	

	// Cleanup, free image in memory
	stbi_image_free(dataTextureContainer);

	glGenTextures(1, &container_texture);
	glBindTexture(GL_TEXTURE_2D, container_texture);
	int width, height, numColorChannels;
	// last argument = desired number of channels, leave at 0 to keep original
	unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &numColorChannels, 0);

	if (data) {
		// now that texture is bound, we can generate it
		// first target: texture target
		// 2nd arg: mipmap level, we leave it at the base val = 0
		// 3rd arg: what format we want to store the texture, we use RBG
		// 4 + 5th arg: width + height
		// 6th: legacy stuff, should always be 0
		// 7 + 8th: format + datatype of source image, we loaded image as RGB and stores as chars (bytes)
		// 9th: our actual image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// generate all the required mipmaps for the currently bound texture.
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "Failed to load texture" << endl;
	}

	// -------------------------------------------- Lighting ------------------------------------------
	//unsigned int lightVBO, lightVAO;
	unsigned int lightVAO;
	//glGenBuffers(1, &lightVBO);
	//glGenBuffers(1, &lightVBO);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// ------------------------------ Cleanup ---------------------------------------------------------
	// Cleanup, free image in memory
	stbi_image_free(data);

	// bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tau_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, container_texture);

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // capture mouse input

	
	// -------------------------------------------- Render Loop ----------------------------------------
	while (!glfwWindowShouldClose(window)) {
		// Process inputs
		processInput(window);

		// Rendering stuff
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set the clear color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear using the color
		
		float currTime = glfwGetTime();

		// Calculate frame time
		float currentFrame = currTime;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
			
		// Use our shader
		// Draw blank cube
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();

		// FOV, aspect ratio, near matrix, far matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 16.0f / 10.0f, 0.1f, 100.0f);


		simpleShader.use();
		simpleShader.setFloat3f("objectColor", 1.0f, 0.5f, 0.31f);
		simpleShader.setFloat3f("lightColor", 1.0f, 1.0f, 1.0f);
		simpleShader.setFloat3fv("lightPos", lightPos);
		simpleShader.setMatrixTransform4fv("view", view);
		simpleShader.setMatrixTransform4fv("projection", projection);
		simpleShader.setMatrixTransform4fv("model", model);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// lighting
		lightingShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightingShader.setMatrixTransform4fv("model", model);
		lightingShader.setMatrixTransform4fv("view", view);
		lightingShader.setMatrixTransform4fv("projection", projection);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		// Cube
		threeDShaderProgram.use();
		threeDShaderProgram.setInt("ourTexture", 0);
		
		threeDShaderProgram.setMatrixTransform4fv("view", view);
		threeDShaderProgram.setMatrixTransform4fv("projection", projection);
		threeDShaderProgram.setFloat3f("objectColor", 1.0f, 1.0f, 1.0f);
		threeDShaderProgram.setFloat3f("lightColor", 1.0f, 1.0f, 1.0f);

		glBindVertexArray(VAOs[0]);
		for (unsigned int i = 0; i < std::size(tau_cubes); i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, tau_cubes[i]);
			model = glm::rotate(model, currTime * glm::radians(50.0f) * (i), glm::vec3(0.5f, 1.0f, 0.0f));
			threeDShaderProgram.setMatrixTransform4fv("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();


	}

	// Exit and close the window
	glfwTerminate();
	return 0;
}

