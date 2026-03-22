#include "issmainwindow.h"
#include "logger.h"

#include <qapplication.h>

int main(int argC, char* argV[])
{
	QApplication app(argC, argV);

	ISS::ISSLogger::attach();
	ISS::ISSMainWindow imageSlideShow;
	imageSlideShow.show();

	app.exec();
	return 0;
}