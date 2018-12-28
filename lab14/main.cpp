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
#include <map>
#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

unsigned char* image;

GLuint texture1;

int width = 0, height = 0;

int currentMode = 1;

GLuint Program1, Program2, Program3, Program4;

GLint Unif1, Unif2;
GLuint VBO, VAO, IBO;
GLuint currentShader;
//transform
glm::mat4 model, viewProjection;
glm::mat3 normaltr;

float viewPosition[]{ 0,0,-50 };

//light
float light_angle = 0, light_pos = 0, light_rad = 50;
float light[]{ 0, 5, 0 };
float ambient[]{ 0.2f, 0.2f,0.2f,1.0f };
float diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
float specular[]{ 1.0f,1.0f,1.0f,1.0f };
float attenuation[]{ 1.0f,0.0f,0.0f };

//tryig model
vector<glm::vec3> indexed_vertices;
vector<glm::vec2> indexed_uvs;
vector<glm::vec3> indexed_normals;
vector<unsigned short> indices;
GLuint vertexbuffer;
GLuint uvbuffer;
GLuint normalbuffer;
GLuint elementbuffer;

string objFilename = "watermelon.obj";
string objTextureFilename = "watermelon.jpg";
double obj_scale = 1.0;
float rotateX = 0, rotateY = 0, rotateZ = 0;


