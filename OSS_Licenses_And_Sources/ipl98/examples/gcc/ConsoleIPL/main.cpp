// Example on using a class from the IPL98 library. The include path
// and file name for a given class can always be found on the C++ Overview
// homepage. At the top of each class documentation the precise path is shown.
// To see the path shown here, click the CImage link on the C++ Overview
// homepage
#include <ipl98/cpp/image.h>

// iostream needed for cout
#include <iostream>

/* Without the next line, an object must be declared like this: ipl::CImage Img; */
using namespace ipl;

int main()
{
	CImage Img;

    std::cout << "IPL98 version: " << IPL98_VERSION << "\n\n";

	// Load a BMP file
	if (Img.Load("../../images/george.bmp"))
	{
		// output basic image information to console
		std::cout << Img << "\n";
	}
	std::cout << "Finished...\n";

	return 0;
}
