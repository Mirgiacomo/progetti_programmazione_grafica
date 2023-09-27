#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <OpenGL/shader_m.h>
#include <OpenGL/shader_s.h>
#include <OpenGL/camera.h>
#include <OpenGL/model.h>
#include <iostream>


// definizione funzioni di callback per operazioni sulla finestra
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// impostazioni finestra
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// impostazioni camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// gestione velocita'
float deltaTime = 0.0f;	// tempo fra ultimo frame e quello corrente
float lastFrame = 0.0f;

int main()
{
	// inizializzazione e configurazione glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// creazione finestra, impostazione contesto e funzioni callback glfw
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Esercitazione 1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// mouse listener
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// caricamento puntatori funzioni opengl
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// inversione su asse y
	stbi_set_flip_vertically_on_load(true);

	// abilitazione z buffer opengl
	glEnable(GL_DEPTH_TEST);

	// creazione shaders
	Shader modelShader("model.vs", "model.fs");
	Shader lightCubeShader("light_cube.vs", "light_cube.fs");

	// caricamento modello
	Model ourModel("backpack/backpack.obj");

	// vertici di un cubo
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	// posizione dei cubi
	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.0,  2.0f,  1.0f),
		glm::vec3(-2.0f, -2.0f, 1.0f),
		glm::vec3(2.0f, -2.0f, -1.0f),
		glm::vec3(-2.0f, 2.0f, -1.0f),
		glm::vec3(0.0f,  0.0f, 0.0f),
		glm::vec3(0.0f,  0.0f, 0.0f)
	};

	// vertex buffer object e vertex array object
	unsigned int VBO, lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glGenBuffers(1, &VBO);

	// associo la memoria
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// posizione
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

	// ciclo di rendering
	while (!glfwWindowShouldClose(window))
	{
		// gestione delta time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// gestione input
		processInput(window);

		// render e reimpostazione buffer colore e profondita'
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// impostazione delle luci tramite variabili uniform
		// ogni luce ha diverse componenti: direzionale, ambientale, diffusiva e speculare
		// le point lights hanno anche una posizione che le inserisce nella scena
		// le componenti "costante", "lineare", "quadratica" servono per il calcolo dell'attenuata
		// la spotlight è posizionata sulla camera verso la scena 

		modelShader.use();
		//modelShader.setVec3("lightPos", lightPos);
		modelShader.setVec3("viewPos", camera.Position);
		modelShader.setFloat("material.shininess", 32.0f);

		// luce direzionale
		modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		modelShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		modelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

		// point lights 1-4
		for (unsigned int i = 0; i < 4; i++) {
			modelShader.setVec3("pointLights[" + to_string(i) + "].position", pointLightPositions[i]);
			modelShader.setVec3("pointLights[" + to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
			modelShader.setVec3("pointLights[" + to_string(i) + "].diffuse", 0.8f, 0.8f, 0.8f);
			modelShader.setVec3("pointLights[" + to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
			modelShader.setFloat("pointLights[" + to_string(i) + "].constant", 1.0f);
			modelShader.setFloat("pointLights[" + to_string(i) + "].linear", 0.09);
			modelShader.setFloat("pointLights[" + to_string(i) + "].quadratic", 0.032);
		}

		// point light 5
		// in questo caso le coordinate y e z cambiano in base al tempo per far ruotare la luce
		float light5_mov_y = pointLightPositions[4].y + cos(glfwGetTime()) * 2.5;
		float light5_mov_z = pointLightPositions[4].z + sin(glfwGetTime()) * 2.0;
		modelShader.setVec3("pointLights[4].position", pointLightPositions[4].x, light5_mov_y, light5_mov_z);
		modelShader.setVec3("pointLights[4].ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3("pointLights[4].diffuse", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3("pointLights[4].specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat("pointLights[4].constant", 1.0f);
		modelShader.setFloat("pointLights[4].linear", 0.09);
		modelShader.setFloat("pointLights[4].quadratic", 0.032);

		// point light 6
		// in questo caso le coordinate x e z cambiano in base al tempo per far ruotare la luce
		float light6_mov_x = pointLightPositions[5].x + cos(glfwGetTime() + 2) * 2.5;
		float light6_mov_z = pointLightPositions[5].z + sin(glfwGetTime() + 2) * 2.0;
		modelShader.setVec3("pointLights[5].position", light6_mov_x, pointLightPositions[5].y, light6_mov_z);
		modelShader.setVec3("pointLights[5].ambient", 0.05f, 0.05f, 0.05f);
		modelShader.setVec3("pointLights[5].diffuse", 0.8f, 0.8f, 0.8f);
		modelShader.setVec3("pointLights[5].specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat("pointLights[5].constant", 1.0f);
		modelShader.setFloat("pointLights[5].linear", 0.09);
		modelShader.setFloat("pointLights[5].quadratic", 0.032);

		// spotlight
		modelShader.setVec3("spotLight.position", camera.Position);
		modelShader.setVec3("spotLight.direction", camera.Front);
		modelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		modelShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		modelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		modelShader.setFloat("spotLight.constant", 1.0f);
		modelShader.setFloat("spotLight.linear", 0.09);
		modelShader.setFloat("spotLight.quadratic", 0.032);
		modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

		// trasformazione view/projection
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);

		// trasformazione world
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));	// sposto il modello al centro della scena
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// riduco la dimensione del modello
		modelShader.setMat4("model", model);
		ourModel.Draw(modelShader);

		// render cubi per lighting
		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);

		// ciclo necessario per far ruotare alcuni cubi 
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 6; i++)
		{
			model = glm::mat4(1.0f);

			if (i == 4) {
				model = glm::translate(model, glm::vec3(pointLightPositions[4].x, light5_mov_y, light5_mov_z));
			}
			else if (i == 5) {
				model = glm::translate(model, glm::vec3(light6_mov_x, pointLightPositions[5].y, light6_mov_z));
			}
			else
				model = glm::translate(model, pointLightPositions[i]);

			model = glm::scale(model, glm::vec3(0.2f)); // riduce la dimensione del cubo
			lightCubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// scambio buffers e interrogazione eventi input/output
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// terminazione
	glfwTerminate();
	return 0;
}

// funzione di callback per ridimensionamento finestra
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// funzione di callback per movimento mouse
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


// funzione callback per scroll rotella mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// funzione per gestione input
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}