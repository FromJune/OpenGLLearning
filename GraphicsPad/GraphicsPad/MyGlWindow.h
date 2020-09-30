#ifndef MY_GL_WINDOW
#define ME_GL_WINDOW
#include <QtOpenGl\qglwidget>
#include <Qt\qtimer.h>


class MyGlWindow : public QGLWidget
{
	Q_OBJECT

	QTimer myTimer;
protected:
	void initializeGL();
	void paintGL();
private slots:
	void myUpdate();
};

#endif
