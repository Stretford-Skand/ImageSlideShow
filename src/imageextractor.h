#pragma once

#include "helper.h"

#include <qdebug.h>
#include <qdir.h>
#include <qobject.h>

namespace ISS
{

	class ImageExtractor : public QObject
	{
		Q_OBJECT
	public:
		ImageExtractor(QObject* parent, const QStringList& supportedImageFormat);

		void updateImagecontentsFromDirectory(const bool& newDir);
		const QFileInfoList& getImageFiles();

	signals:
		void folderContentsModified(const bool& imagesAvailable);

	public slots:
		void browseAndSelectFolder();

	private:
		ISSError extractImagesFromDir(const bool& newDir);

		QDir m_imageDirectory;
		QStringList m_supportedImageformat;
		QStringList m_imageFiles;
		QFileInfoList m_imageFileInfos;
	};
} // namespace ISS