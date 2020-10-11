#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <glm\glm.hpp>
#include "MyGlWindow.h"

using namespace std;
using glm::vec3;

const uint NUM_VERTRICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
GLuint triOffsetUniformPos;

const float DELTA_X = 0.1f;
const float DELTA_Y = 0.1f;
const int max_speed = 20;
const int min_speed = -20;

const float SIZE_PER_TRI = NUM_VERTRICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
uint numTri = 0;
//const uint MAX_TRI_NUM = 20;
vec3 triOffset(0.0f, 0.0f, 0.0f);


float rand_x = -(rand() % (max_speed - min_speed) + min_speed) / 10000.0f;
float rand_y = (rand() % (max_speed - min_speed) + min_speed) / 10000.0f;
vec3 rand_offset(rand_x, rand_y, 0.0f);

GLuint programID;

QTimer myTimer;

void sendDataToOpenGL()
{

	GLuint vertexBufferID;

	GLfloat thisTri[] =
	{
		0.0f, DELTA_Y, DELTA_Y,
		1.0f, 0.0f, 0.0f,

		DELTA_X, DELTA_Y, DELTA_Y,
		1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	
	
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	//glBufferData(GL_ARRAY_BUFFER, 100000 * SIZE_PER_TRI, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, numTri * SIZE_PER_TRI, SIZE_PER_TRI, thisTri);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	
	


}

bool checkStatus(GLuint objectID,
	PFNGLGETSHADERIVPROC getivFuc, 
	PFNGLGETSHADERINFOLOGPROC getInfoLogFuc,
	GLenum statusType
	)
{
	GLint status;
	getivFuc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		getivFuc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei buffersize;
		getInfoLogFuc(objectID, infoLogLength, &buffersize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID) 
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	GLint status;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei buffersize;
		glGetProgramInfoLog(programID, infoLogLength, &buffersize, buffer);
		cout << "something wrong with the program!" << endl;

		delete[] buffer;
		return false;
	}
	return true;
	//return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>());
}

void installShaders()
{

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if(! checkShaderStatus(vertexShaderID) || ! checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (! checkProgramStatus(programID))
		return;

	glUseProgram(programID);
}

void MyGlWindow::initializeGL()
{
	srand(time(0));

	glewInit();

	glEnable(GL_DEPTH_TEST);


	sendDataToOpenGL();

	installShaders();

	triOffsetUniformPos = glGetUniformLocation(programID, "triOffset");


	connect(&myTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
	myTimer.start(0);

	

	
}

void MyGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glUniform3fv(triOffsetUniformPos, 1, &triOffset[0]);
	glDrawArrays(GL_TRIANGLES, (numTri - 1) * NUM_VERTRICES_PER_TRI, NUM_VERTRICES_PER_TRI);
}

float rand_speed()
{
	return abs((rand() % (max_speed - min_speed) + min_speed) / 10000.0f);
}

float rand_speed_any_direction()
{
	return (rand() % (max_speed - min_speed) + min_speed) / 10000.0f;
}

void MyGlWindow::myUpdate()
{
	triOffset += rand_offset;
	

	//cout << rand_x << "__" << rand_y << endl;
	if (triOffset[0] + DELTA_X >= 1.0f)
	{
		rand_offset[0] = -rand_speed();
		rand_offset[1] = rand_speed_any_direction();

	}
	if (triOffset[0] <= -1.0f)
	{
		rand_offset[0] = rand_speed();
		rand_offset[1] = rand_speed_any_direction();
	}
	if (triOffset[1] + DELTA_Y >= 1.0f)
	{
		rand_offset[0] = rand_speed_any_direction();
		rand_offset[1] = -rand_speed();
		cout << rand_offset[1] << endl;
	}
	if (triOffset[1] <= -1.0f)
	{
		rand_offset[0] = rand_speed_any_direction();
		rand_offset[1] = rand_speed();
	}

	repaint();
}