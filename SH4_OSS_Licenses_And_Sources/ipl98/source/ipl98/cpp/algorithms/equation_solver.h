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

#ifndef _IPL98_EQUATIONSOLVER_H
#define _IPL98_EQUATIONSOLVER_H

#include "../../ipl98_setup.h" /* always include the setup file */
#include "../arrays/array2d.h"
#include <vector>

namespace ipl{

using std::vector;

/** @defgroup utilities Utility functions and classes (C++)
	Utility functions and classes are miscellaneous utilities for all kind
	of purposes not fitting into any other category. */

/** @ingroup utilities
@{ */

/** This class contains methods for solving systems of linear equations.
	It is in a very early stage of development so be aware of possible
	bugs and future changes.
	The class is used intensively by the Hough classes.

	This class is part of the Image Processing Library <a href="http://www.mip.sdu.dk/ipl98">IPL98</a>.
	\class CEquationSolver ipl98/cpp/algorithms/equation_solver.h
    @author Qinyin Zhou <qinyin@mip.sdu.dk>
    @version 0.36
*/
class CEquationSolver {
public:
	/** Default constructor. */
	CEquationSolver();
	
	/** Destructor */
	~CEquationSolver();
	
	/** Solve a system of linear equations Ax=b with LU decomposition technique.
		@param A Left hand side matrix.
		@param b Right hand side vector.
		@return The solution vector x. */
	static vector<float> LinearSolve(CArray2D<float>& A, vector<float>& b);
	
	/** Solve a system of linear equations Ax=b with Cholesky decomposition technique.
		@param A Left hand side matrix.
		@param b Right hand side vector.
		@return The solution vector x. */
	static vector<float> CholeskyLinearSolve(CArray2D<float>& A, vector<float>& b);
	
    /** Method calculating LU decomposition of a nxn matrix a.
		The method takes a matrix and stores the LU representation in the original matrix.
		The pivoting index is stored in the index vector.
		@param A Matrix a to be calculated. The result will be stored in A.
		@param index The pivot element index vector. */
	static void LUDecomp(CArray2D<float>& A, vector<unsigned int>& index);
	
	/** Solve a system of linear equations with a LU decomposited matrix.
		@param A LU decomposited matrix.
		@param index Pivot vector of decomposition. Changed from int to unsigned int by edr
		@param b RHS vector.
		@return The solution, return x. */
	static vector<float> LUSolve(const CArray2D<float>& A,const vector<unsigned int>& index, 
								 vector<float>& b);
	
	/** Performs Cholesky decomposition.
		@param A Matrix for decomposition.
		@param p Vector to store diagonal elements. */
	static void CholeskyDecomp(CArray2D<float>& A, vector<float>& p);
	
	/** Solve a Cholesky decomposited matrix.
		@param A Matrix.
		@param p Diagonal vector.
		@param b Right hand side vector.
		@param x Result vector.
		@return The solution, return x. */
	static vector<float> CholeskySolve(const CArray2D<float>& A, const vector<float>& p, 
									   const vector<float>& b, vector<float>& x);
	
	/** Solves an overdetermined system of linear equations Ax=b. */
	static vector<double> LinearOverDeterminedSolve(CArray2D<double>& A, vector<double>& b);

	/** Solves an overdeterminded system of linear equation using a double 'vector' for
		A. Note that this method requires  A to be transformed (opposite order of 
		indeces compared to conventional) so that the equation is 
		(A^T)x=b.  This is done for speed optimizing purposes*/
    vector<float> LinearOverDeterminedSolve1(vector<vector <float> >& A, vector<float>& b);

};

/*@}*/ /* end group "utilities" */

} // end namespace ipl

#endif //_IPL98_EQUATIONSOLVER_H
