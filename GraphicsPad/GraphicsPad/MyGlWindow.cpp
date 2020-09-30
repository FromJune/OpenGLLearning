#include <gl\glew.h>
#include <iostream>
#include <fstream>
#include "MyGlWindow.h"
using namespace std;

const uint NUM_VERTRICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const float DELTA_X = 0.1f;
const float SIZE_PER_TRI = NUM_VERTRICES_PER_TRI * NUM_FLOATS_PER_VERTICE * sizeof(float);
uint numTri = 0;
const uint MAX_TRI_NUM = 20;



QTimer myTimer;

void sendDataToOpenGL()
{

	GLuint vertexBufferID;
	
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, 20 * SIZE_PER_TRI, NULL, GL_STATIC_DRAW);

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

	GLuint programID = glCreateProgram();
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

	connect(&myTimer, SIGNAL(timeout()), this, SLOT(myUpdate()));
	myTimer.start(100);

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
	sendTriangleToOpenGL();
	glDrawArrays(GL_TRIANGLES, (numTri - 1) * NUM_VERTRICES_PER_TRI, NUM_VERTRICES_PER_TRI);
}

void MyGlWindow::myUpdate()
{
	//cout << "frame!" << endl;
	
	sendDataToOpenGL();
	repaint();
}