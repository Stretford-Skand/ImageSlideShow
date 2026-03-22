#include "issmainwindow.h"
using namespace ISS;
ISSMainWindow::ISSMainWindow()
    : QMainWindow(),
      m_mapMirrorModes{
          {NoMirror, "None"}, {Horizontal_Mirror, "Horizontal"}, {Vertical_Mirror, "Vertical"}, {Both_Mirror, "Both"}},
      m_imageExtractor(new ImageExtractor(this, QStringList{"*.tiff", "*.bmp", "*.jpg", "*.jpeg"})),
      m_centralViewer(new CentralViewer(this)),
#ifdef RELOAD_BUTTON
      pb_reload(nullptr),
#endif // RELOAD_BUTTON
      m_imageStream(new ImageStream(DEFAULT_SLIDE_TIME))
{
	setupUi(this);
	qRegisterMetaType<ISS::MirrorMode>();
	qRegisterMetaType<ISS::ISSError>();

	QSignalBlocker sb(sb_slideTime);
	sb_slideTime->setValue(m_slideTime);
	gb_imgArea->layout()->addWidget(m_centralViewer);

	m_streamThread.setObjectName("Slide-show streamer thread");
	m_imageStream->moveToThread(&m_streamThread);
#ifdef RELOAD_BUTTON
	addReloadFolderDataPushButton();
#endif
	fillCbMirrorMode();
	makeConnections();
}

ISSMainWindow::~ISSMainWindow()
{
	m_imageStream->stopStream();
	if (!m_streamThread.wait(5000))
	{
		qWarning() << "Stream thread did not stop cleanly within timeout, terminating.";
		m_streamThread.terminate();
	}
}

void ISSMainWindow::makeConnections()
{
	connect(pb_startStop, &QPushButton::clicked, this, &ISSMainWindow::onPbStartStopClicked);
	connect(this, &ISSMainWindow::slideShowStateChanged, this, &ISSMainWindow::onSlideShowStateChanged);
	connect(cb_mirrorMode, &QComboBox::currentTextChanged, cb_mirrorMode,
	        [this](const QString& mirrorModeStr)
	        {
		        MirrorMode selectedMode;
		        if (getMirrorModeFromValue(mirrorModeStr, selectedMode))
		        {
			        setMirrorMode(selectedMode);
			        m_currentMirrorMode = selectedMode;
		        }
		        else
		        {
			        // if Key not found
			        QSignalBlocker sb(cb_mirrorMode);
			        cb_mirrorMode->setCurrentText(m_mapMirrorModes.at(m_currentMirrorMode));
		        }
	        });
	connect(pb_imageFolder, &QPushButton::clicked, m_imageExtractor, &ImageExtractor::browseAndSelectFolder);
	connect(m_imageExtractor, &ImageExtractor::folderContentsModified, this, &ISSMainWindow::onFolderContentModified);
#ifdef RELOAD_BUTTON
	connect(pb_reload, &QPushButton::clicked, pb_reload,
	        [this]() { m_imageExtractor->updateImagecontentsFromDirectory(false); });
#endif
	auto imgStreamPtr = m_imageStream.data();
	connect(&m_streamThread, &QThread::started, imgStreamPtr, &ImageStream::startStream);
	connect(imgStreamPtr, &ImageStream::sendImageToDisplay, m_centralViewer, &CentralViewer::setImage);
	connect(imgStreamPtr, &ImageStream::finished, &m_streamThread, &QThread::quit);
	connect(sb_slideTime, qOverload<int>(&QSpinBox::valueChanged), sb_slideTime,
	        [this](int val) { m_imageStream->updateSlideTime(val); });
	connect(imgStreamPtr, &ImageStream::streamStatusChanged, this, &ISSMainWindow::onStreamStatusChanged);
	connect(imgStreamPtr, &ImageStream::error, imgStreamPtr,
	        [this](ISSError err)
	        {
		        if (err == ISS_ImageDoesntExist)
		        {
			        m_imageExtractor->updateImagecontentsFromDirectory(false);
		        }
	        });
}

void ISSMainWindow::fillCbMirrorMode()
{
	if (cb_mirrorMode->count())
	{
		cb_mirrorMode->clear();
	}
	for (auto& mapMode : m_mapMirrorModes)
	{
		cb_mirrorMode->addItem(mapMode.second);
	}
}