void makeTextureImage()
{
	texture1 = SOIL_load_OGL_texture
	(
		objTextureFilename.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void checkOpenGLerror() {
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		cout << "OpenGl error! - " << errCode << gluErrorString(errCode);
}

string readFileToString(const char* path)
{
	string res = "";
	ifstream file(path);
	string line;
	getline(file, res, '\0');
	while (getline(file, line))
	{
		res += "\n " + line;
	}
	return res;
}

void initShader1()
{
	string readed = readFileToString("vertex1.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readFileToString("fragment1.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program1 = glCreateProgram();
	glAttachShader(Program1, vShader);
	glAttachShader(Program1, fShader);
	glLinkProgram(Program1);

	int isSucc;
	glGetProgramiv(Program1, GL_LINK_STATUS, &isSucc);
	if (!isSucc)
	{
		cout << "err shader\n";
		return;
	}

	Unif1 = glGetUniformLocation(Program1, "objColor");
	if (Unif1 == -1)
	{
		cout << "err shader" << endl;
		checkOpenGLerror();
		return;
	}

	checkOpenGLerror();
}

void initShader2()
{
	string readed = readFileToString("vertex24.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readFileToString("fragment2.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program2 = glCreateProgram();
	glAttachShader(Program2, vShader);
	glAttachShader(Program2, fShader);
	glLinkProgram(Program2);

	int isSucc;
	glGetProgramiv(Program2, GL_LINK_STATUS, &isSucc);
	if (!isSucc)
	{
		cout << "err shader" << endl;
		return;
	}
	Unif2 = glGetUniformLocation(Program2, "objColor");
	if (Unif2 == -1)
	{
		cout << "err shader" << endl;
		checkOpenGLerror();
		return;
	}
	checkOpenGLerror();
}

void initShader3()
{
	string readed = readFileToString("vertex3.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readFileToString("fragment3.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program3 = glCreateProgram();
	glAttachShader(Program3, vShader);
	glAttachShader(Program3, fShader);
	glLinkProgram(Program3);

	int isSucc;
	glGetProgramiv(Program3, GL_LINK_STATUS, &isSucc);
	if (!isSucc)
	{
		cout << "err shader \n";
		return;
	}

	checkOpenGLerror();
}

void initShader4()
{
	string readed = readFileToString("vertex24.shader");
	const char* vsSource = readed.c_str();

	string readed2 = readFileToString("fragment4.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program4 = glCreateProgram();
	glAttachShader(Program4, vShader);
	glAttachShader(Program4, fShader);
	glLinkProgram(Program4);

	int isSucc;
	glGetProgramiv(Program4, GL_LINK_STATUS, &isSucc);
	if (!isSucc)
	{
		cout << "err shader \n";
		return;
	}

	checkOpenGLerror();
}

void freeShader()
{
	glUseProgram(0);
	glDeleteProgram(Program1);
	glDeleteProgram(Program2);
	glDeleteProgram(Program3);
	glDeleteProgram(Program4);
}

void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
}

struct PackedVertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const
	{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	};
};

bool getSimilarVertexIndex_fast(
	PackedVertex & packed,
	map<PackedVertex, unsigned short> & VertexToOutIndex,
	unsigned short & result
)
{
	map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end())
	{
		return false;
	}
	else
	{
		result = it->second;
		return true;
	}
}

void loadObjFromFile(const string & path, vector<glm::vec3> & out_vertices, vector<glm::vec2> & out_uvs, vector<glm::vec3> & out_normals)
{
	vector<unsigned int> vertex_indices, uv_indices, normal_indices;
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;

	ifstream infile(path);
	string line;
	while (getline(infile, line))
	{
		stringstream ss(line);
		string lineHeader;
		getline(ss, lineHeader, ' ');
		if (lineHeader == "v")
		{
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;

			vertex.x *= obj_scale;
			vertex.y *= obj_scale;
			vertex.z *= obj_scale;
			temp_vertices.push_back(vertex);
		}
		else if (lineHeader == "vt")
		{
			glm::vec2 uv;
			ss >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (lineHeader == "vn")
		{
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (lineHeader == "f")
		{
			unsigned int vertex_index, uv_index, normal_index;
			char slash;
			while (ss >> vertex_index >> slash >> uv_index >> slash >> normal_index)
			{
				vertex_indices.push_back(vertex_index);
				uv_indices.push_back(uv_index);
				normal_indices.push_back(normal_index);
			}
		}
	}

	for (unsigned int i = 0; i < vertex_indices.size(); i++)
	{
		unsigned int vertexIndex = vertex_indices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);

		unsigned int uvIndex = uv_indices[i];
		glm::vec2 uv = temp_uvs.at(uvIndex - 1);
		out_uvs.push_back(uv);

		unsigned int normalIndex = normal_indices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
}

void indexVBO(
	vector<glm::vec3> & in_vertices,
	vector<glm::vec2> & in_uvs,
	vector<glm::vec3> & in_normals,

	vector<unsigned short> & out_indices,
	vector<glm::vec3> & out_vertices,
	vector<glm::vec2> & out_uvs,
	vector<glm::vec3> & out_normals
)
{
	map<PackedVertex, unsigned short> VertexToOutIndex;

	for (unsigned int i = 0; i < in_vertices.size(); i++)
	{
		PackedVertex packed = { in_vertices[i], in_uvs[i], in_normals[i] };

		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found)
		{
			out_indices.push_back(index);
		}
		else
		{
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			unsigned short newindex = (unsigned short)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

void initBuffers()
{

	vector<glm::vec3> normals;
	vector<glm::vec2> uvs;
	vector<glm::vec3> vertices;

	loadObjFromFile(objFilename.c_str(), vertices, uvs, normals);

	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &elementbuffer);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void freeBuffers()
{
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

const double pi = 3.14159265358979323846;
void recountLightPos()
{
	double x = light_rad * glm::cos(light_angle / 180 * pi);
	double z = light_rad * glm::sin(light_angle / 180 * pi);
	light[0] = x;
	light[1] = light_pos;
	light[2] = z;
}

void setTransform()
{

	GLint s_model = glGetUniformLocation(currentShader, "transform.model");
	GLint s_proj = glGetUniformLocation(currentShader, "transform.viewProjection");
	GLint s_normal = glGetUniformLocation(currentShader, "transform.normal");
	GLint s_view = glGetUniformLocation(currentShader, "transform.viewPosition");

	glUniformMatrix4fv(s_model, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(s_proj, 1, GL_FALSE, &viewProjection[0][0]);
	glUniformMatrix3fv(s_normal, 1, GL_FALSE, &normaltr[0][0]);
	glUniform3fv(s_view, 1, viewPosition);
}

void setPointLight()
{

	GLint s_position = glGetUniformLocation(currentShader, "light.position");
	GLint s_ambient = glGetUniformLocation(currentShader, "light.ambient");
	GLint s_diffuse = glGetUniformLocation(currentShader, "light.diffuse");
	GLint s_specular = glGetUniformLocation(currentShader, "light.specular");
	GLint s_attenuation = glGetUniformLocation(currentShader, "light.attenuation");

	glUniform4fv(s_position, 1, light);
	glUniform4fv(s_ambient, 1, ambient);
	glUniform4fv(s_diffuse, 1, diffuse);
	glUniform4fv(s_specular, 1, specular);
	glUniform3fv(s_attenuation, 1, attenuation);
}

void setMaterial(float* m_ambient, float* m_diffuse, float* m_specular, float* m_emission, float m_shiness)
{

	GLint s_ambient = glGetUniformLocation(currentShader, "material.ambient");
	GLint s_diffuse = glGetUniformLocation(currentShader, "material.diffuse");
	GLint s_specular = glGetUniformLocation(currentShader, "material.specular");
	GLint s_emission = glGetUniformLocation(currentShader, "material.emission");
	GLint s_shiness = glGetUniformLocation(currentShader, "material.shiness");

	glUniform4fv(s_ambient, 1, m_ambient);
	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform4fv(s_emission, 1, m_emission);
	glUniform1f(s_shiness, m_shiness);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0, 0, 1));

	viewProjection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	viewProjection *= glm::lookAt(
		glm::vec3(viewPosition[0], viewPosition[1], viewPosition[2]),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	normaltr = glm::transpose(glm::inverse(model));

	float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	if (currentMode == 1 || currentMode == 2)
	{
		if (currentMode == 1)
		{
			glUseProgram(Program1);
			glUniform4fv(Unif1, 1, red);
		}
		else
		{
			glUseProgram(Program2);
			glUniform4fv(Unif2, 1, red);
		}
	}
	else
	{
		GLint Unif_tex = glGetUniformLocation(Program3, "myTexture");
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture1);

		checkOpenGLerror();
		glUseProgram(currentShader);
	}

	setTransform();
	setPointLight();
	float m_ambient[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
	float m_specular[]{ 0.2f,0.2f,0.2f,1.0f };
	float m_emission[]{ 0.0f,0.0f,0.0f,1.0f };
	float m_shiness = 0;
	setMaterial(m_ambient, m_diffuse, m_specular, m_emission, m_shiness);

	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	checkOpenGLerror();
	if (glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1: currentMode = 1;
		currentShader = Program1;
		break;
	case GLUT_KEY_F2:
		currentMode = 2;
		currentShader = Program2;
		break;
	case GLUT_KEY_F3:
		currentMode = 3;
		currentShader = Program3;
		break;
	case GLUT_KEY_F4:
		currentMode = 4;
		currentShader = Program4;
		break;
	case GLUT_KEY_UP: light_pos += 0.5; break;
	case GLUT_KEY_DOWN: light_pos -= 0.5; break;
	case GLUT_KEY_RIGHT: light_angle -= 3; break;
	case GLUT_KEY_LEFT: light_angle += 3; break;
	case GLUT_KEY_PAGE_UP: light_rad -= 0.5; break;
	case GLUT_KEY_PAGE_DOWN: light_rad += 0.5; break;
	}
	recountLightPos();
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		rotateX -= 1;
		break;
	case '2':
		rotateX += 1;
		break;
	case '3':
		rotateY -= 1;
		break;
	case '4':
		rotateY += 1;
		break;
	case '5':
		rotateZ -= 1;
		break;
	case '6':
		rotateZ += 1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(700, 700);

	glutInitWindowPosition(30, 30);
	glutCreateWindow("lab14");

	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		cout << "Error: " << glewGetErrorString(glew_status) << "\n";   
		return 1;
	}

	initBuffers();

	makeTextureImage();

	initShader1();
	initShader2();
	initShader3();
	initShader4();
	currentShader = Program1;


	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);

	recountLightPos();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	freeShader();
	freeBuffers();
}