/*
 * Packer.cpp
 *
 *  Created on: Sep 4, 2011
 *      Author: ashleygwinnell
 */

#include "Packer.h"

string getExtension(string s);
string getExtension(string s) { 
	unsigned int pos = s.find_last_of('.') + 1;  
	return s.substr(pos);
}

bool is_big_endian();
bool is_big_endian() {
	static unsigned long x(1);
	static bool result(reinterpret_cast<unsigned char*>(&x)[0] == 0);
	return result;
}

const string base64_chars =
					 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					 "abcdefghijklmnopqrstuvwxyz"
					 "0123456789+/";

string encodeBase64(const unsigned char* bytes_to_encode, unsigned int in_len);
string encodeBase64(const unsigned char* bytes_to_encode, unsigned int in_len) {
	string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i) {
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';
	}

	return ret;
}

char* file_get_contents(const char* fileName);
char* file_get_contents(const char* fileName) {
    if (fileName != NULL) {

    	std::cout << "Opening file: " << fileName << std::endl;
    	std::fstream f(fileName, std::ios::in); 
    	if (!f.is_open()) {
    		//std::cout << "File does not exist." << std::endl;
    		string str = "Could not open file ["; str += fileName; str += "] as it does not exist.";
    		//ErrorDialog::createAndShow(str);
    		std::cout << str << std::endl;
    		return NULL;
    	} else {
    		f.close();

    		char* text = NULL;

    		FILE* file = fopen(fileName, "rt");
    		if (file == NULL) {
    			string str = "Could not open file ["; str += fileName; str += "] as it does not exist.";
    			std::cout << str << std::endl;
    			return NULL;
    		}

    		fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);

			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);

			return text;
    	}
    }
    return NULL;

}

void compress_memory(void *in_data, size_t in_data_size, std::vector<uint8_t> &out_data);
void compress_memory(void *in_data, size_t in_data_size, std::vector<uint8_t> &out_data)
{
 std::vector<uint8_t> buffer;

 const size_t BUFSIZE = 128 * 1024;
 uint8_t temp_buffer[BUFSIZE];

 z_stream strm;
 strm.zalloc = 0;
 strm.zfree = 0;
 strm.next_in = reinterpret_cast<uint8_t *>(in_data);
 strm.avail_in = in_data_size;
 strm.next_out = temp_buffer;
 strm.avail_out = BUFSIZE;

 deflateInit(&strm, Z_BEST_COMPRESSION);

 while (strm.avail_in != 0)
 {
  //int res =
		  deflate(&strm, Z_NO_FLUSH);
  //assert(res == Z_OK);
  if (strm.avail_out == 0)
  {
   buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
   strm.next_out = temp_buffer;
   strm.avail_out = BUFSIZE;
  }
 }

 int deflate_res = Z_OK;
 while (deflate_res == Z_OK)
 {
  if (strm.avail_out == 0)
  {
   buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
   strm.next_out = temp_buffer;
   strm.avail_out = BUFSIZE;
  }
  deflate_res = deflate(&strm, Z_FINISH);
 }

 //assert(deflate_res == Z_STREAM_END);
 buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
 deflateEnd(&strm);

 out_data.swap(buffer);
}


Packer::Packer():
	files() {

}

PackerFile* Packer::get(string file) {
	for (unsigned int i = 0; i < files.size(); i++) {
		PackerFile* pf = files.at(i);
		//std::cout << file.c_str() << " : " << pf->nameString << std::endl;
		if (strcmp(pf->nameString, file.c_str()) == 0) {
			return pf;
		}
	}
	std::cout << "File `" << file << "` not found in packer." << std::endl;
	return NULL;
}

