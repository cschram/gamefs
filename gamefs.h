#ifndef __HEADER_GAMEFS
#define __HEADER_GAMEFS

#include "physfs/physfs.h"

namespace GameFS {

	// Initialize the file system
	// This must be done before using anything else
	int Init(const char* argv);

	// Deinitialize the file system
	void DeInit();

	// Set the write dir
	// This is the directory the file system is able to write files to.
	// This needs to be set before opening a file for writing.
	void SetWriteDir(std::string dir);

	// Mount a directory or archive to a point in the virtual filesystem
	void Mount(std::string path, std::string mountPoint="/");

	// Return a string of the last error from PhysicsFS
	std::string GetLastError();

	//
	// File Class
	// This is a light wrapper around the PHYFS_file 
	class File {
	public:

		// File Mode for opening files
		enum Mode {
			READ,
			WRITE,
			APPEND
		};

	private:

		std::string  mFileName; // File Name
		PHYSFS_file* mFile;     // File Handle
		Mode         mFileMode; // File Mode

	public:

		File(std::string filename, Mode mode=READ);
		~File();

		int         Length();                // Get the full length of the file
		int         Tell();                  // Tell the current position in the file
		int         Seek(int pos);           // Seek to a position in the file
		std::string Read(int len);           // Read a length of data
		int         Write(std::string data); // Write a string of data

	};

}

#endif