#ifdef RELOAD_BUTTON
void ISSMainWindow::addReloadFolderDataPushButton()
{
	pb_reload = new QPushButton(gb_Settings);
	pb_reload->setObjectName(QString::fromUtf8("pb_reload"));
	pb_reload->setText("Reload");
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/res/icon_reload.png"), QSize(), QIcon::Normal, QIcon::Off);
	pb_reload->setIcon(icon);
	pb_reload->setToolTip("To rescan and load images of selected folder");
	verticalLayout_2->addWidget(pb_reload);
}
#endif

void ISSMainWindow::onStreamStatusChanged(bool isStreaming)
{
	if (isStreaming)
	{
		m_slideShowState = Running;
	}
	else
	{
		m_slideShowState = Idle;
	}
	emit slideShowStateChanged();
}

void ISSMainWindow::onPbStartStopClicked()
{
	if (m_slideShowState == SlideShowState::NoImageData)
	{
		return;
	}

	if (m_slideShowState == Idle)
	{
		if (!m_streamThread.isRunning()) // NOLINT(bugprone-branch-clone)
		{
			m_streamThread.start();
		}
		else
		{
			m_imageStream->resumeStream();
		}
		return;
	}
	if (m_slideShowState == Running)
	{
		m_imageStream->pauseStream();
		return;
	}
	qCritical() << "Undefined behavior in updating the slide-show state"; //[unlikely]
}

void ISSMainWindow::onSlideShowStateChanged()
{
	if (m_slideShowState <= Idle)
	{
		pb_startStop->setText("Start");
		QIcon icon;
		icon.addFile(QString::fromUtf8(":/res/icon_play.png"));
		pb_startStop->setIcon(icon);
		pb_startStop->setToolTip("Start Slide-show");

		pb_startStop->setEnabled(m_slideShowState != NoImageData);
#ifdef RELOAD_BUTTON
		pb_reload->setEnabled(true);
#endif // RELOAD_BUTTON
	}
	else if (m_slideShowState == Running)
	{
		pb_startStop->setText("Stop");
		QIcon icon;
		icon.addFile(QString::fromUtf8(":/res/icon_stop.png"));
		pb_startStop->setIcon(icon);
		pb_startStop->setEnabled(true);
		pb_startStop->setToolTip("Stop Slide-show");
#ifdef RELOAD_BUTTON
		pb_reload->setEnabled(false);
#endif // RELOAD_BUTTON
	}
	else //[unlikely]
	{
		qCritical() << "Undefined behavior in updating the slide-show state";
	}
}

void ISSMainWindow::setMirrorMode(const MirrorMode& mirrorMode)
{
	m_centralViewer->updateMirrorProperty(mirrorMode);
	m_imageStream->setMirrorProperty(mirrorMode);
	m_currentMirrorMode = mirrorMode;
}

void ISS::ISSMainWindow::onFolderContentModified(const bool& imagesAvailable)
{
	if (m_streamThread.isRunning())
	{
		m_imageStream->stopStream();
		if (!m_streamThread.wait(5000))
		{
			qWarning() << "Stream thread did not stop cleanly within timeout, terminating.";
			m_streamThread.terminate();
		}
		qInfo() << "Stopping the slide-show, since image folder or image contents in the folder are modified";
	}

	if (imagesAvailable)
	{
		m_slideShowState = Idle;
		displayFirstImage();
		m_imageStream->setImageFilesToStream(m_imageExtractor->getImageFiles());
	}
	else
	{
		m_slideShowState = NoImageData;
		m_centralViewer->clearImageContents();
	}
	emit slideShowStateChanged();
}

void ISS::ISSMainWindow::displayFirstImage() const
{
	if (m_imageExtractor->getImageFiles().size())
	{
		const QFileInfo* file = &(m_imageExtractor->getImageFiles().at(0));

		if (file->exists())
		{
			auto fileWPath = file->absoluteFilePath();
			QImage firstImage(fileWPath);

			MirrorMode selectedMode = NoMirror;
			auto mirrorModeStr = cb_mirrorMode->currentText();
			getMirrorModeFromValue(mirrorModeStr, selectedMode);
			auto modeBools = ISS::getMirrorModeBools(selectedMode);
			m_centralViewer->setImage(firstImage.mirrored(modeBools.first, modeBools.second), selectedMode);
		}
		else
		{
			qCritical() << errorMessage(ISS_ImageDoesntExist);
		}
	}
}

bool ISS::ISSMainWindow::getMirrorModeFromValue(const QString& mirrorModeStr, MirrorMode& mirrorMode) const
{
	for (auto& mode : m_mapMirrorModes)
	{
		if (mode.second == mirrorModeStr)
		{
			mirrorMode = mode.first;
			return true;
		}
	}
	return false;
}