void Packer::read(string inFile) {
	FILE* f = fopen(inFile.c_str(), "rb");

	uint32_t numFiles;
	fread(&numFiles, 4, 1, f);
	std::cout << "num files: " << numFiles << std::endl;

	for (unsigned int i = 0; i < numFiles; i++) {
		PackerFile* packerFile = new PackerFile();

		fread(&packerFile->type, 4, 1, f);
		std::cout << "type: " << packerFile->type << std::endl;

		fread(&packerFile->nameLength, 4, 1, f);
		std::cout << "nameLength: " << packerFile->nameLength << std::endl;

		packerFile->nameString = (char*) malloc(packerFile->nameLength);
		fread(packerFile->nameString, packerFile->nameLength, 1, f);

		//packerFile->nameString = "ASDASDASDAS";
		//std::cout << "derp" << std::endl;
		//std::cout << packerFile->nameString[0] << std::endl;
		packerFile->nameString[packerFile->nameLength-1] = '\0';
		//std::cout << "derp" << std::endl;
		std::cout << "nameString: " << packerFile->nameString << std::endl;

		fread(&packerFile->size, 8, 1, f);
		std::cout << "size: " << packerFile->size << std::endl;

		if (packerFile->type == PackerFile::TYPE_AUDIO) {
			fread(&packerFile->format, 4, 1, f);
			std::cout << "format: " << packerFile->format << std::endl;

			fread(&packerFile->frequency, 4, 1, f);
			std::cout << "frequency: " << packerFile->frequency << std::endl;
		}

		packerFile->data = (char*) malloc(packerFile->size);
		fread(packerFile->data, packerFile->size, 1, f);

		files.push_back(packerFile);
	}
}
void Packer::write(string outFile, vector<string> inFiles) {

	std::cout << "Reading input files..." << std::endl;
	for(unsigned int i = 0; i < inFiles.size(); i++) {

		string file = inFiles.at(i);

		ifstream thefile (file.c_str());
		if (!thefile.is_open()) {
			std::cout << "Could not open file: " << file << std::endl;
		} else {

			thefile.close();

			string ext = getExtension(file);
			if (ext == "ogg") {

				// Some vars!
				const unsigned int BUFFER_SIZE = 32768; // 32kb buffer
				int format;
				int frequency;
				int bitStream;
				long bytes;
				int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
				if (is_big_endian()) {
					endian = 1;
				}

				char array[BUFFER_SIZE]; // Local fixed size array
				vorbis_info* oggInfo;
				OggVorbis_File oggFile;
				std::vector<char> bufferData;


				// Open for binary reading
				FILE* f = fopen(file.c_str(), "rb");
				if (f == NULL) {
					std::cout << "Could not open file:" << file << std::endl;
					return;
				} else {
					std::cout << "Opened file:" << file << std::endl;
				}


				// open using the SDK, no need to call fclose() now.
				ov_open(f, &oggFile, NULL, 0);

				// Get some info about the OGG and store it in oggInfo.
				oggInfo = ov_info(&oggFile, -1);


				// Check the number of channels... always use 16-bit samples
				if (oggInfo->channels == 1) {
					format = PACKER_AUDIO_MONO16;
				} else {
					format = PACKER_AUDIO_STEREO16;
				}

				frequency = oggInfo->rate;

				do {
					// Read up to a buffer's worth of decoded sound data
					bytes = ov_read(&oggFile, &array[0], BUFFER_SIZE, endian, 2, 1, &bitStream);

					// Append to end of buffer
					bufferData.insert(bufferData.end(), array, array + bytes);
				}
				while (bytes > 0);



				// Make mean AudioFile
				PackerFile* af = new PackerFile();
				af->nameLength = strlen(file.c_str()) + 1;
				af->nameString = const_cast<char*>( file.c_str() );
				std::cout << af->nameString << std::endl;
				af->format = format;
				af->frequency = frequency;
				af->size = bufferData.size();
				af->data = malloc(af->size);
				memcpy(af->data, (void*) &bufferData[0], af->size);

				ov_clear(&oggFile);

				files.push_back(af);

			}
		}

	}

	std::cout << "Writing packed file..." << std::endl;
	FILE* out = fopen(outFile.c_str(), "w");
	if (out == NULL) {
		std::cout << "Could not write packed file. " << outFile.c_str() << std::endl;
		return;
	}



	// find total size.
	uint64_t totalSize = 0;
	for(unsigned int i = 0; i < files.size(); i++) {
		int thisSize = 0;
		thisSize += files.at(i)->size;
		thisSize += 12; // 4 for type and 8 for length.
		thisSize += 4; // 4 for name length;
		thisSize += files.at(i)->nameLength;

		if (files.at(i)->type == PackerFile::TYPE_AUDIO) {
			thisSize += 8; // 4 for format and 4 for frequency.
		}
		totalSize += thisSize;

		std::cout << "file " << i << " was " << files.at(i)->size << " and " << thisSize << " size." << std::endl;
	}
	totalSize += 1; // for \0

	char* space = (char*) malloc(totalSize);
	char* current = space;

	// write num of files
	uint32_t numFiles = files.size();
	memcpy(current, &numFiles, 4);
	current += 4;

	// for each file
	for(unsigned int i = 0; i < files.size(); i++) {
		PackerFile* f = files.at(i);

		//write type (4 bytes)
		memcpy(current, &f->type, 4);
		current += 4;

		// write name length (4 bytes)
		memcpy(current, &f->nameLength, 4);
		current += 4;

		// write name string
		memcpy(current, f->nameString, f->nameLength);
		current += f->nameLength;

		// write length (8 bytes)
		memcpy(current, &f->size, 8);
		current += 8;

		if (f->type == PackerFile::TYPE_AUDIO) {
			//write format
			//fwrite(&f->format, 4, 1, out);
			memcpy(current, &f->format, 4);
			current += 4;

			// write freq
			//fwrite(&f->frequency, 4, 1, out);
			memcpy(current, &f->frequency, 4);
			current += 4;
		}

		//fwrite(&f->data, f->size, 1, out);
		memcpy(current, f->data, f->size);
		current += f->size;
	}
	char end = '\0';
	memcpy(current, &end, 1);
	current += 1;

	//std::cout << ((char*) space) << std::endl;

	bool base64enabled = false;
	bool zlibEnabled = false;


	const char* newfcstr = NULL;
	int newflength = 0;

	if (base64enabled) {
		std::cout << "Base64 Encoding..." << std::endl;
		const unsigned char* blahh = (const unsigned char*) const_cast<const char*>(space);
		string newf = encodeBase64(blahh, totalSize);
		newfcstr = newf.c_str();
		newflength = newf.length();
	} else {
		newfcstr = const_cast<const char*>(space);
		newflength = totalSize;
	}


	if (zlibEnabled) {
		std::cout << "ZLib Compressing..." << std::endl;
		std::vector<unsigned char> woop;
		compress_memory((void*) newfcstr, newflength, woop);

		std::cout << "Writing packed file..." << std::endl;
		fwrite(&woop[0], woop.size(), 1, out);
	} else {
		std::cout << "Writing packed file..." << std::endl;
		fwrite((void*) newfcstr, newflength, 1, out);
	}



	fclose(out);



	//char* f = file_get_contents(outFile.c_str());
	//std::cout << f << std::endl;

	/*out = fopen(outFile.c_str(), "r");
	fseek(out, 0, SEEK_SET);
	int count = ftell(out);
	rewind(out);

	char* text = (char*) malloc(count+1);
	count = fread(text, sizeof(char), count, out);
	text[count] = '\0';

	std::cout << count << std::endl;*/



}

Packer::~Packer() {

}

PackerFile::PackerFile():
	nameString(NULL),
	data(NULL)
{

}

PackerFile::~PackerFile() {

}


