#include<Windows.h>       
#include<stdio.h>
#include "GL/glew.h"
#include<gl/GL.h>   // GL.h header file    
#include<gl/GLU.h> // GLU.h header file    
#include<gl/glut.h>  // glut.h header file from freeglut\include\GL folder    
#include<conio.h>
#include<math.h>
#include<string>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

unsigned char* image;
GLuint texture1, texture2;
int width = 0, height = 0;
int task = 1;
GLuint task1Program, task2Program, task3Program;
GLint Unif_tex_1, Unif_tex_2, Unif_coef;
GLuint VBO, VAO,IBO;
vector<GLushort> indices;
float coef = 0.5f;

void makeTextureImage() {
	texture1 = SOIL_load_OGL_texture(
		"cat2.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	texture2 = SOIL_load_OGL_texture(
		"rainbow.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

void checkOpenGLerror() {
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		cout << "OpenGl error! - " << gluErrorString(errCode);
}

string readFile(const char* path) {
	string res = "";
	ifstream file(path);
	string line;
	getline(file, res, '\0');
	while (getline(file, line))
		res += "\n " + line;
	
	return res;
}

void createTask1() {
	string readed = readFile("vertex.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readFile("fragment.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	task1Program = glCreateProgram();
	glAttachShader(task1Program, vShader);
	glAttachShader(task1Program, fShader);
	glLinkProgram(task1Program);

	int linked;
	glGetProgramiv(task1Program, GL_LINK_STATUS, &linked);
	if (!linked) {
		cout << "error attach shaders \n";
		return;
	}

	checkOpenGLerror();
}

void createTask2() {
	std::string readed = readFile("vertex.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readFile("fragment2.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	task2Program = glCreateProgram();
	glAttachShader(task2Program, vShader);
	glAttachShader(task2Program, fShader);
	glLinkProgram(task2Program);

	int linked;
	glGetProgramiv(task2Program, GL_LINK_STATUS, &linked);
	if (!linked) {
		cout << "error attach shaders" << endl;
		return;
	}
}

void createTask3() {
	string readed = readFile("vertex.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readFile("fragment3.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	task3Program = glCreateProgram();
	glAttachShader(task3Program, vShader);
	glAttachShader(task3Program, fShader);
	glLinkProgram(task3Program);

	int linked;
	glGetProgramiv(task3Program, GL_LINK_STATUS, &linked);
	if (!linked) {
		cout << "error attach shaders" << endl;
		return;
	}

	const char* tex1_name = "ourTexture1";
	Unif_tex_1 = glGetUniformLocation(task3Program, tex1_name);
	if (Unif_tex_1 == -1) {
		cout << "could not bind uniform " << tex1_name << endl;
		return;
	}
	const char* tex2_name = "ourTexture2";
	Unif_tex_2 = glGetUniformLocation(task3Program, tex2_name);
	if (Unif_tex_2 == -1) {
		cout << "could not bind uniform " << tex2_name << endl;
		return;
	}

	const char* coef_name = "coef";
	Unif_coef = glGetUniformLocation(task3Program, coef_name);
	if (Unif_coef == -1) {
		cout << "could not bind uniform " << coef_name << endl;
		return;
	}
	checkOpenGLerror();
}

void removeShaders() {
	glUseProgram(0);
	glDeleteProgram(task1Program);
	glDeleteProgram(task2Program);
	glDeleteProgram(task3Program);
}

void init(void) {
	glClearColor(0.4, 0.4, 0.4, 0.0);
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h) {
	width = w; height = h;
	glViewport(0, 0, w, h);
}

void initBuffers() {
	GLfloat vertices[] = {
		// Позиции          // Цвета             // Текстурные координаты
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // Нижний левый
		-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
	};

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	
	if (task == 1) {
		glUseProgram(task1Program);
		glBindTexture(GL_TEXTURE_2D, texture1);
	}
	else if (task == 2) {
		glUseProgram(task2Program);
		glBindTexture(GL_TEXTURE_2D, texture1);
	}
	else if (task == 3) {
		glUseProgram(task3Program);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(Unif_tex_1, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(Unif_tex_2, 1);
		glUniform1f(Unif_coef, coef);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, 4,GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	
	glUseProgram(0);
	checkOpenGLerror();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1: task = 1;
		break;
	case GLUT_KEY_F2:
		task = 2;
		break;
	case GLUT_KEY_F3:
		task = 3;
		break;
	}
	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '+': coef += 0.1f;
		break;
	case '-': coef -= 0.1f;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Lab 13.3");

	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		cout << "Error: " << glewGetErrorString(glew_status) << endl;   
		return 1;
	}
	if (!GLEW_VERSION_2_0) {
		cout << "No support for OpenGL 2.0 found" << endl;
		return 1;
	}

	initBuffers();
	makeTextureImage();
	createTask1();
	createTask2();
	createTask3();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();

	removeShaders();
}