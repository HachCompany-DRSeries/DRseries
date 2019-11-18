
#include "fileTools.h"


	FileTools::FileTools()
	{
		path = NULL;
		searchString = NULL;
		file = NULL;
		hSearch = NULL;
	}

	FileTools::~FileTools()
	{
		if (path != NULL)
			delete[] path;
		if (searchString != NULL)
			delete[] searchString;
		if (file != NULL)
			delete[] file;
		if (hSearch != NULL)
			FindClose( hSearch );
	}

	char *FileTools::getFirstFile(char *dir, char *imgType)
	{
		int imgTypeLength;

		this.~ImgDirectory();
		
		imgTypeLength = strlen(imgType);
		pathLength = strlen(dir);

		path = new char[pathLength+1];
		strcpy(path, dir);

		// Remove the trailing back slash from the directory name if it's there
		if (path[strlen(path)-1] == '\\')
		{
			path[strlen(path)-1] = '\0';
			--pathLength;
		}

		if (imgTypeLength > 0)
		{
			searchString = new char[pathLength+3+imgTypeLength+1];
			sprintf(searchString, "%s\\*.%s", dir, imgType);
		}
		else
		{
			searchString = new char[pathLength+1];
			sprintf(searchString, "%s", dir);
		}
		hSearch = FindFirstFile( searchString, &ffd );
		if (hSearch != INVALID_HANDLE_VALUE)
		{
			fileLength = pathLength + 1 + strlen(ffd.cFileName);
			file = new char[fileLength + 1];
			sprintf( file, "%s\\%s", path, ffd.cFileName );
		}
		else
			file = NULL;
		return file;
	}

	char *FileTools::getNextFile(void)
	{
		if ((hSearch == INVALID_HANDLE_VALUE) || (file == NULL))
			file = NULL;
		else
		{
			if (!FindNextFile( hSearch, &ffd ) )
				file = NULL;
			else
			{
				int newFileLength = pathLength + 1 + strlen(ffd.cFileName);

				if (newFileLength > fileLength)
				{
					// Allocate more memory for our file name
					fileLength = newFileLength;
					delete[] file;
					file = new char[fileLength + 1];
				}
				sprintf(file, "%s\\%s", path, ffd.cFileName );
			}
		}
		return file;
	}

	// This function deletes all the files in the specified directory with the given file extension.
	bool FileTools::DeleteAllFiles(char *dir, char *imgType)
	{
		getFirstFile(dir, imgType);
		while (file != NULL)
		{
			if (!DeleteFile(file))
				return false;
			getNextFile();
		}
		return true;
	}

