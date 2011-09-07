/*
 * Packer.h
 *
 *  Created on: Sep 4, 2011
 *      Author: ashleygwinnell
 */

#ifndef PACKER_H_
#define PACKER_H_

#include "../vendor/ogg130/ogg.h"
#include "../vendor/vorbis132/vorbisfile.h"

#include "../vendor/zlib123/zlib.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#define PACKER_AUDIO_MONO16   1
#define PACKER_AUDIO_STEREO16 2

#include <stdint.h>

class PackerFile {
	public:
		static const int TYPE_AUDIO = 0;
		static const int TYPE_IMAGE = 1;

		int32_t type;
		int32_t nameLength;
		char* nameString;
		uint64_t size;
		void* data;

		// audio file properties
		int32_t format;
		int32_t frequency;

	public:
		PackerFile();
		virtual ~PackerFile();
};

/**
 * Packer format
 *
 * 	num_files 					4 bytes
 * 	[
 * 		type 					4 bytes
 * 		name_length				4 bytes
 * 		name_string				name_length bytes
 * 		length					8 bytes
 * 			audiofileformat 	4 bytes (will exist depending on file type);
 * 			audiofilefrequency 	4 bytes (will exist depending on file type);
 * 			imagefilecolour		4 bytes (will exist depending on file type);
 * 			imagefilewidth		4 bytes
 * 			imagefileheight		4 bytes
 * 		data					length bytes
 * 	]
 * 	checksum					4 bytes
 *
 *
 */
class Packer {
	public:
		vector<PackerFile*> files;

	public:
		Packer();
		PackerFile* get(string file);
		void read(string inFile);
		void write(string outFile, vector<string> inFiles);
		virtual ~Packer();
};

#endif /* PACKER_H_ */
