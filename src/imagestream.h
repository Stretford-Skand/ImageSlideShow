#pragma once

#include <qfileinfo.h>
#include <qmutex.h>
#include <qthread.h>
#include <qwaitcondition.h>

#include <helper.h>

class ImageStream : public QObject
{
	Q_OBJECT
public:
	ImageStream(const int& slideTime, QObject* parent = nullptr);

	void pauseStream();
	void resumeStream();
	void stopStream();
	void setImageFilesToStream(const QFileInfoList& imageFiles);

signals:
	void sendImageToDisplay(const QImage& image, const ISS::MirrorMode& mirrorMode);
	void finished();
	void streamStatusChanged(bool streamStatus);
	void error(ISS::ISSError err);

public slots:
	void startStream();
	void updateSlideTime(int slideTime);
	void setMirrorProperty(const ISS::MirrorMode& mode);

private:
	QMutex m_mutex;
	QWaitCondition m_waitCondition;
	bool m_pauseStream{false};
	bool m_stopStream{false};
	QFileInfoList m_streamImageFiles;
	int m_slideTime;
	ISS::MirrorMode m_mirrorHV{ISS::NoMirror};
};