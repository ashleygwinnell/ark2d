/*
 * FileDialog.h
 *
 *  Created on: 14 Jul 2011
 *      Author: Ashley
 */

#ifndef FILEDIALOG_H_
#define FILEDIALOG_H_

#include "../ARK.h"

class FileDialogPlatformData {
	public:
		#ifdef __WIN32
			OPENFILENAME opf;
			string title;
			char buffer[256];
		#endif
};

class FileDialog {
	private:
		static void init(FileDialogPlatformData* fdpd)
		{
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
		}

	public:
		static string openFileDialog() {
			return openFileDialog("Open File");
		}
		static string openFileDialog(string title) {
			#ifdef __WIN32
				FileDialogPlatformData* fdpd = new FileDialogPlatformData();
				fdpd->title = title;
				FileDialog::init(fdpd);

				string returnString;
				if (GetOpenFileName(&fdpd->opf)) {
					returnString.append(fdpd->opf.lpstrFile);
				}
				return returnString;
			#endif
		}

		static string saveFileDialog() {
			return saveFileDialog("Save File");
		}
		static string saveFileDialog(string title) {
			#ifdef __WIN32
				FileDialogPlatformData* fdpd = new FileDialogPlatformData();
				fdpd->title = title;
				FileDialog::init(fdpd);

				string returnString;
				if (GetSaveFileName(&fdpd->opf)) {
					returnString.append(fdpd->opf.lpstrFile);
				}
				return returnString;
			#endif
		}
};

#endif /* FILEDIALOG_H_ */
