#include <string>
#include <vector>
#include "windows.h"
#include "math.h"
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

glm::vec3 startPosition   = glm::vec3(30.0f, 2.0f, 0.0f);
glm::vec3 wiezaPosition   = glm::vec3(30.0f, 2.0f, 0.8f);

glm::mat4 tankObj;

Shader shaderProgram();
Model ourModel();
bool keys[1024];

// inicjujące wartosci obrócenia czołgu i jego wieży
GLfloat wiezaRotate = -30.0f;
GLfloat tankRotate = -30.0f;

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

	//modele i shader
	Shader shaderProgram = Shader("vshader.txt","fshader.txt");
	Model ourModel = Model("obiekty/tank5.obj");
	Model wiezaModel = Model("wieza.obj");

	//koordynaty wierzcholkow kafelkow
	GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f
    };
	GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //wrzucamy wierzchołki do bufora VBO
    // Ustawiamy atrybuty - 3 pierwsze bity na pozycje kafelkow pozostałe dwa na koordynaty tekstur
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2); 
	glBindVertexArray(0); // Odłączamy VAO po ustawieniach

	// TEKSTURY kafelków
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // Wszystkie następujące operacje będą dotyczyły załadowanych obiektów
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Ustawiamy zeby teskturowanie sie powtarzało
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Ustawiamy filtrowanie tektur
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	// ładujemy obrazek do tekstury
    unsigned char* image = SOIL_load_image("grass.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	// TEKSTURY drewna	
    GLuint textureWood;
    glGenTextures(1, &textureWood);
    glBindTexture(GL_TEXTURE_2D, textureWood); // Wszystkie następujące operacje będą dotyczyły załadowanych obiektów
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Ustawiamy zeby teskturowanie sie powtarzało
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Ustawiamy filtrowanie tektur
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width1, height1;
	// ładujemy obrazek do tekstury
    unsigned char* image1 = SOIL_load_image("wood.jpg", &width1, &height1, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image1);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	while (!glfwWindowShouldClose(window))
    {
        // sprawdza aktywacje eventow
        glfwPollEvents();
		// odpalamy funkcje poruszania i przeliczamy wszystkie wartosci
        do_movement();

        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, texture); //przypisanie tekstur - od tej pory wszystkie nowo stworzone obiekty będą miały tą teksture
		shaderProgram.Use();
		//ustawianie środowiska
		glm::mat4 view;
        glm::mat4 projection;
		//ustawienia widoku
		view = glm::lookAt(glm::vec3(0.0f, 0.0f,  3.0f), glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 1.0f,  0.0f));
		view = glm::translate(view, glm::vec3(-30.0f, -5.0f, -7.0f));
		view = glm::rotate(view, -45.3f, glm::vec3(1.0f, 0.0f, 0.0f));
		//ustawienia projekcji
        projection = glm::perspective(45.0f, 1200.0f / 1000.0f, 0.1f, 100.0f);
        //bierzemy uniformy MVC
        GLint modelLoc = glGetUniformLocation(shaderProgram.Program, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram.Program, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram.Program, "projection");
		
		//ładujemy ustawienia widoku i projekcji
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//budujemy ziemie
		glBindVertexArray(VAO);
		for( GLuint j=0; j<30; j++ ){
			for (GLuint i = 0; i < 60; i++)
			{
				glm::mat4 model;
				model = glm::translate(model, glm::vec3((float)i, (float)j, 0.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, textureWood);
		shaderProgram.Use();
		for( GLuint j=0; j<15; j++ ){
			
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(30.0f, (float)j, 0.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 6);
		
		}

		//resetujemy tekstury żeby nie miec juz trawy
		glBindTexture(GL_TEXTURE_2D, 0); 

		//tworzymy czołg
		glm::mat4 modelTank;
		modelTank = glm::translate(modelTank, startPosition);
		modelTank = glm::rotate(modelTank, -30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		modelTank = glm::rotate(modelTank, tankRotate, glm::vec3(0.0f, 1.0f, 0.0f));
        modelTank = glm::scale(modelTank, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTank));
        ourModel.Draw(shaderProgram);
		glBindVertexArray(0);

		//tworzymy wieże
		glm::mat4 modelWieza;
		modelWieza = glm::translate(modelWieza, wiezaPosition);
		modelWieza = glm::rotate(modelWieza, -30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		modelWieza = glm::rotate(modelWieza, wiezaRotate, glm::vec3(0.0f, 1.0f, 0.0f));
        modelWieza = glm::scale(modelWieza, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelWieza));
        wiezaModel.Draw(shaderProgram);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
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
	// poruszanie czołgu w przód i w tył
    if (keys[GLFW_KEY_W]){
		startPosition = glm::vec3(startPosition.x + 0.005f * cosf(tankRotate),  startPosition.y + 0.005f * sinf(tankRotate), startPosition.z );
		wiezaPosition = glm::vec3(startPosition.x,  startPosition.y, wiezaPosition.z);
	}
    if (keys[GLFW_KEY_S]){
		startPosition = glm::vec3(startPosition.x - 0.005f * cosf(tankRotate),  startPosition.y - 0.005f * sinf(tankRotate), startPosition.z );
		wiezaPosition = glm::vec3(startPosition.x,  startPosition.y, wiezaPosition.z);
	}
	// oblsuga skręcania czołgu (obracania)
    if (keys[GLFW_KEY_A]){
		tankRotate = tankRotate + 0.01f;
	}
    if (keys[GLFW_KEY_D]){
		tankRotate = tankRotate - 0.01f;
	}
	//obsługa kontroli obrozu wieży
	if (keys[GLFW_KEY_LEFT]) {
		wiezaRotate = wiezaRotate + 0.01f;
	}
	if (keys[GLFW_KEY_RIGHT]) {
		wiezaRotate = wiezaRotate - 0.01f;
	}
}