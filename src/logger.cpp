#include "logger.h"

using namespace ISS;

QString ISSLogger::filename = QDir::currentPath() + QDir::separator() + "log.txt";
bool ISSLogger::logging = false;
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);

ISSLogger::ISSLogger(QObject* Parent) : QObject(Parent) {}

void ISSLogger::attach()
{
	ISSLogger::logging = true;
	qInstallMessageHandler(ISSLogger::handler);
}

void ISSLogger::handler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	if (ISSLogger::logging)
	{
		QString txt;
		switch (type)
		{
		case QtInfoMsg:
			txt = QString("Info: %1").arg(msg);
			break;
		case QtDebugMsg:
			txt = QString("Debug: %1").arg(msg);
			break;
		case QtWarningMsg:
			txt = QString("Warning: %1").arg(msg);
			break;
		case QtCriticalMsg:
			txt = QString("Critical: %1").arg(msg);
			break;
		case QtFatalMsg:
			txt = QString("Fatal: %1").arg(msg);
			break;
		}
		QFile file(ISSLogger::filename);
		if (file.open(QIODevice::Append))
		{
			QTextStream ts(&file);

			ts << QDateTime::currentDateTime().toString() << " - " << txt << context.file << " line: " << context.line
			   << '\r' << '\n';
			ts.flush();
			file.close();
		}
	}

	(*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}