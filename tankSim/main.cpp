#include "windows.h"
#include "SOIL.h"
#include "GL/glew.h"
#include "GL/glut.h"
#include <stdio.h>
#include "shaderprogram.h"


/*
void assignVBOtoAttribute(GLuint bufVBO, int variableSize, GLfloat vertices) {
	glBindBuffer(GL_ARRAY_BUFFER, bufVBO); //przypisujemy mu typ
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //ladujemy do niego nasze wierzcholki 
	glEnableVertexAttribArray(0); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej location
	glVertexAttribPointer(0, variableSize, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location maj¹ byæ brane z aktywnego VBO
}*/

GLuint VBO, VAO;
// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position, 1.0);\n"
    "ourColor = color;\n"
    "}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "in vec3 ourColor;\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(ourColor, 1.0f);\n"
    "}\n\0";
/*
void prepare(){
	
	
		// tutaj renderowanie

	 GLfloat vertices[] = {
        // Positions         // Colors
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
       -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
        0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top 
    };

	 GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO

}*/

void displayFrame() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	Shader shaderProgram = Shader("vshader.txt","fshader.txt");

	//Kompilacja vertex shader
	
		// tutaj renderowanie

	//koordynaty wierzcholkow
	 GLfloat vertices[] = {
        // Positions         // Colors
        0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,	 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
    };

	 GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

	 GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2); 

	glBindVertexArray(0); // Unbind VAO

	// tekstury
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

    glBindVertexArray(0); // Unbind VAO
	glBindTexture(GL_TEXTURE_2D, texture);
	shaderProgram.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); 
	glutSwapBuffers();
}


void initializeGLUT(int* pargc, char** argv) {
	glutInit(pargc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(800,800);
	glutCreateWindow("Program OpenGL");
	glutDisplayFunc(displayFrame);
}

void initializeGLEW() {
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: Nie uda³o siê zainicjowaæ biblioteki GLEW. */
		//fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}
void initOpenGL() {
	
	
}


int main (int argc, char** argv) {
	initializeGLUT(&argc,argv);
	initializeGLEW();
	initOpenGL();
	//prepare();
	//Kod inicjuj¹cy tutaj
	glutMainLoop();
	//Kod zwalniaj¹cy zasoby tutaj
}
