#include "filesys.h"

namespace GameFS {

	bool isInit = false;

	int Init(const char* argv) {
		if (!PHYSFS_isInit()) {
			int ret = PHYSFS_init(argv);
			if (ret == 0) {
				std::cout << "[gamefs] Error: Unable to initialize: " << GetLastError() << "\n";
			}
			return ret;
		}

		return 1;
	}

	void DeInit() {
		PHYSFS_deinit();
		isInit = false;
	}

	void SetWriteDir(std::string dir) {
		PHYSFS_setWriteDir(dir.c_str());
	}

	void Mount(std::string path, std::string mountPoint) {
		PHYSFS_mount(path.c_str(), mountPoint.c_str(), 1);
	}

	std::string GetLastError() {
		return std::string(PHYSFS_getLastError());
	}

	File::File(std::string filename, Mode mode) {
		if (PHYSFS_isInit()) {
			// Open File
			mFileName = filename;
			if (mode == READ) {
				mFile = PHYSFS_openRead(filename.c_str());
			} else if (mode == WRITE) {
				mFile = PHYSFS_openWrite(filename.c_str());
			} else if (mode == APPEND) {
				mFile = PHYSFS_openAppend(filename.c_str());
			}

			// Handle Errors
			if (mFile == NULL) {
				std::cerr << "[gamefs] Error: Could not open " << filename << ": " << GetLastError() << "\n";
			}
		}
	}

	File::~File() {
		if (mFile != NULL) {
			PHYSFS_close(mFile);
		}
	}

	int File::Length() {
		if (mFile != NULL) {
			return PHYSFS_fileLength(mFile);
		}

		return 0;
	}

	int File::Tell() {
		if (mFile != NULL) {
			int pos = PHYSFS_tell(mFile);
			if (pos == -1) {
				std::cerr << "[gamefs] Error: Could not get position of " << mFileName << ": " << GetLastError() << "\n";
			} else {
				return pos;
			}
		} else {
			std::cout << "[gamefs] Warning: File " << mFileName << " is not open.\n";
		}
		
		return -1;
	}

	int File::Seek(int pos) {
		if (mFile != NULL) {
			if (PHYSFS_seek(mFile, pos) == 0) {
				std::cerr << "[gamefs] Error: Unable to seek file " << mFileName << " to position " << pos << ": " << GetLastError() << "\n";
			} else {
				return 1;
			}
		} else {
			std::cout << "[gamefs] Warning: File " << mFileName << " is not open.\n";
		}
		
		return 0;
	}

	std::string File::Read(int len) {
		if (mFile != NULL) {
			char* buffer = new char[len];
			int read = PHYSFS_read(mFile, buffer, sizeof(char), len);

			if (read < len) {
				std::cerr << "[gamefs] Error: Cannot read " << mFileName << ":\n\t" << GetLastError() << "\n";
			} else {
				std::string out(buffer, len);
				delete buffer;
				return out;
			}
		}

		return std::string("");
	}


	int File::Write(std::string data) {
		if (mFile != NULL) {
			int written = PHYSFS_write(mFile, data.c_str(), sizeof(char), data.length());

			if (written < data.length()) {
				std::cerr << "[gamefs] Error: Cannot write to " << mFileName << ":\n\t" << GetLastError() << "\n";
			}

		} else {
			std::cout << "[gamefs] Warning: File " << mFileName << " is not open.\n";
		}
		
		return -1;
	}

}