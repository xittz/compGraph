#include<Windows.h>    
// first include Windows.h header file which is required    
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
#include <map>
#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

int width = 0, height = 0, task = 1;
float viewPosition[]{ 0,0,-50 };

//transform
glm::mat4 model, viewProjection;
glm::mat3 normaltr;


//light
float lightAngle = 0, lightPosition = 0, light_rad = 50;
float light[]{ 0, 5, 0 };
float ambient[]{ 0.2f, 0.2f,0.2f,1.0f };
float diffuse[]{ 1.0f,1.0f,1.0f,1.0f };
float specular[]{ 1.0f,1.0f,1.0f,1.0f };
float attenuation[]{ 1.0f,0.0f,0.0f };

GLuint LambertProgram, BlinnProgram, MinnaertProgram, ToonProgram, AmiGoochProgram, RimProgram, VAO, shader_program;
GLuint texture1, vertexBuffer, uvBuffer, normalBuffer, elementBuffer;

//tryig model
vector<glm::vec3> indexed_vertices;
vector<glm::vec2> indexed_uvs;
vector<glm::vec3> indexed_normals;
vector<unsigned short> indices;


string obj = "watermelon.obj";
string texture = "watermelon.jpg";

double scale = 1;
float rotateX = 0, rotateY = 180, rotateZ = 0;

unsigned char* image;


