// Title: Double Array Logic Commands
//
// About:
// This functionset gives you many commands you already know from mel (+ a few extras) wrapped so they work with arrays.
//
// Important conventions:
// All commands don't change the actual data in place, but create a new output.


#include <maya/MDoubleArray.h>
#include <maya/MArgList.h>
#include <math.h>

#include "../include/mHelperFunctions.h"
#include "../include/mDoubleLogicCmd.h"

namespace melfunctions
{


//************************************************************************************************//
/*
   Function: mDblIsEqual

   Check if two mDblArray elements are exactly equal

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      $dblArrayA == $dblArrayB, the result of the elementwise comparison as a float[] of (0,1)

*/
#define mel mDblIsEqual(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblIsEqual)
MStatus mDblIsEqual::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MDoubleArray dblC(count,0);

	for (unsigned int i=0;i<count;i++)
	{

		
		if (dblA[iterA]==dblB[iterB])
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblIsNotEqual

   Check if  mDblArray elements  are not  equal

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      $dblArrayA != $dblArrayB, the result of the elementwise comparison as a float[] of (0,1)

*/
#define mel mDblIsNotEqual(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblIsNotEqual)
MStatus mDblIsNotEqual::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;


	MDoubleArray dblC(count,0);

	for (unsigned int i=0;i<count;i++)
	{

		
		if (dblA[iterA]!=dblB[iterB])
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblIsEquivalent

   Check if elements of two double arrays are equivalent within a tolerance

   Parameters:

		$dblArrayA - the first matrix array
		$dblArrayB - the second matrix array
		$toleranceArray - the maximum amount of error allowed

   Returns:

      the result of the comparison as a float array of (0,1)

*/

#define mel mDblIsEquivalent(float[] $dblArrayA, float[] $dblArrayB, float[] $toleranceArray);
#undef mel

CREATOR(mDblIsEquivalent)
MStatus mDblIsEquivalent::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB, dblC;
    unsigned int incA, incB, incC, count;
	MStatus stat = getArgDblDblDbl(args, dblA, dblB, dblC, incA, incB, incC, count);
	ERROR_FAIL(stat);


	// do the actual job
	unsigned int iterA, iterB,iterC;
	iterA = iterB = iterC = 0;


	MDoubleArray result(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		double temp = dblA[iterA] - dblB[iterB];
		
		if (temp <0)
			temp *= -1;

		if ((temp <= dblC[iterC]))
			result = 1;

		iterA += incA;
		iterB += incB;
		iterC += incC;
	}

	setResult(result);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblIsBigger

   Check if dblArray element A is bigger than B

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      $dblArrayA > $dblArrayB, the result of the elementwise comparison as a float[] of (0,1)

*/
#define mel mDblIsBigger(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblIsBigger)
MStatus mDblIsBigger::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MDoubleArray dblC(count,0);

	for (unsigned int i=0;i<count;i++)
	{

		
		if (dblA[iterA] > dblB[iterB])
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblIsSmaller

   Check if dblArray element A is smaller than B

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      $dblArrayA < $dblArrayB, the result of the elementwise comparison as a float[] of (0,1)

*/
#define mel mDblIsSmaller(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblIsSmaller)
MStatus mDblIsSmaller::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MDoubleArray dblC(count,0);

	for (unsigned int i=0;i<count;i++)
	{

		
		if (dblA[iterA] < dblB[iterB])
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}


//************************************************************************************************//
/*
   Function: mDblNot

   Boolean logic NOT, dbl value is considered TRUE if not 0, FALSE if it equals 0

   Parameters:

		$dblArrayA - the double array

   Returns:

      !$dblArrayA , the result of the operation as a float[] of (0,1)

*/
#define mel mDblNot(float[] $dblArrayA);
#undef mel
CREATOR(mDblNot)
MStatus mDblNot::doIt( const MArgList& args )
{
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgDbl(args, dblA, count);
	ERROR_FAIL(stat);
	for (unsigned int i=0;i<dblA.length();i++)
        dblA[i] = !dblA[i];
	setResult(dblA);
	return MS::kSuccess;
}


//************************************************************************************************//
/*
   Function: mDblAnd

   Boolean logic AND, dbl value is considered TRUE if a and b are true

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      $dblArrayA && $dblArrayB, the result of the operation as a float[] of (0,1)

*/
#define mel mDblAnd(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblAnd)
MStatus mDblAnd::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MDoubleArray dblC(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		if (dblA[iterA] && dblB[iterB])
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblOr

   Boolean logic OR, dbl value is considered TRUE if a or b is true

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      $dblArrayA && $dblArrayB, the result of the operation as a float[] of (0,1)

*/
#define mel mDblOr(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblOr)
MStatus mDblOr::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MDoubleArray dblC(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		if (dblA[iterA] || dblB[iterB])
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblXOr

   Boolean logic Exclusive OR, dbl value is considered true if a or b is true, but not a and b

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      $dblArrayA && $dblArrayB, the result of the operation as a float[] of (0,1)

*/
#define mel mDblXOr(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblXOr)
MStatus mDblXOr::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MDoubleArray dblC(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		if ((dblA[iterA] || dblB[iterB]) &&  (!(dblA[iterA] && dblB[iterB])))
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

}// namespace
