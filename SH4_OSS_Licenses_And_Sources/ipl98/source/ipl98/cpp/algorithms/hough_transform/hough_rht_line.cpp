/********************************************************************
   The Image Processing Library 98 - IPL98
   Copyright (C) by René Dencker Eriksen - edr@mip.sdu.dk

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation. Only
   addition is, that if the library is used in proprietary software
   it must be stated that IPL98 was used.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   More details can be found in the file licence.txt distributed
   with this library.
*********************************************************************/

#include "hough_rht_line.h"
#include <algorithm>
#include <time.h>

namespace ipl {

using std::find;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CHoughRHTLine::CHoughRHTLine() : m_os(cout) {
}


CHoughRHTLine::CHoughRHTLine(ostream& os, bool WriteOutputFiles, const string& FilePrefix) : m_os(os) {
   m_initialized = false;
   m_writeOutputFiles = WriteOutputFiles;
   m_filePrefix = FilePrefix;
   m_houghSpace2D = NULL;
}


CHoughRHTLine::~CHoughRHTLine() { 
   // if (m_houghSpace2D!=NULL) delete m_houghSpace2D;
}


//////////////////////////////////////////////////////////////////////
// Public methods
//////////////////////////////////////////////////////////////////////


bool CHoughRHTLine::Initialize(CHoughBase::CBinaryImage& BinarySource, int Thickness, 
                                 CHoughBase::PARAMETERSPACETYPE SpaceType, CHoughBase::PARAMETERTYPE ParamType) {
   m_binaryImage = BinarySource;
   return this->InitializeCommon(Thickness, SpaceType, ParamType);
}


bool CHoughRHTLine::Initialize(const CStdImage& Source, COLORTYPE Color, UINT8 Threshold, 
							   CHoughBase::ORIGINTYPE OriginType, int Thickness, 
							   CHoughBase::PARAMETERSPACETYPE SpaceType, CHoughBase::PARAMETERTYPE ParamType) {
   if ((Source.GetBits()!=1) && (Source.GetBits()!=8)) {
		ostringstream ost;
		ost << "CHoughRHTLine::Initialize() Source must be 1 or 8 b/p (current is " 
			<< Source.GetBits() << ")" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		return false;
	}
   m_binaryImage = CHoughBase::CBinaryImage(Source, Color, Threshold, OriginType);
   return this->InitializeCommon(Thickness, SpaceType, ParamType);
}


vector<CParameter2D<float> > CHoughRHTLine::PerformTransform(int HSThreshold, int MaxNumShapes) {
   vector<CParameter2D<float> > peak_coordinates_real;

   if (!m_initialized) {
		ostringstream ost;
		ost << "CHoughRHTLine::RandomizedHoughTransform() Not initialized" << IPLAddFileAndLine;
		CError::ShowMessage(IPL_ERROR,ost);
		peak_coordinates_real.empty();
		return peak_coordinates_real;
	}

   int k = 0;
   int maxK = m_binaryImage.GetSize()*HSThreshold*MaxNumShapes/10;
   int numofShapes = 0;
   int procent = 0, procent_old = 0;
   bool detected;
   m_houghSpace2D->SetThreshold(HSThreshold);

   m_os << "Randomized Hough transform started." << endl;
   m_os << "Distance criterium (min): " << m_distanceCriteriumMin << endl;
   m_os << "Distance criterium (max): " << m_distanceCriteriumMax << endl;   
   m_os << "Detection threshold:      " << HSThreshold << endl;
   
   CHoughBase::CBinaryImage rc_bImg(m_binaryImage.GetWidth(), m_binaryImage.GetHeight(), m_binaryImage.GetOrigin());
   
   srand( (unsigned) time( NULL ) );
   while (k<maxK && numofShapes<=MaxNumShapes && m_binaryImage.GetSize()>HSThreshold*3) {

      this->GetPixelPair();
      detected = this->ComputeRandomizedHough();

      if (detected) {

         CParameter2D<int> peak_disk = m_houghSpace2D->GetPeakCoordinates();
         CParameter2D<float> peak_coordinate_real = m_houghSpace2D->GetRealPeakCoordinates();
         std::vector<CPoint2D<int> >::iterator p;
         p = find(m_peaks.begin(), m_peaks.end(), peak_disk);
         if (p == m_peaks.end()) {  // heuristic check
            m_peaks.push_back(peak_disk);
	        peak_coordinates_real.push_back(peak_coordinate_real);

            numofShapes++;
            cout << "Parameter " << numofShapes << ": " << peak_coordinate_real << endl;
            m_binaryImage.RemoveLine2(peak_coordinate_real,2);
            if (m_writeOutputFiles) this->WriteFiles(rc_bImg, peak_coordinate_real, numofShapes);
         } else m_binaryImage.RemoveLine2(peak_coordinate_real,5);
         m_houghSpace2D->Alloc();
      }

      k++;

      // compute finished procentage
      procent = (int) (100.0*k/maxK);
      if (procent > procent_old) {
         m_os << "#";
         procent_old = procent;
      }
   }
   return peak_coordinates_real;
}


void CHoughRHTLine::ToImage(CStdImage& outImg) { 
   m_houghSpace2D->ToImage(outImg); 
}


//////////////////////////////////////////////////////////////////////
// Private methods
//////////////////////////////////////////////////////////////////////


bool CHoughRHTLine::InitializeCommon(int Thickness, CHoughBase::PARAMETERSPACETYPE SpaceType, 
                                     CHoughBase::PARAMETERTYPE ParamType) {
   int width = m_binaryImage.GetWidth();
   int height = m_binaryImage.GetHeight();

   switch (SpaceType) {
	case CHoughBase::QUANTIZATION_SPACE:
      m_houghSpace2D = new CHoughSpace2D;
		break;
	case CHoughBase::SPARSE_SPACE:
      m_houghSpace2D = new CSparseHoughSpace2D;
		break;
	default:
      m_houghSpace2D = new CSparseHoughSpace2D;
		break;
	}

   m_houghSpace2D->Initialize(width, height, Thickness, 303, ParamType);
	this->ComputeSinCosTheta(m_houghSpace2D->GetWidth());

	m_distanceCriteriumMin = 2.0f;
	m_distanceCriteriumMax = m_houghSpace2D->GetParameterRangeValue(2,2);

   m_initialized = true;
   return true;
}


void CHoughRHTLine::ComputeSinCosTheta(int n) {
   printf("Computing sine and cosine value vectors...\n");
   float T = m_houghSpace2D->GetParameterRangeValue(1,2) - m_houghSpace2D->GetParameterRangeValue(1,1);
   float theta = (float) (-m_houghSpace2D->GetOffset(1)*T/n);
   float step  = (float) (T/n);

   m_sinTheta.resize(n);
   m_cosTheta.resize(n);
   for(int i=0; i<n; i++) {
      m_sinTheta[i] = (float) sin(theta);
      m_cosTheta[i] = (float) cos(theta);
      theta += step;
   }
}


void CHoughRHTLine::WriteFiles(CHoughBase::CBinaryImage& BImage, const CParameter2D<float>& Coordinate, int NumOfShapes) {
   string filename;
   char no[2];
   sprintf(no, "%i", NumOfShapes);
   CImage outImage;

   // Reconstruction files
   filename = "";
   filename += m_filePrefix+"_RC_RHT_"+no+".bmp";
   BImage.AddLine(Coordinate);
   BImage.ToImage(outImage);
   outImage.Save(filename);

   // Original image files
   filename = "";
   filename += m_filePrefix+"_ORIGINAL_RHT_"+no+".bmp";
   m_binaryImage.ToImage(outImage);
   outImage.Save(filename);

   // Parameter space files
   /*
   filename = "";
   filename += Output+"_HS_RHT_"+no+".bmp";
   m_houghSpace2D.ToImage(outImage);
   outImage.Save(filename);
   */
}


void CHoughRHTLine::GetPixelPair() {
   int index1, index2;
   index1 = Round(1.0f*rand()/RAND_MAX*m_binaryImage.GetSize());   // get 1st index
   index2 = Round(1.0f*rand()/RAND_MAX*m_binaryImage.GetSize());   // get 2nd index
   m_p1 = m_binaryImage.GetPixel(index1);  
   m_p2 = m_binaryImage.GetPixel(index2);

   if (this->DistanceCheck()) {
   } else {
      this->GetPixelPair();
   }
}


float CHoughRHTLine::Distance() {
   float dx12 = (float)(m_p1.GetX() - m_p2.GetX());
   float dy12 = (float)(m_p1.GetY() - m_p2.GetY());
   return sqrt(dx12*dx12+dy12*dy12);
}


bool CHoughRHTLine::DistanceCheck() {
   float d = this->Distance();
   if (m_distanceCriteriumMin < d && d < m_distanceCriteriumMax) return true;
   return false;
}


bool CHoughRHTLine::ComputeRandomizedHough() {
   float theta, rho;
   int t=-1, r=-1;

   bool detected = false;

   float rhoMin = m_houghSpace2D->GetParameterRangeValue(2,1);
   float rhoMax = m_houghSpace2D->GetParameterRangeValue(2,2);
   int param1Q = m_houghSpace2D->GetQuantizationValue(1);
   int param2Q = m_houghSpace2D->GetQuantizationValue(2);
   CPoint2D<int> position;

   theta = (float) atan((1.0*m_p1.GetX()-m_p2.GetX())/(m_p2.GetY()-m_p1.GetY()));
   if (theta<0.0f) theta += (float) PI;

   t = m_houghSpace2D->Discretize(1, theta);
   rho = m_p1.GetX()*cos(theta) + m_p1.GetY()*sin(theta);
   // rho2 = m_p2.GetX()*cos(theta) + m_p2.GetY()*sin(theta);
   // rho = m_p1.GetX()*m_cosTheta[t] + m_p1.GetY()*m_sinTheta[t];

   if (!(rhoMin>=rho || rho>rhoMax)) {
      r = m_houghSpace2D->Discretize(2, rho);

      if (0<=t && t<param1Q && 0<=r && r<param2Q) {
		  position.Set(t,r);
        detected = m_houghSpace2D->Inc(position);
      }
   }
   // cout << position << " " << detected << endl;
   return detected;
}

} // end namespace ipl
