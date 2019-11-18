#pragma hdrstop
#include <condefs.h>
#include <vector>
#include <ipl98/cpp/arrays/array2d.h>
#include <ipl98/cpp/arrays/arrayos2d.h>
#include <ipl98/cpp/image>
#include <ipl98/cpp/image.h>
#include <time/time_date.h>
#include <ipl98/cpp/algorithms/hough_transform/hough_sht_line.h>
#include <ipl98/cpp/algorithms/graphics.h>
#include <ipl98/cpp/arrays/arrayos2d.h>
#include <ipl98/cpp/algorithms/segmentate.h>
#include <ipl98/cpp/pixelgroups.h>
#include <ipl98/cpp/algorithms/statistic.h>
#include <ipl98/cpp/vectors/vector3d.h>

#pragma argsused

#include <iostream>
using namespace std;
using namespace ipl;

int main(int argc, char* argv[])
{
    CImage Img;
    cout << "IPL98 version: " << IPL98_VERSION << endl << endl;
    // Load a BMP file
    if (Img.Load("../../images/george.bmp")==true)
    {
      // output basic image information to console
      cout << Img << endl;
    }
    cout << "Finished..." << endl;
    if (getchar()){}
    return 0;
}
