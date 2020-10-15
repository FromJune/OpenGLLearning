#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <glm\glm.hpp>
#include "MyGlWindow.h"

using namespace std;
using glm::vec3;
using glm::dot;

char ch;

const uint NUM_VERTRICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint NUM_WALLS = 4;
GLuint triOffsetUniformPos;

const float DELTA_X = 0.1f;
const float DELTA_Y = 0.1f;
const int max_speed = 10;
const int min_speed = -10;

const float SIZE_PER_TRI = NUM_VERTRICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
uint numTri = 0;
//const uint MAX_TRI_NUM = 20;
vec3 triOffset(0.0f, 0.0f, 0.0f);
vec3 zeroOffset(0.0f, 0.0f, 0.0f);
vec3 dotTest1(1.0f, 2.0f, 1.0f);
vec3 triPos(0.0f, 0.0f, 0.0f);


float rand_x = -(rand() % (max_speed - min_speed) + min_speed) / 10000.0f;
float rand_y = (rand() % (max_speed - min_speed) + min_speed) / 10000.0f;
vec3 rand_offset(rand_x, rand_y, 0.0f);

GLuint programID;

GLuint vao1ID;
GLuint vao2ID;

QTimer myTimer;

vec3 p1(0.0f, 1.0f, 0.0f);
vec3 p2(1.0f, 0.0f, 0.0f);
vec3 p3(0.0f, -1.0f, 0.0f);
vec3 p4(-1.0f, 0.0f, 0.0f);
vec3 white(1.0f, 1.0f, 1.0f);

vec3 wall[] =
{
	p1,
	white,
	p2,
	white,
	p3,
	white,
	p4,
	white,
};

GLfloat thisTri[] =
{
	0.0f, DELTA_Y, 0.0f,
	1.0f, 0.0f, 0.0f,

	DELTA_X, -DELTA_Y, 0.0f,
	1.0f, 0.0f, 0.0f,

	-DELTA_X, -DELTA_Y, 0.0f,
	1.0f, 0.0f, 0.0f,
};

void sendDataToOpenGL()
{

	GLuint vertexBufferID;
	GLuint vertexBuffer2ID;

	
	glGenVertexArrays(1, &vao1ID);
	glBindVertexArray(vao1ID);

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(thisTri), thisTri,GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, SIZE_PER_TRI, SIZE_PER_TRI, thisTri);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	glGenVertexArrays(1, &vao2ID);
	glBindVertexArray(vao2ID);

	glGenBuffers(1, &vertexBuffer2ID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wall), wall, GL_STATIC_DRAW);

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
	glBindVertexArray(vao1ID);
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTRICES_PER_TRI);
	//draw wall
	glUniform3fv(triOffsetUniformPos, 1, &zeroOffset[0]);
	glBindVertexArray(vao2ID);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	

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
	triPos = triOffset;

	//cout << triPos[0] << " " << triPos[1] << endl;
	

	//cout << rand_x << "__" << rand_y << endl;
	if (triOffset[0] + DELTA_X >= 1.0f)
	{
		rand_offset[0] = -rand_speed();
		rand_offset[1] = rand_speed_any_direction();

	}
	if (triOffset[0] - DELTA_X <= -1.0f)
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
	if (triOffset[1] - DELTA_Y <= -1.0f)
	{
		rand_offset[0] = rand_speed_any_direction();
		rand_offset[1] = rand_speed();
	}
	bool hitWall = false;

	for (int i = 0; i < NUM_WALLS; i++)
	{
		vec3 first = wall[i*2];
		vec3 second = wall[((i + 1) % (NUM_WALLS)) * 2];
		
		vec3 wall = second - first;
		vec3 respectiveTriPos;
		if (i == 1)
		{
			respectiveTriPos = triPos - first + vec3(DELTA_X, -DELTA_Y, 0.0f);
		}
		else if (i == 2)
		{
			respectiveTriPos = triPos - first + vec3(-DELTA_X, -DELTA_Y, 0.0f);
		}
		else
		{
			respectiveTriPos = triPos - first + vec3(0.0f, DELTA_Y, 0.0f);
		}
		
		vec3 normal(wall[1], -wall[0], wall[2]);
		//cout << respectiveTriPos[0] << " " << respectiveTriPos[1] << endl;


		float dotResult = dot(respectiveTriPos, normal);

		hitWall = hitWall || (dotResult < 0);
		if (hitWall)
		{
			rand_offset[0] = 0.0f;
			rand_offset[1] = 0.0f;

			
		}
		
	}

	if (GetAsyncKeyState(0x20))
	{
		triOffset = zeroOffset;
		rand_offset[0] = rand_speed_any_direction();
		rand_offset[1] = rand_speed_any_direction();
	}


	repaint();
}

