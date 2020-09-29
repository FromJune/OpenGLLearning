#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include <glm\glm.hpp>
#include "MyGlWindow.h"

using namespace std;
using glm::vec3;

const uint NUM_VERTRICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const float DELTA_X = 0.1f;
const float SIZE_PER_TRI = NUM_VERTRICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
uint numTri = 0;
const uint MAX_TRI_NUM = 20;


GLuint vao1ID;
GLuint vao2ID;
GLuint vao3ID;

GLuint programID;



void sendDataToOpenGL()
{
	GLfloat vertices1[] =
	{
		-1.0f, 1.0f, 0.5f,
		0.8f, 0.8f, 0.8f,
		1.0f, 1.0f, 0.5f,
		0.8f, 0.8f, 0.8f,
		-1.0f, 0.0f, 0.5f,
		0.8f, 0.8f, 0.8f,
		1.0f, 0.0f, 0.5f,
		0.8f, 0.8f, 0.8f,

		-1.0f, 0.0f, 0.5f,
		0.7f, 0.7f, 0.7f,
		1.0f, 0.0f, 0.5f,
		0.7f, 0.7f, 0.7f,
		-1.0f, -1.0f, 0.5f,
		0.7f, 0.7f, 0.7f,
		1.0f, -1.0f, 0.5f,
		0.7f, 0.7f, 0.7f,
	};
	GLfloat vertices2[] =
	{
		-1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f,
		-1.0f, 0.3f, 0.0f,
		0.5f, 0.5f, 0.5f,

		-0.5f, 0.7f, 0.0f,
		0.5f, 0.5f, 0.5f,
		0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f,

	};
	GLfloat vertices3[] =
	{
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		-0.5f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		-0.5f, 0.7f, 0.0f,
		1.0f, 1.0f, 1.0f,

		0.0f, 0.3f, 0.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f,

	};

	GLushort indices1[] =
	{
		0,1,2,
		1,3,2,

		4,5,6,
		5,7,6,
	};

	GLushort indices2[] =
	{
		0,1,2,
		1,3,4,

	};

	GLushort indices3[] =
	{
		0,2,1,
		1,4,3,


	};

	
	glGenVertexArrays(1, &vao1ID);
	glBindVertexArray(vao1ID);

	GLuint vertexBufferID;
	GLuint elementBufferID;
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &elementBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);


	glGenVertexArrays(1, &vao2ID);
	glBindVertexArray(vao2ID);

	GLuint vertexBuffer2ID;
	glGenBuffers(1, &vertexBuffer2ID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer2ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));

	
	GLuint elementBuffer2ID;
	glGenBuffers(1, &elementBuffer2ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer2ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao3ID);
	glBindVertexArray(vao3ID);

	GLuint vertexBuffer3ID;
	glGenBuffers(1, &vertexBuffer3ID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer3ID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (char*)(sizeof(float) * 3));


	GLuint elementBuffer3ID;
	glGenBuffers(1, &elementBuffer3ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer3ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);


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
	glewInit();

	glEnable(GL_DEPTH_TEST);

	sendDataToOpenGL();

	installShaders();
}

void sendTriangleToOpenGL()
{
	if (numTri == MAX_TRI_NUM)
		return;

	const GLfloat THIS_TRI_X = -1.0f + numTri * DELTA_X;

	GLfloat thisTri[] = 
	{ 
		THIS_TRI_X, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		THIS_TRI_X + DELTA_X, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		THIS_TRI_X, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	glBufferSubData(GL_ARRAY_BUFFER, numTri * SIZE_PER_TRI, SIZE_PER_TRI, thisTri);
	numTri++;
}

void MyGlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	//sendTriangleToOpenGL();
	vec3 triOffset(0.0f, 0.0f, 0.0f);
	float xScale = 0.25f;

	GLuint triOffsetUniformPos = glGetUniformLocation(programID, "triOffset");
	GLuint yScaleUniformPos = glGetUniformLocation(programID, "yScale");
	GLuint xScaleUniformPos = glGetUniformLocation(programID, "xScale");
	
	//offset(0,0,0) yScale 1
	glUniform3fv(triOffsetUniformPos, 1, &triOffset[0]);
	glUniform1f(yScaleUniformPos, 1.0f);
	glUniform1f(xScaleUniformPos, 1.0f);

	glBindVertexArray(vao1ID);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);

	glUniform1f(xScaleUniformPos, xScale);

	float tri_x;
	float tri_y_scale;

	for(int i = 0; i < 1 / xScale * 2; i++)
	{
		for (float j = 0; j < 2; j++)
		{
			tri_x =  i - 1.0f * (1.0f / xScale - 1.0f);
			tri_y_scale = -1.0f + j * 2;
			triOffset.r = tri_x;
			glUniform3fv(triOffsetUniformPos, 1, &triOffset[0]);
			glUniform1f(yScaleUniformPos, tri_y_scale);
			

			glBindVertexArray(vao2ID);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

			glBindVertexArray(vao3ID);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
			
		}
	}



}