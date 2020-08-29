#include <GL\glew.h>
#include "MyGlWindow.h"

void MyGlWindow::initializeGL()
{
	glewInit();

	GLfloat verts[] =
	{
		+0.00f, +1.00f,
		+0.00f, +0.83f,
		+0.00f, +0.50f,
		-0.25f, +0.67f,
		-0.50f, +0.33f,
		-0.75f, +0.00f,
		+0.25f, +0.67f,
		+0.50f, +0.33f,
		+0.75f, +0.00f,

		-0.10f, +0.00f,
		-0.10f, -1.00f,
		+0.10f, -1.00f,
		+0.10f, +0.00f

	};
	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLushort indices[] = { 
		0,3,6, 
		1,4,7, 
		2,5,8,
		9,10,11,
		9,11,12 
	};
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


}

void MyGlWindow::paintGL()
{
	glViewport(0, 0, width(), height());
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_SHORT, 0);
}