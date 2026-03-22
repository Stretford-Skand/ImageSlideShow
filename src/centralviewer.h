#pragma once

#include "helper.h"

#include <QWidget>
#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qscrollarea.h>

namespace ISS
{
	class CentralViewer : public QWidget
	{
		Q_OBJECT
	public:
		CentralViewer(QWidget* parent = nullptr);
		~CentralViewer() override;

		void updateMirrorProperty(const MirrorMode& mirrorHV);
		void clearImageContents();

	public slots:
		void setImage(const QImage& image, const ISS::MirrorMode& srcImgMode);

	private:
		void resizeEvent(QResizeEvent* event) override;
		void updateDisplay();

		QImage m_srcImage, m_mirroredImage;
		MirrorMode m_srcImgMode{NoMirror};
		QLabel* m_imgLabel;
		QVBoxLayout* m_widgetLayout;
	};
} // namespace ISS
