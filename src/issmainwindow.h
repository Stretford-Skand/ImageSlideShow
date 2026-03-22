#pragma once

#include "centralviewer.h"
#include "imageextractor.h"
#include "imagestream.h"
#include "qthread.h"
#include "ui_issmainwindow.h"

#include <qdebug.h>
#include <qmainwindow.h>

namespace ISS
{

	class ISSMainWindow : public QMainWindow, public Ui_issMainWindow
	{
		Q_OBJECT

	public:
		ISSMainWindow();

		~ISSMainWindow() override;
	signals:
		void slideShowStateChanged();

	private slots:
		void onPbStartStopClicked();
		void onSlideShowStateChanged();
		void onFolderContentModified(const bool& imagesAvailable);

	private:
		void makeConnections();
		void fillCbMirrorMode();
#ifdef RELOAD_BUTTON
		void addReloadFolderDataPushButton();
#endif // RELOAD_BUTTON
		void onStreamStatusChanged(bool isStreaming);
		void setMirrorMode(const MirrorMode& mirrorMode);
		void displayFirstImage() const;
		bool getMirrorModeFromValue(const QString& mirrorModeStr, MirrorMode& mode) const;

		int m_slideTime{DEFAULT_SLIDE_TIME};
		SlideShowState m_slideShowState{SlideShowState::NoImageData};
		std::map<MirrorMode, QString> m_mapMirrorModes;
		MirrorMode m_currentMirrorMode{MirrorMode::NoMirror};
		ImageExtractor* m_imageExtractor;
		CentralViewer* m_centralViewer;
#ifdef RELOAD_BUTTON
		QPushButton* pb_reload;
#endif
		QScopedPointer<ImageStream> m_imageStream;
		QThread m_streamThread;
	};
} // namespace ISS
Q_DECLARE_METATYPE(ISS::MirrorMode);
Q_DECLARE_METATYPE(ISS::ISSError);
