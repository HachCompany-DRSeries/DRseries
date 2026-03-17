
#include <windows.h>
#include <stdio.h>

// This object provides the names of files in a directory with a given file extension (such as .bmp)
class FileTools
{
private:
	char *path; // the directory where the images are located
	int pathLength; // length of the path string (includes the null termination character)
	char *searchString;  // the directory where the images are located plus something like *.bmp
	char *file; // full path name of the next file
	int fileLength; // length of the file string (includes the null termination character)
	WIN32_FIND_DATA ffd;
	HANDLE hSearch;

public:
	FileTools();
	~FileTools();
	char *getFirstFile(char *dir, char *imgType);
	char *getNextFile(void);
	bool DeleteAllFiles(char *dir, char *imgType);
};


