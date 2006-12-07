/* COPYRIGHT --
 *
 * This file is part of melfunctions, a collection of mel commands to for Autodesk Maya.
 * melfunctions is (c) 2006 Carsten Kolve <carsten@kolve.com>
 * and distributed under the terms of the GNU GPL V2.
 * See the ./License-GPL.txt file in the source tree root for more information.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

// Title: Matrix Array Math Commands
//
// About:
// All you ever wanted to do with mMatrices in MEL. Well, this description might be going a little too far, but 
// proper mMatrix support is something that is missing in MEL and melfunctions gives you access to some of the hidden
// functionalities. OK, there is a mMatrix datatype and you can  
// retrieve a mMatrix from transforms using the "getAttr" and "xform" commands, but actually working with them
// is not really possible. To overcome this limitation melfunctions has a bunch of commonly used mMatrix 
// functions. Most of them are just wrappers for the mMatrix functions provided by the Maya API.
//
// Important conventions:
// A mMatrix in melfunctions is represented as an array of 16 floats. The values are in row order (that means the first
// 4 values belong to the first row of the mMatrix, the next 4 to the second... and so on)
// All commands don't change the actual data in place, but create a new output (that might change in the future).


#include <maya/MMatrix.h>
#include <maya/MDoubleArray.h>
#include <maya/MArgList.h>



#include "../include/mHelperFunctions.h"
#include "../include/mMatrixMathCmd.h"

namespace melfunctions
{


/*
   Function: mMatAdd

   Add two matrix arrays together elementwise

   Parameters:

		$matArrayA - the first matrix array
		$matArrayB - the second matrix array

   Returns:

      $matArrayA + $maArrayB, the sum of the two matrix arrays elementswise as a float[]

*/
#define mel mMatAdd(float[] $matArrayArrayA, float[] $matArrayArrayB);
#undef mel

CREATOR(mMatAdd)
MStatus mMatAdd::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgMatMat(args, dblA, dblB, incA, incB, count);
	ERROR_ARG_FAIL(stat);

//    cerr <<"\nadd count: "<<count;
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MMatrix matA,matB,matC;
	MDoubleArray dblC = createEmptyMatArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,iterA,matA);
		getMatFromArray(dblB,iterB,matB);
		matC = matA+matB;
		setMatToArray(dblC,i,matC);

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mMatSub

   Subtract two matrix arrays from each other elementwise

   Parameters:

		$matArrayA - the first matrix array
		$matArrayB - the second matrix array

   Returns:

      $matArrayA + $maArrayB, the difference of the two matrix arrays elementswise as a float[]

*/
#define mel mMatSub(float[] $matArrayArrayA, float[] $matArrayArrayB);
#undef mel

CREATOR(mMatSub)
MStatus mMatSub::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgMatMat(args, dblA, dblB, incA, incB, count);
	ERROR_ARG_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MMatrix matA,matB,matC;
	MDoubleArray dblC = createEmptyMatArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,iterA,matA);
		getMatFromArray(dblB,iterB,matB);
		matC = matA-matB;
		setMatToArray(dblC,i,matC);

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mMatMult

   Multiply matrix with matrix of arrays elementwise

   Parameters:

		$matArrayA - the first matrix array
		$matArrayB - the second matrix array

   Returns:

      $matArrayA[] * $matArrayB[], the result of the matrix multiplication as a float[]


*/
#define mel mMatMult(float[] $matArrayArrayA, float[] $matArrayArrayB);
#undef mel

CREATOR(mMatMult)
MStatus mMatMult::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgMatMat(args, dblA, dblB, incA, incB, count);
	ERROR_ARG_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MMatrix matA,matB,matC;
	MDoubleArray dblC = createEmptyMatArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,iterA,matA);
		getMatFromArray(dblB,iterB,matB);
		matC = matA*matB;
		setMatToArray(dblC,i,matC);

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mMatDblMult

   Multiply matrix (array) with double (array) elementwise

   Parameters:

		$matArray - the matrix array
		$dblArray - the double array

   Returns:

      $matArray[] * $dblArray[], the result of the multiplication as a float[]


