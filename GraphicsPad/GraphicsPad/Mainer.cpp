#include <Qt\qapplication.h>
#include <MyGlWindow.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MyGlWindow myWindow;
	myWindow.show();
	return app.exec();
}