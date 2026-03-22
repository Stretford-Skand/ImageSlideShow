#include "centralviewer.h"

#include "qcolorspace.h"
#include "qthread.h"

#include <qdebug.h>
#include <qevent.h>

using namespace ISS;

CentralViewer::CentralViewer(QWidget* parent)
    : QWidget(parent), m_imgLabel(new QLabel(this)), m_widgetLayout(new QVBoxLayout(this))
{
	m_imgLabel->setBackgroundRole(QPalette::Base);
	m_imgLabel->setScaledContents(false);
	m_imgLabel->setAlignment(Qt::AlignCenter);
	m_imgLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

	m_widgetLayout->addWidget(m_imgLabel);
}

CentralViewer::~CentralViewer() {}

void CentralViewer::setImage(const QImage& image, const MirrorMode& srcImgMode)
{
	if (!image.isNull())
	{
		m_srcImage = image.copy();
		m_mirroredImage = m_srcImage;
		if (m_srcImgMode != srcImgMode)
		{
			m_srcImgMode = srcImgMode; // Mirrored state of the actual image
		}
		updateDisplay();
	}
}

void CentralViewer::updateDisplay()
{
	if (!m_mirroredImage.isNull())
	{
		if (m_mirroredImage.colorSpace().isValid())
		{
			m_mirroredImage.convertToColorSpace(QColorSpace::SRgb);
		}
		m_imgLabel->setPixmap(QPixmap::fromImage(m_mirroredImage).scaled(m_imgLabel->size(), Qt::KeepAspectRatio));
		m_imgLabel->adjustSize();
	}
}

void CentralViewer::updateMirrorProperty(const MirrorMode& mirrorHV)
{
	// If the streamed image is already mirrored, the mirror has to be cancelled to display it in right mirror mode
	auto newMirrorMode = ISS::getViewerMirrorMode(m_srcImgMode, mirrorHV);

	if (!m_srcImage.isNull())
	{
		auto mirrorBool = getMirrorModeBools(newMirrorMode);
		m_mirroredImage =
		    m_srcImage.mirrored(mirrorBool.first, mirrorBool.second); // Mirroring the actual image in viewer
		updateDisplay();
	}
}

void CentralViewer::clearImageContents()
{
	m_srcImage = QImage();
	m_mirroredImage = QImage();
	m_imgLabel->clear();
}

void CentralViewer::resizeEvent(QResizeEvent* event)
{

	if (!m_mirroredImage.isNull())
	{
		m_imgLabel->setPixmap(QPixmap::fromImage(m_mirroredImage).scaled(event->size(), Qt::KeepAspectRatio));
	}
	QWidget::resizeEvent(event);
}