*/
#define mel mMatDblMult(float[] $matArray, float[] $dblArray);
#undef mel

CREATOR(mMatDblMult)
MStatus mMatDblMult::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgMatDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MMatrix matA,matC;
	MDoubleArray dblC = createEmptyMatArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,iterA,matA);
		matC = matA*dblB[iterB];
		setMatToArray(dblC,i,matC);

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mMatIsEqual

   Check if two mMatrices are exactly equal

   Parameters:

		$matArrayArrayA - the first matrix array
		$matArrayArrayB - the second matrix array

   Returns:

      $matArrayArrayA == $matArrayArrayB, the result of the elementwise comparison as a float[] of (0,1)

*/
#define mel mMatIsEqual(float[] $matArrayArrayA, float[] $matArrayArrayB);
#undef mel

CREATOR(mMatIsEqual)
MStatus mMatIsEqual::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgMatMat(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
    
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MMatrix matA,matB;
	MDoubleArray dblC = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,iterA,matA);
		getMatFromArray(dblB,iterB,matB);
		
		if (matA==matB)
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mMatIsNotEqual

   Check if two mMatrices are exactly not equal

   Parameters:

		$matArrayArrayA - the first matrix array
		$matArrayArrayB - the second matrix array

   Returns:

      $matArrayArrayA != $matArrayArrayB, the result of the elementwise comparison as a float[] of (0,1)

*/
#define mel mMatIsNotEqual(float[] $matArrayArrayA, float[] $matArrayArrayB);
#undef mel

CREATOR(mMatIsNotEqual)
MStatus mMatIsNotEqual::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgMatMat(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
    
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MMatrix matA,matB;
	MDoubleArray dblC = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,iterA,matA);
		getMatFromArray(dblB,iterB,matB);
		
		if (matA!=matB)
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mMatIsEquivalent

   Check if elements of two matrix arrays are equivalent within a tolerance

   Parameters:

		$matArrayArrayA - the first matrix array
		$matArrayArrayB - the second matrix array
		$toleranceArray - the maximum amount of error allowed

   Returns:

      the result of the comparison as a float array of (0,1)

*/

#define mel mMatIsEquivalent(float[] $matArrayArrayA, float[] $matArrayArrayB), float[] $toleranceArray);
#undef mel

CREATOR(mMatIsEquivalent)
MStatus mMatIsEquivalent::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB, dblC;
    unsigned int incA, incB,incC, count;
	MStatus stat = getArgMatMatDbl(args, dblA, dblB, dblC, incA, incB, incC,count);
	ERROR_FAIL(stat);
    

	// do the actual job
	unsigned int iterA, iterB,iterC;
	iterA = iterB = iterC = 0;

	MMatrix matA,matB;
	MDoubleArray result = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,iterA,matA);
		getMatFromArray(dblB,iterB,matB);
		
		result[i] = matA.isEquivalent(matB,dblC[iterC]);

		iterA += incA;
		iterB += incB;
		iterC += incC;
	}

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mMatIsSingular

   Check if a matrices in an array are singular (do not have an inverse).

   Parameters:

		$matArrayArrayA - the matrix array

   Returns:

   a float array (0,1) indicating wether the element of a matrix array is singular or not
	// get the arguments
    MDoubleArray dblA, dblB, dblC;
    unsigned int incA, incB,incC, count;
	MStatus stat = getArgMatMat(args, dblA, dblB, dblC, incA, incB, incC,count);
	ERROR_FAIL(stat);

*/
#define mel mMatIsSingular(float[] $matArrayArrayA);
#undef mel

CREATOR(mMatIsSingular)
MStatus mMatIsSingular::doIt( const MArgList& args )
{
// get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgMat(args, dblA, count);
	ERROR_FAIL(stat);


	MMatrix matA;
	MDoubleArray dblC = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,i,matA);
		dblC[i] = matA.isSingular();
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mMatInverse

   Get the inverse of a the elements of a matrix array.

   Parameters:

		$matArrayA - the matrix array

   Returns:

   the inverse of the elements of a matrix array as a float[]

