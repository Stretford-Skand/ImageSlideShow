#include "helper.h"

using namespace ISS;

QString ISS::errorMessage(const ISSError& err)
{
	QString errMessage = QString();

	switch (err)
	{
	case ISS_UnknownError:
		errMessage = QString("The application has encountered an unknown error. ");
		break;
	case ISS_NoSupportedImagesFound:
		errMessage = QString("The provided directory does not contain images of supported image-format. ");
		break;
	case ISS_DirectoryNotFound:
		errMessage = QString("The provided directory for loading images is not available. ");
		break;
	case ISS_ImageDoesntExist:
		errMessage = QString("Unable to access and load the image found in the given directory. ");
		break;
	case ISS_Success:
		errMessage = QString("Function executed successfully. ");
		break;
	default:
		errMessage = QString("Undefined error. ");
		break;
	}

	return errMessage;
}

const QPair<bool, bool> ISS::getMirrorModeBools(const MirrorMode& mirrorMode)
{
	QPair<bool, bool> pairVal;
	switch (mirrorMode)
	{
	case NoMirror:
		pairVal = {false, false};
		break;
	case Horizontal_Mirror:
		pairVal = {true, false};
		break;
	case Vertical_Mirror:
		pairVal = {false, true};
		break;
	case Both_Mirror:
		pairVal = {true, true};
		break;
	default:
		pairVal = {false, false};
		break;
	}
	return pairVal;
}

const ISS::MirrorMode ISS::getViewerMirrorMode(const MirrorMode& srcMode, const MirrorMode& newMode)
{
	ISS::MirrorMode unMirrorMode = ISS::NoMirror;

	if (srcMode == newMode)
	{
		unMirrorMode = NoMirror;
		return unMirrorMode;
	}

	switch (srcMode)
	{
	case NoMirror:
		unMirrorMode = newMode;
		break;
	case Horizontal_Mirror:
		if (newMode == ISS::Vertical_Mirror)
		{
			unMirrorMode = ISS::Both_Mirror;
		}
		else if (newMode == Both_Mirror)
		{
			unMirrorMode = ISS::Vertical_Mirror;
		}
		else if (newMode == NoMirror)
		{
			unMirrorMode = ISS::Horizontal_Mirror;
		}
		break;
	case Vertical_Mirror:
		if (newMode == ISS::Horizontal_Mirror)
		{
			unMirrorMode = ISS::Both_Mirror;
		}
		else if (newMode == Both_Mirror)
		{
			unMirrorMode = ISS::Horizontal_Mirror;
		}
		else if (newMode == NoMirror)
		{
			unMirrorMode = ISS::Vertical_Mirror;
		}
		break;
	case Both_Mirror:
		if (newMode == ISS::Horizontal_Mirror)
		{
			unMirrorMode = ISS::Vertical_Mirror;
		}
		else if (newMode == Vertical_Mirror)
		{
			unMirrorMode = ISS::Horizontal_Mirror;
		}
		else if (newMode == NoMirror)
		{
			unMirrorMode = ISS::Both_Mirror;
		}
		break;
	default:
		unMirrorMode = ISS::NoMirror;
		;
		break;
	}
	return unMirrorMode;
}

bool ISS::operator==(const QStringList& list1, const QStringList& list2)
{
	if (list1.count() != list2.count())
	{
		return false;
	}

	auto diff = (QSet<QString>(list2.begin(), list2.end()) - QSet<QString>(list1.begin(), list1.end()));
	return diff.empty();
}