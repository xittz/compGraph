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

int width = 0, height = 0, task = 1;
GLuint Program, vProgram, hProgram;

GLint Attrib_vertex, Unif_color;
GLint Unif_v_color1, Unif_v_color2;
GLint Unif_h_color1, Unif_h_color2;
GLint Unif_v_lines, Unif_h_lines;

float v_color1[]{ 1.0f, 1.0f, 1.0f, 1.0f };
float v_color2[]{ 0.0f, 0.0f, 1.0f, 1.0f };
float h_color1[]{ 0.0f, 1.0f, 0.0f, 1.0f };
float h_color2[]{ 0.0f, 0.0f, 1.0f, 1.0f };

int ver_line = 50;
int hor_line = 50;

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

	checkOpenGLerror();
}

void initVertShader() {
	string readed = readFile("vertex.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readFile("fragment_vert.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	vProgram = glCreateProgram();
	glAttachShader(vProgram, vShader);
	glAttachShader(vProgram, fShader);
	glLinkProgram(vProgram);

	int linked;
	glGetProgramiv(vProgram, GL_LINK_STATUS, &linked);
	if (!linked) {
		cout << "error attach shaders" << endl;
		return;
	}

	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(vProgram, attr_name);
	if (Attrib_vertex == -1) {
		cout << "could not bind attributes " << attr_name << endl;
		return;
	}

	const char* color1_name = "v_color1";
	Unif_v_color1 = glGetUniformLocation(vProgram, color1_name);
	if (Unif_v_color1 == -1) {
		cout << "could not bind uniform " << color1_name << endl;
		return;
	}

	const char* color2_name = "v_color2";
	Unif_v_color2 = glGetUniformLocation(vProgram, color2_name);
	if (Unif_v_color2 == -1) {
		cout << "could not bind uniform " << color2_name << endl;
		return;
	}

	const char* lines_name = "lines";
	Unif_v_lines = glGetUniformLocation(vProgram, lines_name);
	if (Unif_v_lines == -1) {
		cout << "could not bind uniform " << lines_name << endl;
		return;
	}
	checkOpenGLerror();
}

void initHorShader() {
	string readed = readFile("vertex.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readFile("fragment_hor.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	hProgram = glCreateProgram();
	glAttachShader(hProgram, vShader);
	glAttachShader(hProgram, fShader);
	glLinkProgram(hProgram);

	int linked;
	glGetProgramiv(hProgram, GL_LINK_STATUS, &linked);
	if (!linked) {
		cout << "error attach shaders \n";
		return;
	}

	const char* attr_name = "coord";
	Attrib_vertex = glGetAttribLocation(hProgram, attr_name);
	if (Attrib_vertex == -1) {
		cout << "could not bind attrib " << attr_name << endl;
		return;
	}

	const char* color1_name = "h_color1";
	Unif_h_color1 = glGetUniformLocation(hProgram, color1_name);
	if (Unif_h_color1 == -1) {
		cout << "could not bind uniform " << color1_name << endl;
		return;
	}

	const char* color2_name = "h_color2";
	Unif_h_color2 = glGetUniformLocation(hProgram, color2_name);
	if (Unif_h_color2 == -1) {
		cout << "could not bind uniform " << color2_name << endl;
		return;
	}

	const char* lines_name = "lines";
	Unif_h_lines = glGetUniformLocation(hProgram, lines_name);
	if (Unif_h_lines == -1) {
		cout << "could not bind uniform " << lines_name << endl;
		return;
	}
	checkOpenGLerror();
}

void removeShaders() {
	glUseProgram(0);
	glDeleteProgram(Program);
	glDeleteProgram(vProgram);
	glDeleteProgram(hProgram);
}

void init(void) {
	glClearColor(0.4, 0.4, 0.4, 0.0);
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h) {
	width = w; height = h;
	glViewport(0, 0, w, h);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if (task == 1) {
		glUseProgram(Program);
		static float red[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
		glUniform4fv(Unif_color, 1, red);
	}
	else if (task == 2) {
		glUseProgram(vProgram);
		glUniform4fv(Unif_v_color1, 1, v_color1);
		glUniform4fv(Unif_v_color2, 1, v_color2);
		glUniform1i(Unif_v_lines, ver_line);
	}
   else if (task == 3) {
		glUseProgram(hProgram);
		glUniform4fv(Unif_h_color1, 1, h_color1);
		glUniform4fv(Unif_h_color2, 1, h_color2);
		glUniform1i(Unif_h_lines, hor_line);
	}

	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.5f, -0.5f);
	glColor3f(0.0, 1.0, 0.0); glVertex2f(-0.5f, 0.5f);
	glColor3f(0.0, 0.0, 1.0); glVertex2f(0.5f, 0.5f);
	glColor3f(1.0, 1.0, 1.0); glVertex2f(0.5f, -0.5f);  
	glEnd();

	glFlush();
	glUseProgram(0);
	checkOpenGLerror();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1': task = 1;
		break;
	case '2':
		task = 2;
		break;
	case '3':
		task = 3;
		break;
	case '=':
		if (task == 2)
			++ver_line;
		else ++hor_line;
		break;
	case '-':
		if (task == 2)
			--ver_line;
		else --hor_line;
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Lab 13");

	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		cout << "Error: " << glewGetErrorString(glew_status) << endl;
		return 1;
	}

	if (!GLEW_VERSION_2_0) {
		cout << "No support for OpenGL 2.0 found\n";
		return 1;
	}

	initShader();
	initVertShader();
	initHorShader();
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	removeShaders();
}