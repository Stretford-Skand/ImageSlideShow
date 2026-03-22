#include "imagestream.h"

#include <qdebug.h>
#include <qimage.h>

ImageStream::ImageStream(const int& slideTime, QObject* parent) : QObject(parent), m_slideTime(slideTime) {}

void ImageStream::setImageFilesToStream(const QFileInfoList& imageFiles)
{
	QMutexLocker locker(&m_mutex);
	m_streamImageFiles = imageFiles;
}

void ImageStream::startStream()
{
	m_stopStream = false;
	emit streamStatusChanged(true);

	while (!m_stopStream)
	{
		try
		{
			// Take a snapshot of the file list under lock so the list can be
			// safely updated from the main thread while we iterate.
			QFileInfoList currentFiles;
			{
				QMutexLocker locker(&m_mutex);
				currentFiles = m_streamImageFiles;
			}

			for (const QFileInfo& file : std::as_const(currentFiles))
			{
				// Check pause/stop flags before processing each image.
				{
					QMutexLocker locker(&m_mutex);
					if (m_pauseStream)
					{
						m_waitCondition.wait(&m_mutex);
					}
					if (m_stopStream)
					{
						return;
					}
				}

				if (!file.exists())
				{
					emit error(ISS::ISS_ImageDoesntExist);
					qCritical() << ISS::errorMessage(ISS::ISS_ImageDoesntExist) << file.absoluteFilePath();
					stopStream();
					return;
				}

				// Read shared state under lock before releasing it for the
				// slow operations (disk I/O and sleep) below.
				ISS::MirrorMode currentMirror;
				int currentSlideTime;
				{
					QMutexLocker locker(&m_mutex);
					currentMirror = m_mirrorHV;
					currentSlideTime = m_slideTime;
				}

				QImage image(file.absoluteFilePath());
				auto mirrorBool = ISS::getMirrorModeBools(currentMirror);
				emit sendImageToDisplay(image.mirrored(mirrorBool.first, mirrorBool.second), currentMirror);
				QThread::sleep(static_cast<unsigned long>(currentSlideTime));
			}
		}
		catch (const std::exception& e)
		{
			qCritical() << "Exception thrown when thread is streaming images to viewer. Exception: " << e.what();
			stopStream();
		}
		catch (...)
		{
			qCritical() << "Unknown exception thrown when streaming images to viewer.";
			stopStream();
		}
	}
}

void ImageStream::pauseStream()
{
	{
		QMutexLocker locker(&m_mutex);
		m_pauseStream = true;
	}
	emit streamStatusChanged(false);
}

void ImageStream::resumeStream()
{
	{
		QMutexLocker locker(&m_mutex);
		m_pauseStream = false;
		m_waitCondition.wakeAll();
	}
	emit streamStatusChanged(true);
}

void ImageStream::stopStream()
{
	{
		QMutexLocker locker(&m_mutex);
		m_waitCondition.wakeAll();
		m_pauseStream = false;
		m_stopStream = true;
	}
	emit finished();
	emit streamStatusChanged(false);
}

void ImageStream::updateSlideTime(int slideTime)
{
	QMutexLocker locker(&m_mutex);
	m_slideTime = slideTime;
}

void ImageStream::setMirrorProperty(const ISS::MirrorMode& mirror)
{
	QMutexLocker locker(&m_mutex);
	m_mirrorHV = mirror;
}
