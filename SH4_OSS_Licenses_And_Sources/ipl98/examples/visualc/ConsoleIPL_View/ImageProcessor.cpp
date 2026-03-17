
#include "ImageProcessor.h"
#include <ipl98/cpp/algorithms/coordinate_transform.h>

ImageProcessor::ImageProcessor()
{
	readDirectory = NULL;
}

ImageProcessor::~ImageProcessor()
{
	delete[] readDirectory;
}

// This function loads the first image file in the directory directoryName and sets
// the width and height in the variables: imgWidth and imgHeight
bool ImageProcessor::Setup(char *directoryName, int &imgWidth, int &imgHeight)
{
	this.~ImageProcessor();
	
	// Read the program input parameters
	if (directoryName == NULL)
		return false;
	
	readDirectory = new char[strlen(directoryName)+1];
	strcpy(readDirectory,directoryName);

	numberOfImages = 0;
	
	// Load the first image and prepare to process the sequence of images
	imageFileName = imageDir.getFirstFile(readDirectory,"bmp");
	if (imageFileName == NULL)
	{
		printf("Unable to read \".bmp\" image files from directory: %s\n", readDirectory);
		return false;
	}
	else
	{
		// Load a BMP file
		if (!image.Load(imageFileName))
		{
			printf("Can't open image file: %s\n", imageFileName);
			return false;
		}
		bitmapFileBits = image.GetBits();
		if ((bitmapFileBits == 8) && (bitmapFileBits == 1))
			image.CopyConvert(24,image);
		++numberOfImages;
	}
	
	imageWidth = image.GetWidth();
	imageHeight = image.GetHeight();
	imgWidth = imageWidth;
	imgHeight = imageHeight;
	
	printf("Loading images in directory: %s\n", readDirectory);
	return true;
}

// This C function copies a TImage to a one dimentional array, so it can be displayed to the screen.
bool ImageProcessor::k_copyTImageToBMP_bmBits(const TImage *colorImage, unsigned char *screen_bmp_bmBits)
{
	int x,y;
	long bitmapIndex = 0;

	if (screen_bmp_bmBits == NULL)
		return false;
	// Copy Image TImage to screen_bmp_bmBits so it can be displayed
	for (y = colorImage->Height-1; y >= 0; --y)
		for (x = 0; x < colorImage->Width; ++x)
		{
			screen_bmp_bmBits[bitmapIndex] = k_GetBlueComponentFast(x,y, colorImage);
			++bitmapIndex;
			screen_bmp_bmBits[bitmapIndex] = k_GetGreenComponentFast(x,y, colorImage);
			++bitmapIndex;
			screen_bmp_bmBits[bitmapIndex] = k_GetRedComponentFast(x,y, colorImage);
			++bitmapIndex;
		}
	return true;
}

//  screenBitmap_bmBits is a Pointer to the bitmap data, allowing flat access
bool ImageProcessor::ProcessNextFrame(unsigned char *screenBitmap_bmBits)
{
	// Copy the image to the screen memory
	k_copyTImageToBMP_bmBits(image.GetTImagePtr(), screenBitmap_bmBits);
	
	// Open the next image file
	imageFileName = imageDir.getNextFile();
	if (imageFileName == NULL)
	{
		printf("Loaded %d images.\n", numberOfImages);
		return false;
	}
	
	// Load a BMP file
	if (!image.Load(imageFileName))
	{
		printf("Can't open image file: %s\n", imageFileName);
		return false;
	}
	
	// Check to make sure the bits and dimentions are the same - if not, we scale and change bits
	if ((image.GetWidth() != imageWidth) || (image.GetHeight() != imageHeight) || (image.GetBits() != bitmapFileBits))
	{
		image.CopyConvert(24,image);
		CCoordinateTransform::ScaleFixed(image,image,image.GetWidth()/2,image.GetHeight()/2,imageWidth,imageHeight,
			(double)imageWidth/image.GetWidth(),(double)imageHeight/image.GetHeight(),0);
	}
	++numberOfImages;
	
	if ((bitmapFileBits == 8) && (bitmapFileBits == 1))
		image.CopyConvert(24,image);
	
	return true;
}
