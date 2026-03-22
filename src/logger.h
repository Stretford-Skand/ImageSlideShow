#pragma once

#include "helper.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QTextStream>

namespace ISS
{

	class ISSLogger : public QObject
	{
		Q_OBJECT
	public:
		explicit ISSLogger(QObject* parent = nullptr);
		static void attach();

	private:
		static void handler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

		static bool logging;
		static QString filename;
	};
} // namespace ISS