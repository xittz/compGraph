#include<Windows.h>     
#include<stdio.h>
#include "GL/glew.h"
#include<gl/GL.h>   
#include<gl/GLU.h>    
#include<gl/glut.h>    
#include<conio.h>
#include<math.h>
#include<string>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

using namespace std;

int width = 0, height = 0, axis = 0;
float rotateX = 90.0f, rotateY = 90.0f, rotateZ = 90.0f;
float scaleX = 1, scaleY = 1, scaleZ = 1;

GLuint Program;
GLint Attrib_vertex, Unif_color, Unif_matrix, Unif_proj;


int task = 0;
void checkOpenGLerror() {
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		std::cout << "OpenGl error! - " << gluErrorString(errCode);
}

std::string readFile(const char* path) {
	string res = "";
	ifstream file(path);
	string line;
	getline(file, res, '\0');
	while (getline(file, line))
		res += "\n " +line;
	return res;
}

void initShader() {
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

	Program = glCreateProgram();
	glAttachShader(Program, vShader);
	glAttachShader(Program, fShader);
	glLinkProgram(Program);

	int linked;
	glGetProgramiv(Program, GL_LINK_STATUS, &linked);
	if (!linked) {
		cout << "error attach shaders \n";
		return;
	}

	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(Program, attr_name);
	if (Attrib_vertex == -1) {
		cout << "could not bind attrib " << attr_name << endl;
		return;
	}

	const char* color_name = "color";
	Unif_color = glGetUniformLocation(Program, color_name);
	if (Unif_color == -1) {
		cout << "could not bind uniform " << color_name << endl;
		return;
	}

	const char* matrix_name = "matrix";
	Unif_matrix = glGetUniformLocation(Program, matrix_name);
	if (Unif_matrix == -1) {
		cout << "could not bind uniform " << matrix_name << endl;
		return;
	}

	const char* proj_name = "projection";
	Unif_proj = glGetUniformLocation(Program, proj_name);
	if (Unif_proj == -1) {
		std::cout << "could not bind uniform " << proj_name << endl;
		return;
	}
	checkOpenGLerror();
}	 

void clearShaders() {
	glUseProgram(0);
	glDeleteProgram(Program);
}

void init(void) {
	glClearColor(0.4, 0.4, 0.4, 0.0);
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h) {
	width = w; height = h;
	glViewport(0, 0, w, h);
}

glm::mat4 rotate_matrix() {
	float sin = glm::sin(rotateX*3.14 / 180);
	float cos = glm::sin(rotateX*3.14 / 180);
	
	//x
	glm::mat4 m1 = { 1.0f,   0.0f,     0.0f,   0.0f,
					0.0f,  cos,   -sin,  0.0f,
					0.0f,  sin,  cos,  0.0f,
					 0.0f,   0.0f,     0.0f,   1.0f };

	//y

	sin = glm::sin(rotateY*3.14 / 180);
	cos = glm::sin(rotateY*3.14 / 180);

	glm::mat4 m2 = { cos, 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin, 0.0f, cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	
	//z

	sin = glm::sin(rotateZ*3.14 / 180);
	cos = glm::sin(rotateZ*3.14 / 180);
	glm::mat4 m3 = { cos, sin, 0.0f, 0.0f,
		-sin, cos, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	
	glm::mat4 res = glm::matrixCompMult(m2, m1);
	return m1;
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glUseProgram(Program);
	static float red[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glUniform4fv(Unif_color, 1, red);

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 5, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 MVP = Projection * View;
	glUniformMatrix4fv(Unif_proj, 1, GL_FALSE, &MVP[0][0]);

	glm::mat4 S = { scaleX, 0.0f,0.0f, 0.0f,
					0.0f , scaleY, 0.0f, 0.0f ,
					0.0f ,0.0f , scaleZ, 0.0f ,
						0.0f ,0.0f ,0.0f, 1.0f };
	float a = rotateX * 3.14f / 180.0f;
	glm::mat4 R = rotate_matrix();

	glm::mat4 Matrix = glm::matrixCompMult(S, R);
	
	glUniformMatrix4fv(Unif_matrix, 1, GL_FALSE, &Matrix[0][0]);
	glutSolidCube(1);
	glFlush();

	glUseProgram(0);

	checkOpenGLerror();

	glutSwapBuffers();
}

void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1: task = 0;
		break;
	case GLUT_KEY_F2: task = 1;
		break;
	}
}

void keyboard(unsigned char key, int x, int y) {
	if (task == 0) {
		switch (key) {
		case '1': scaleX += 0.1;
			break;
		case '2': scaleX -= 0.1;
			break;
		case '3': scaleY += 0.1;
			break;
		case '4': scaleY -= 0.1;
			break;
		case '5': scaleZ += 0.1;
			break;
		case '6': scaleZ -= 0.1;
			break;
		}
	}
	else
		switch (key) {
		case '1': rotateX += 5;
			break;
		case '2': rotateX -= 5;
			break;
		case '3': rotateY += 5;
			break;
		case '4': rotateY -= 5;
			break;
		case '5': rotateZ += 5;
			break;
		case '6': rotateZ -= 5;
			break;
		}
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Lab 12");
	
	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		std::cout << "Error: " << glewGetErrorString(glew_status) << "\n";   return 1;
	}
	if (!GLEW_VERSION_2_0) { 
		std::cout << "No support for OpenGL 2.0 found\n";
		return 1; 
	}

	initShader();

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMainLoop();

	clearShaders();
}