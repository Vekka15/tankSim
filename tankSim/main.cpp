#include <string>
#include <vector>
#include "windows.h"
#include "SOIL.h"
#include "GL/glew.h"
#include "GL/glut.h"
// GLFW
#include "glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <stdio.h>
#include "shaderprogram.h"
#include "Model.h"
#include "Mesh.h"
#include "Camera.h"
const GLuint WIDTH = 1200, HEIGHT = 1000;

GLuint VBO, VAO1;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();

// Camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
//bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

glm::vec3 startPosition   = glm::vec3(2.0f, 0.0f, 0.0f);
glm::vec3 wiezaPosition   = glm::vec3(2.0f, 0.0f, 0.8f);

glm::mat4 tankObj;

Shader shaderProgram();
Model ourModel();
bool keys[1024];

int main (int argc, char** argv) {
	// Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST);

	Shader shaderProgram = Shader("vshader.txt","fshader.txt");
		Model ourModel = Model("obiekty/tank5.obj");
		Model wiezaModel = Model("wieza.obj");
		// tutaj renderowanie
	
	//koordynaty wierzcholkow
	GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f
    };
	 glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
		 glm::vec3( 1.0f,  0.0f, -0.0f),
		glm::vec3( 2.0f,  0.0f, -0.0f)
    };
	 GLuint VBO, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
   // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2); 
	glBindVertexArray(0); // Unbind VAO1
		// TEKSTURY
	   // Load and create a texture 
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
    unsigned char* image = SOIL_load_image("grass.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	while (!glfwWindowShouldClose(window))
    {
				

			// Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        do_movement();
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(0); // Unbind VAO1
		glBindTexture(GL_TEXTURE_2D, texture); //przypisanie tekstur
		shaderProgram.Use();
		glm::mat4 view;
        glm::mat4 projection;
	//	model = glm::rotate(model, 40.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		        view = glm::translate(view, glm::vec3(-5.0f, -3.0f, -7.0f));
		view = glm::rotate(view, -45.3f, glm::vec3(1.0f, 0.0f, 0.0f));
		//view = camera.GetViewMatrix();
        projection = glm::perspective(45.0f, 1200.0f / 1000.0f, 0.1f, 100.0f);
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(shaderProgram.Program, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram.Program, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram.Program, "projection");
	//		 glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(VAO1);
		for( GLuint j=0; j<10; j++ ){
			for (GLuint i = 0; i < 10; i++)
			{
				// Calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model;
				model = glm::translate(model, glm::vec3((float)i, (float)j, 0.0f));
				//model = glm::rotate(model, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			
		glm::mat4 model;
		model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, (float)j, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glm::mat4 model2;
		model2 = glm::rotate(model2, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model2 = glm::translate(model2, glm::vec3(10.0f, 0.0f, (float)j));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glBindVertexArray(0);
		  glBindTexture(GL_TEXTURE_2D, 0); 

			glm::mat4 modelTank;
			modelTank = glm::translate(modelTank, startPosition);
		 modelTank = glm::rotate(modelTank, -30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        modelTank = glm::scale(modelTank, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTank));
		tankObj = modelTank;
        ourModel.Draw(shaderProgram);
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
			glm::mat4 modelWieza;
			modelWieza = glm::translate(modelWieza, wiezaPosition);
		 modelWieza = glm::rotate(modelWieza, -30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        modelWieza = glm::scale(modelWieza, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelWieza));
        wiezaModel.Draw(shaderProgram);
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		  glBindTexture(GL_TEXTURE_2D, 0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
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

void do_movement()
{
    // Camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W]){
    //    cameraPos += cameraSpeed * cameraFront;
		//modyfikacja wektora startPosition na polu Z (+ lub -)
		//	printf("%d", startPosition);
//		cameraPos = glm::vec3(cameraPos.x,  cameraPos.y, cameraPos.z - 5.0f);
			startPosition = glm::vec3(startPosition.x,  min(10.0f, (startPosition.y + 0.1f)), startPosition.z);
			wiezaPosition = glm::vec3(wiezaPosition.x,  min(10.0f, (wiezaPosition.y + 0.1f)), wiezaPosition.z);
	}
    if (keys[GLFW_KEY_S]){
		//modyfikacja wektora startPosition na polu Z (+ lub -)
//        cameraPos -= cameraSpeed * cameraFront;
	startPosition = glm::vec3(startPosition.x,  startPosition.y - 0.1f, startPosition.z);
	wiezaPosition = glm::vec3(wiezaPosition.x,  wiezaPosition.y - 0.1f, wiezaPosition.z);
	}
    if (keys[GLFW_KEY_A]){
		//modyfikacja wektora startPosition na polu X (+ lub -)
        startPosition = glm::vec3(startPosition.x - 0.1f, startPosition.y, 0.0f);
	}
    if (keys[GLFW_KEY_D]){
		//modyfikacja wektora startPosition na polu X (+ lub -)
	//w		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        startPosition = glm::vec3(startPosition.x + 0.1f, startPosition.y, 0.0f);
	}
}
