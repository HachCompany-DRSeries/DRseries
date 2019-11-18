// Example on using a class from the IPL98 library. The include path
// and file name for a given class can always be found on the C++ Overview
// homepage. At the top of each class documentation the precise path is shown.
// To see the path shown here, click the CImage link on the C++ Overview
// homepage
#include <ipl98/cpp/image.h>

// ostream needed for endl
#include <ostream>
// iostream needed for cout
#include <iostream>

/* Without the next line, an object must be declared like this: ipl::CImage Img; */
using namespace ipl;


int main()
{
	CImage Img;
    cout << "IPL98 version: " << IPL98_VERSION << endl << endl;
	// Load a BMP file
	if (Img.Load("../../images/george.bmp")==true)
	{
		// output basic image information to console
		cout << Img << endl;
	}
	std::cout << "Finished..." << std::endl;
    if (getchar()){}
	return 0;
}