*/
#define mel mMatInverse(float[] $matArrayA);
#undef mel

CREATOR(mMatInverse)
MStatus mMatInverse::doIt( const MArgList& args )
{
// get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgMat(args, dblA, count);
	ERROR_FAIL(stat);

	// do the actual job
	MMatrix matA,matC;
	MDoubleArray dblC = createEmptyMatArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,i,matA);
		matC = matA.inverse();
		setMatToArray(dblC,i,matC);
	}

	setResult(dblC);
	return MS::kSuccess;
}
/*
   Function: mMatTranspose

   Get the transpose of of the elements of a matrix array

   Parameters:

		$matArrayA - the mMatrix

   Returns:

  the transpose of the elements of a matrix array as a float[]

*/
#define mel mMatTranspose(float[] $matArrayA);
#undef mel

CREATOR(mMatTranspose)
MStatus mMatTranspose::doIt( const MArgList& args )
{
    // get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgMat(args, dblA, count);
	ERROR_FAIL(stat);

	// do the actual job
	MMatrix matA,matC;
	MDoubleArray dblC = createEmptyMatArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,i,matA);
		matC = matA.transpose();
		setMatToArray(dblC,i,matC);
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mMatHomogenize

   Get the homogenized version o the elements of a matrix array

   Parameters:

		$matArrayA - the mMatrix

   Returns:

  the homogenized elements of a matrix array as a float[]

*/
#define mel mMatHomogenize(float[] $matArrayA);
#undef mel

CREATOR(mMatHomogenize)
MStatus mMatHomogenize::doIt( const MArgList& args )
{
// get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgMat(args, dblA, count);
	ERROR_FAIL(stat);

	// do the actual job
	MMatrix matA,matC;
	MDoubleArray dblC = createEmptyMatArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,i,matA);
		matC = matA.homogenize();
		setMatToArray(dblC,i,matC);
	}

	setResult(dblC);
	return MS::kSuccess;
}


/*
   Function: mMatAdjoint

   Get adjoint (or conjugate transpose) of the elements of a matrix array

   Parameters:

		$matArrayA - the mMatrix

   Returns:

  adjoint of the elements of a matrix array as a float[]

*/
#define mel mMatAdjoint(float[] $matArrayA);
#undef mel

CREATOR(mMatAdjoint)
MStatus mMatAdjoint::doIt( const MArgList& args )
{
// get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgMat(args, dblA, count);
	ERROR_FAIL(stat);

	// do the actual job
	MMatrix matA,matC;
	MDoubleArray dblC = createEmptyMatArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,i,matA);
		matC = matA.adjoint();
		setMatToArray(dblC,i,matC);
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mMatDet4x4

   Get 4x4 determinant of the elements of an matrix array.

   Parameters:

		$matArrayA - the mMatrix

   Returns:

  the determinant as a float

*/
#define mel mMatDet4x4(float[] $matArrayA);
#undef mel

CREATOR(mMatDet4x4)
MStatus mMatDet4x4::doIt( const MArgList& args )
{
// get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgMat(args, dblA, count);
	ERROR_FAIL(stat);

	MMatrix matA;
	MDoubleArray dblC = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,i,matA);
		dblC[i] = matA.det4x4();
	}

	setResult(dblC);
	return MS::kSuccess;
}
/*
   Function: mMatDet3x3

   Get 3x3 (upper right) determinant of a mMatrix.

   Parameters:

		$matArrayA - the mMatrix

   Returns:

  the determinant as a float

*/
#define mel mMatDet3x3(float[] $matArrayA);
#undef mel

CREATOR(mMatDet3x3)
MStatus mMatDet3x3::doIt( const MArgList& args )
{
    // get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgMat(args, dblA, count);
	ERROR_FAIL(stat);

	MMatrix matA;
	MDoubleArray dblC = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,i,matA);
		dblC[i] = matA.det3x3();
	}

	setResult(dblC);
	return MS::kSuccess;
}




} // end namespace