void makeTextureImage() {
	texture1 = SOIL_load_OGL_texture(
		texture.c_str(),
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

string readFile(const char* path) {
	std::string res = "";
	std::ifstream file(path);
	std::string line;
	getline(file, res, '\0');
	while (getline(file, line))
		res += "\n " + line;

	return res;
}

void createLambert() {
	string readed = readFile("vertex1.shader");
	string readed2 = readFile("fragment1.shader");
	const char* vsSource = readed.c_str();
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	LambertProgram = glCreateProgram();
	glAttachShader(LambertProgram, vShader);
	glAttachShader(LambertProgram, fShader);
	glLinkProgram(LambertProgram);

	int linked;
	glGetProgramiv(LambertProgram, GL_LINK_STATUS, &linked);
	
	if (!linked) {
		cout << "Couldn't attach shader" << endl;
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(LambertProgram, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void createBlinn() {
	string readed = readFile("vertex2.shader");
	string readed2 = readFile("fragment2.shader");
	const char* vsSource = readed.c_str();
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	BlinnProgram = glCreateProgram();
	glAttachShader(BlinnProgram, vShader);
	glAttachShader(BlinnProgram, fShader);
	glLinkProgram(BlinnProgram);

	int linked;
	glGetProgramiv(LambertProgram, GL_LINK_STATUS, &linked);

	if (!linked) {
		cout << "Couldn't attach shader" << endl;
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(LambertProgram, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void createMinnaert() {
	string readed = readFile("vertex3.shader");
	string readed2 = readFile("fragment3.shader");
	const char* vsSource = readed.c_str();
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	MinnaertProgram = glCreateProgram();
	glAttachShader(MinnaertProgram, vShader);
	glAttachShader(MinnaertProgram, fShader);
	glLinkProgram(MinnaertProgram);

	int linked;
	glGetProgramiv(LambertProgram, GL_LINK_STATUS, &linked);

	if (!linked) {
		cout << "Couldn't attach shader" << endl;
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(LambertProgram, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void createToon() {
	string readed = readFile("vertex4.shader");
	string readed2 = readFile("fragment4.shader");
	const char* vsSource = readed.c_str();
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	ToonProgram = glCreateProgram();
	glAttachShader(ToonProgram, vShader);
	glAttachShader(ToonProgram, fShader);
	glLinkProgram(ToonProgram);

	int linked;
	glGetProgramiv(LambertProgram, GL_LINK_STATUS, &linked);

	if (!linked) {
		cout << "Couldn't attach shader" << endl;
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(LambertProgram, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void createAmiGooch() {
	string readed = readFile("vertex5.shader");
	string readed2 = readFile("fragment5.shader");
	const char* vsSource = readed.c_str();
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	AmiGoochProgram = glCreateProgram();
	glAttachShader(AmiGoochProgram, vShader);
	glAttachShader(AmiGoochProgram, fShader);
	glLinkProgram(AmiGoochProgram);

	int linked;
	glGetProgramiv(LambertProgram, GL_LINK_STATUS, &linked);

	if (!linked) {
		cout << "Couldn't attach shader" << endl;
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(LambertProgram, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void createRim() {
	
	string readed = readFile("vertex6.shader");
	string readed2 = readFile("fragment6.shader");
	const char* vsSource = readed.c_str();
	const char* fsSource = readed2.c_str();
	
	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	RimProgram = glCreateProgram();
	glAttachShader(RimProgram, vShader);
	glAttachShader(RimProgram, fShader);
	glLinkProgram(RimProgram);

	int linked;
	glGetProgramiv(LambertProgram, GL_LINK_STATUS, &linked);

	if (!linked) {
		cout << "Couldn't attach shader" << endl;
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(LambertProgram, 512, &size, infoLog);
		cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void createShaders() {
	createLambert();
	createBlinn();
	createMinnaert();
	createToon();
	createAmiGooch();
	createRim();
	shader_program = LambertProgram;
}

void removeShaders() {
	glUseProgram(0);
	glDeleteProgram(LambertProgram);
	glDeleteProgram(BlinnProgram);
	glDeleteProgram(MinnaertProgram);
	glDeleteProgram(ToonProgram);
	glDeleteProgram(AmiGoochProgram);
	glDeleteProgram(RimProgram);
}

void init(void) {
	glClearColor(0.4, 0.4, 0.4, 1.0);
	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h) {
	width = w; height = h;
	glViewport(0, 0, w, h);
}

struct PackedVertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
	};
};

bool getSimilarVertexIndex(
	PackedVertex & packed,
	map<PackedVertex, unsigned short> & VertexToOutIndex,
	unsigned short & result
) {
	map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end())
		return false;
	else {
		result = it->second;
		return true;
	}
}

void loadOBJ(const string & path, vector<glm::vec3> & outputVertices, vector<glm::vec2> & outputUV, vector<glm::vec3> & outputNormals) {
	vector<unsigned int> vertexIndices, UVIndices, normalIndices;
	vector<glm::vec3> tempVertices;
	vector<glm::vec2> tempUV;
	vector<glm::vec3> tempNormals;

	ifstream infile(path);
	string line;
	while (getline(infile, line)) {
		stringstream ss(line);
		string lineHeader;
		getline(ss, lineHeader, ' ');
		if (lineHeader == "v") {
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;
			vertex.x *= scale;
			vertex.y *= scale;
			vertex.z *= scale;
			tempVertices.push_back(vertex);
		}
		else if (lineHeader == "vt") {
			glm::vec2 uv;
			ss >> uv.x >> uv.y;
			tempUV.push_back(uv);
		}
		else if (lineHeader == "vn") {
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			tempNormals.push_back(normal);
		}
		else if (lineHeader == "f") {
			unsigned int vertex_index, uv_index, normal_index;
			char slash;
			while (ss >> vertex_index >> slash >> uv_index >> slash >> normal_index) {
				vertexIndices.push_back(vertex_index);
				UVIndices.push_back(uv_index);
				normalIndices.push_back(normal_index);
			}
		}
	}

	// triangles
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = tempVertices[vertexIndex - 1];
		outputVertices.push_back(vertex);

		unsigned int uvIndex = UVIndices[i];
		glm::vec2 uv = tempUV[uvIndex - 1];
		outputUV.push_back(uv);

		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = tempNormals[normalIndex - 1];
		outputNormals.push_back(normal);
	}
}

void indexVBO(
	vector<glm::vec3> & inputVertices,
	vector<glm::vec2> & inputUV,
	vector<glm::vec3> & inputNormals,

	vector<unsigned short> & outputIndices,
	vector<glm::vec3> & outputVertices,
	vector<glm::vec2> & outputUV,
	vector<glm::vec3> & outputNormals
) {
	map<PackedVertex, unsigned short> VertexToOutIndex;

	for (unsigned int i = 0; i < inputVertices.size(); i++) {
		PackedVertex packed = { inputVertices[i], inputUV[i], inputNormals[i] };

		// находим схожую в output
		unsigned short index;
		bool found = getSimilarVertexIndex(packed, VertexToOutIndex, index);

		// если нашли используем схожую
		if (found)
			outputIndices.push_back(index);
		else {
			// если нет, добавляем новую
			outputVertices.push_back(inputVertices[i]);
			outputUV.push_back(inputUV[i]);
			outputNormals.push_back(inputNormals[i]);
			unsigned short newindex = (unsigned short)outputVertices.size() - 1;
			outputIndices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

void initBuffers() {
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	
	loadOBJ(obj.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	//gen
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &uvBuffer);
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &elementBuffer);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	
	//binding
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	
	//pointers
	// вершины
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// нормали
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// UV
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void clearBuffers() {
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &elementBuffer);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

const double pi = 3.14159265358979323846;
void calcNewLightPosition() {
	double x = light_rad * glm::cos(lightAngle / 180 * pi);
	double z = light_rad * glm::sin(lightAngle / 180 * pi);
	light[0] = x;
	light[1] = lightPosition;
	light[2] = z;
}

void setTransform() {
	/* in shader:
	uniform struct Transform{
	mat4 model;
	mat4 viewProjection;
	mat3 normal;
	vec3 viewPosition;
	} transform;
	*/
	GLint s_model = glGetUniformLocation(shader_program, "transform.model");
	GLint s_proj = glGetUniformLocation(shader_program, "transform.viewProjection");
	GLint s_normal = glGetUniformLocation(shader_program, "transform.normal");
	GLint s_view = glGetUniformLocation(shader_program, "transform.viewPosition");

	glUniformMatrix4fv(s_model, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(s_proj, 1, GL_FALSE, &viewProjection[0][0]);
	glUniformMatrix3fv(s_normal, 1, GL_FALSE, &normaltr[0][0]);
	glUniform3fv(s_view, 1, viewPosition);

	checkOpenGLerror();
}

void setPointLight() {
	/* in shader:
	uniform struct PointLight {
		vec4 position;
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec3 attenuation;
	} light;*/

	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	GLint s_ambient = glGetUniformLocation(shader_program, "light.ambient");
	GLint s_diffuse = glGetUniformLocation(shader_program, "light.diffuse");
	GLint s_specular = glGetUniformLocation(shader_program, "light.specular");
	GLint s_attenuation = glGetUniformLocation(shader_program, "light.attenuation");

	glUniform4fv(s_position, 1, light);
	glUniform4fv(s_ambient, 1, ambient);
	glUniform4fv(s_diffuse, 1, diffuse);
	glUniform4fv(s_specular, 1, specular);
	glUniform3fv(s_attenuation, 1, attenuation);
}

void Lambert() {
	glUseProgram(shader_program);
	float fColor[4] = { 1.0f,0.0f,1.0f,1.0f };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);
	checkOpenGLerror();

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();
}

void Blinn() {
	glUseProgram(shader_program);
	float fColor[4] = { 1.0f,0.0f,1.0f,1.0f };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);
	checkOpenGLerror();

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();

	//set material
	GLint s_diffuse = glGetUniformLocation(shader_program, "material.diffuse");
	GLint s_specular = glGetUniformLocation(shader_program, "material.specular");
	GLint s_shiness = glGetUniformLocation(shader_program, "material.shiness");

	float m_diffuse[]{ 0.5f,0.0f,0.0f,1.0f };
	float m_specular[]{ 0.7f,0.7f,0.0f,1.0f };
	float m_shiness = 30;

	glUniform4fv(s_diffuse, 1, m_diffuse);
	glUniform4fv(s_specular, 1, m_specular);
	glUniform1f(s_shiness, m_shiness);
}

void Minnaert() {
	glUseProgram(shader_program);
	float fColor[4] = { 1.0f,1.0f,0.0f,1.0f };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);
	checkOpenGLerror();

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();

	//set koef
	float koef = 0.8;
	GLint s_k = glGetUniformLocation(shader_program, "k");
	glUniform1f(s_k, koef);
	checkOpenGLerror();
}

void Toon() {
	glUseProgram(shader_program);
	float fColor[4] = { 0.0f,1.0f,0.0f,1.0f };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);
	checkOpenGLerror();

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();
}

void Gooch() {
	glUseProgram(shader_program);
	float fColor[4] = { 0.75f,0.75f,0.75f,1.0f };
	GLint color = glGetUniformLocation(shader_program, "diffColor");
	glUniform4fv(color, 1, fColor);
	checkOpenGLerror();

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();

	//set other colors
	float oWarm[4] = { 0.6f,0.6f,0.0f, 1.0f };
	GLint warm = glGetUniformLocation(shader_program, "warm");
	glUniform4fv(warm, 1, oWarm);
	checkOpenGLerror();

	float oCold[4] = {0.0f,0.0f,0.6f, 1.0f};
	GLint cold = glGetUniformLocation(shader_program, "cold");
	glUniform4fv(cold, 1, oCold);
	checkOpenGLerror();

	float oDiffWarm = 0.45;
	GLint dwarm = glGetUniformLocation(shader_program, "diffwarm");
	glUniform1f(dwarm, oDiffWarm);
	checkOpenGLerror();

	float oDiffCold = 0.45;
	GLint dcold = glGetUniformLocation(shader_program, "diffcold");
	glUniform1f(dcold, oDiffCold);
	checkOpenGLerror();
}

void Rim() {
	glUseProgram(shader_program);

	setTransform();
	//set point light
	GLint s_position = glGetUniformLocation(shader_program, "light.position");
	glUniform4fv(s_position, 1, light);
	checkOpenGLerror();

	//set other colors
	float diffc[4] = { 0.5f,0.0f,0.0f, 1.0f };
	GLint diff = glGetUniformLocation(shader_program, "diff");
	glUniform4fv(diff, 1, diffc);
	checkOpenGLerror();

	float specc[4] = { 0.7f,0.7f,0.0f, 1.0f };
	GLint spec = glGetUniformLocation(shader_program, "spec");
	glUniform4fv(spec, 1, specc);
	checkOpenGLerror();

	float ospecPow = 30.0f;
	GLint specpow = glGetUniformLocation(shader_program, "specPow");
	glUniform1f(specpow, ospecPow);
	checkOpenGLerror();

	float orimPow = 8.0f;
	GLint rimpow = glGetUniformLocation(shader_program, "rimPow");
	glUniform1f(rimpow, orimPow);
	checkOpenGLerror();

	float obias = 0.3f;
	GLint bias = glGetUniformLocation(shader_program, "bias");
	glUniform1f(bias, obias);
	checkOpenGLerror();
}

void display(void) {	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	model = glm::mat4(1.0f);
	//rotate model here
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
	
	switch (task) {
	case 1: Lambert();
		break;
	case 2: Blinn();
		break;
	case 3: Minnaert();
		break;
	case 4: Toon();
		break;
	case 5: Gooch();
		break;
	case 6: Rim();
		break;
	}

	
	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	checkOpenGLerror();
	glutSwapBuffers();
}
void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1: task = 1;
		shader_program = LambertProgram;
		break;
	case GLUT_KEY_F2:
		task = 2;
		shader_program = BlinnProgram;
		break;
	case GLUT_KEY_F3:
		task = 3;
		shader_program = MinnaertProgram;
		break;
	case GLUT_KEY_F4:
		task = 4;
		shader_program = ToonProgram;
		break;
	case GLUT_KEY_F5:
		task = 5;
		shader_program = AmiGoochProgram;
		break;
	case GLUT_KEY_F6:
		task = 6;
		shader_program = RimProgram;
		break;
	case GLUT_KEY_UP: lightPosition += 0.5; break;
	case GLUT_KEY_DOWN: lightPosition -= 0.5; break;
	case GLUT_KEY_RIGHT: lightAngle -= 3; break;
	case GLUT_KEY_LEFT: lightAngle += 3; break;
	case GLUT_KEY_PAGE_UP: light_rad -= 0.5; break;
	case GLUT_KEY_PAGE_DOWN: light_rad += 0.5; break;
	}
	calcNewLightPosition();

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
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

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("WATERMELON!!!");

	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		cout << "Error: " << glewGetErrorString(glew_status) << endl;   
		return 1;
	}
	if (!GLEW_VERSION_2_0) {
		cout << "No support for OpenGL 2.0 found\n" << endl;
		return 1;
	}
	
	initBuffers();
	makeTextureImage();
	createShaders();
	init();
	calcNewLightPosition();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	removeShaders();
	clearBuffers();
}