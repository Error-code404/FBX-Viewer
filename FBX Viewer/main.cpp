#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "ShaderLoader.h"
#include "Camera.h"
//#include "FBX_Loader.h"
#include "FBXModel.h"

//FBX_loader loader("Cube.FBX");
//FBXModel cube("Cube.FBX");

//callback functions declarations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

//window properties
GLuint screenWidth = 1366, screenHeight = 768;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
	//std::cout << cube.numIndices << "\n";
	//std::cout << cube.indices[2];
	//for (auto c : cube.indices)
		//std::cout << c << " ";
	//std::cout<< "number i verts in cube model" << cube.index << "\n";
	//loader.LoadNode(loader.lScene->GetRootNode());
	//loader.loadNode();
	//loader.loadMesh();
	//std::cout << "polygon count:" << loader.pMesh->GetPolygonCount() << "\n";
	//std::cout << "polygon vertex:" << loader.pMesh->GetPolygonVertexCount() <<"\n";
	//std::cout << "vertices count:" << loader.pMesh->GetControlPoints() << "\n";
	//std::cout <<loader.pMesh->IsTriangleMesh();
	//std::cout << loader.meshes.size();
	//initialize
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "FBX Viewer", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	////Data
	//GLfloat verts[] = { -0.5f, -0.5f, 0.0f,
	//					0.0f, 0.5f, 0.0f,
	//					0.5f, -0.5f, 0.0f };

	//Shaders and program

	ShaderLoader Program("VS.frag", "FS.frag");
	FBXModel Model("Cube.FBX", Program);

	//VBO VAO
	/*GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube.vertices), &cube.vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) *cube.numIndices, cube.indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
*/
	
	//std::cout << " number of total vertices: " << Model.numVerts << "\n";
	//std::cout << " number of last mesh vertices: " << Model.numIndices << "\n";
	//main loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		Do_Movement();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Program.Use();

		// Create camera transformation
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
		glm::mat4 model;

		//pozycja modelu
		glm::vec3 modelPos = glm::vec3(-20.0f, -100.0f, -20.0f);
		model = glm::translate(model, modelPos);
		//model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(projection));

		//cube.program->Use();
		//Program.Use();
		//glBindVertexArray(VAO);
		Model.Draw();
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, cube.numIndices, GL_UNSIGNED_INT, 0);

		////glDrawArrays(GL_POINTS, 0, cube.numVerts);
	
		//glBindVertexArray(0);
		glfwSwapBuffers(window);
	}


	//shutdown
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
//callback functions definitions
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
