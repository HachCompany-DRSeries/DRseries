#include "FileTools.h"

#include <ipl98/cpp/image.h>
#include <time/time_date.h> 

/* Without the next line, an object must be declared like this: ipl::CImage Img; */
using namespace ipl;

class ImageProcessor
{
public:
	ImageProcessor(void);
	~ImageProcessor (void);
	bool Setup(char *directoryName, int &imgWidth, int &imgHeight);
	bool k_copyTImageToBMP_bmBits(const TImage *colorImage, unsigned char *screen_bmp_bmBits);
	bool ProcessNextFrame(unsigned char *screenBitmap_bmBits);

private:
	FileTools imageDir; // gets the names of the files in a directory
	char *readDirectory; // the directory of files to read
	char *imageFileName; // the full path name of the next file
	CImage image; // the next image to be displayed
	int imageWidth, imageHeight; // dimensions of the first image
	int bitmapFileBits; // number of bits used for each pixel in the first image
	int numberOfImages; // number of images loaded
};
