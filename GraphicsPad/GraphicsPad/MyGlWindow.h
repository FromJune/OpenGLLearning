#ifndef MY_GL_WINDOW
#define ME_GL_WINDOW
#include <QtOpenGl\qglwidget>

class MyGlWindow : public QGLWidget
{
protected:
	void initializeGL();
	void paintGL();
public:
};

#endif
