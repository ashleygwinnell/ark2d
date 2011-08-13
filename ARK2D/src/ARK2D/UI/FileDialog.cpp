/*
 * FileDialog.cpp
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#include "FileDialog.h"

void FileDialog::init(FileDialogPlatformData* fdpd)
{
	#ifdef ARK2D_WINDOWS
		fdpd->opf.hwndOwner = 0;
		fdpd->opf.lpstrFilter = "All files\0*.*\0\0";
		fdpd->opf.lpstrCustomFilter = 0;
		fdpd->opf.nMaxCustFilter = 0L;
		fdpd->opf.nFilterIndex = 1L;
		fdpd->opf.lpstrFile = (LPSTR) &fdpd->buffer[0];
		fdpd->opf.lpstrFile[0] = '\0';
		fdpd->opf.nMaxFile = 256;
		fdpd->opf.lpstrFileTitle = 0;
		fdpd->opf.nMaxFileTitle=50;
		fdpd->opf.lpstrInitialDir = 0;
		//fdpd->opf.lpstrInitialDir = path;
		fdpd->opf.lpstrTitle = fdpd->title.c_str();
		fdpd->opf.nFileOffset = 0;
		fdpd->opf.nFileExtension = 2;
		fdpd->opf.lpstrDefExt = "*.*";
		fdpd->opf.lpfnHook = NULL;
		fdpd->opf.lCustData = 0;
		fdpd->opf.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
		fdpd->opf.lStructSize = sizeof(OPENFILENAME);
	#elif defined(ARK2D_MACINTOSH)

	#endif
}

string FileDialog::openFileDialog() {
	return openFileDialog("Open File");
}
string FileDialog::openFileDialog(string title) {
	#ifdef __WIN32
		FileDialogPlatformData* fdpd = new FileDialogPlatformData();
		fdpd->title = title;
		FileDialog::init(fdpd);

		string returnString;
		if (GetOpenFileName(&fdpd->opf)) {
			returnString.append(fdpd->opf.lpstrFile);
		}
		return returnString;
	#elif defined(ARK2D_MACINTOSH)

		NSOpenPanel* op = [NSOpenPanel openPanel];
		if ([op runModal] == NSOKButton)
		{
			NSURL* fileUrl = [op URL];
			NSString* fileName = [fileUrl path];
			string retStr = [fileName cStringUsingEncoding:[NSString defaultCStringEncoding]];
			return retStr;
		}
		return "";
	#endif
	return "";
}

string FileDialog::saveFileDialog() {
	return saveFileDialog("Save File");
}
string FileDialog::saveFileDialog(string title) {
	#ifdef __WIN32
		FileDialogPlatformData* fdpd = new FileDialogPlatformData();
		fdpd->title = title;
		FileDialog::init(fdpd);

		string returnString;
		if (GetSaveFileName(&fdpd->opf)) {
			returnString.append(fdpd->opf.lpstrFile);
		}
		return returnString;
	#elif defined(ARK2D_MACINTOSH)

		NSSavePanel* op = [NSSavePanel savePanel];
		if ([op runModal] == NSOKButton)
		{
			NSURL* fileUrl = [op URL];
			//NSString* fileName = [fileUrl absoluteString];
			//NSString* fileName = [fileUrl resourceSpecifier];
			//NSString* fileName = [fileUrl relativeString];
			NSString* fileName = [fileUrl path];
			string retStr = [fileName cStringUsingEncoding:[NSString defaultCStringEncoding]];
			return retStr;
		}
		return "";

	#endif
	return "";
}
