/*
 * main.cpp
 *
 *  Created on: Sep 4, 2011
 *      Author: ashleygwinnell
 */

#include "ARK.h"

// exec /Users/ashleygwinnell/Projects/C++/ark2d/Tools/Packer/build_release/Packer.app/Contents/MacOS/Packer

int main(int argc, char** argv) {




	bool write = true;

	std::cout << "Started" << std::endl;
	if (argc <= 1) {
		std::cout << "must be some cmd args" << std::endl;
		return 0;
	} else {

		Packer* p = new Packer();

		if (write) {
			string outputFile(argv[1]);

			vector<string> inputFiles;
			for(int i = 2; i < argc; i++) {
				inputFiles.push_back(argv[i]);
			}

			p->write(outputFile, inputFiles);
		}
		else {

			string inputFile(argv[1]);
			p->read(inputFile);

			//PackerFile* pf = p->get("sound1.ogg");

		}


	}

	std::cout << "Finished" << std::endl;
	return 0;

}
