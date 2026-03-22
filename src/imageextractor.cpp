#include "imageextractor.h"

#include "qfiledialog.h"
#include "qstandardpaths.h"
using namespace ISS;

ImageExtractor::ImageExtractor(QObject* parent, const QStringList& supportedImageFormat)
    : QObject(parent),
      m_supportedImageformat(supportedImageFormat),
      m_imageDirectory(QDir(QString())),
      m_imageFiles(QStringList()),
      m_imageFileInfos(QFileInfoList())
{
}

const QFileInfoList& ImageExtractor::getImageFiles()
{
	return m_imageFileInfos;
}

void ImageExtractor::updateImagecontentsFromDirectory(const bool& newDir)
{
	auto ret = extractImagesFromDir(newDir);
	if (ret != ISS_Success)
	{
		qCritical() << errorMessage(ret);
	}
}

void ImageExtractor::browseAndSelectFolder()
{
	QString dir = QFileDialog::getExistingDirectory(
	    qobject_cast<QWidget*>(this->parent()), tr("Select the folder with images for slide-show"),
	    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), QFileDialog::ShowDirsOnly);

	if (!dir.isEmpty())
	{
		bool newDir = false;
		if (m_imageDirectory != QDir(dir))
		{
			m_imageDirectory = QDir(dir);
			newDir = true;
		}
		updateImagecontentsFromDirectory(newDir);
	}
	else
	{
		qWarning() << "No folder is selected. Application continues to use previous folder, if present. ";
	}
}

ISSError ImageExtractor::extractImagesFromDir(const bool& newDir)
{
	if (m_imageDirectory.exists())
	{
		auto files = m_imageDirectory.entryList(m_supportedImageformat, QDir::Files, QDir::Name);

		if ((m_imageFiles == files) && !newDir)
		{
			return ISS_Success;
		}
		m_imageFiles = files;
		m_imageFileInfos = m_imageDirectory.entryInfoList(m_supportedImageformat, QDir::Files, QDir::Name);

		if (m_imageFiles.size())
		{
			emit folderContentsModified(true);
		}
		else
		{
			emit folderContentsModified(false);
			return ISS_NoSupportedImagesFound;
		}
	}
	else
	{
		if (m_imageFiles.size())
		{
			m_imageFiles.clear();
			emit folderContentsModified(false);
		}
		return ISS_DirectoryNotFound;
	}

	return ISS_Success;
}