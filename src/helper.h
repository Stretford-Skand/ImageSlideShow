#pragma once
#include <qmetatype.h>
#include <qset.h>
#include <qstringlist.h>

namespace ISS
{
#ifndef DEFAULT_SLIDE_TIME
#define DEFAULT_SLIDE_TIME 2
#endif

	enum MirrorMode
	{
		NoMirror = 0,
		Horizontal_Mirror,
		Vertical_Mirror,
		Both_Mirror
	};

	enum SlideShowState
	{
		NoImageData = 0,
		Idle,
		Running
	};

	enum ISSError
	{

		ISS_UnknownError = -99,
		ISS_NoSupportedImagesFound,
		ISS_DirectoryNotFound,
		ISS_ImageDoesntExist,

		ISS_Success = 0
	};

	QString errorMessage(const ISSError& err);
	const QPair<bool, bool> getMirrorModeBools(const MirrorMode& mirrorMode);

	// If the source image is already mirrored, it needs to be unmirrored when displaying in viewer
	const MirrorMode getViewerMirrorMode(const MirrorMode& srcMode, const MirrorMode& newMode);
	bool operator==(const QStringList& list1, const QStringList& list2);
} // namespace ISS