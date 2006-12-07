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

// Title: Matrix Array Management Commands
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
#include "../include/mMatrixManagementCmd.h"

namespace melfunctions
{



/*
   Function: mMatCreate
   Create a matrix array

   Parameters:

		none - return the identity matrix
		or
		count - return count identity matrices
		or
		count - the number of matrix elements to create
		template - the template matrix which will be used for creation

   Returns:

      The matrix array of as a float array of its elements

*/
#define mel mMatCreate(int $count, float $template[16]);
#undef mel

CREATOR(mMatCreate)
MStatus mMatCreate::doIt( const MArgList& args )
{
	MStatus stat;
	int count = 1;
	
	// check how many arguments we have
	if (args.length() == 0)
	{
		// no args
		// just return a single matrix
		setResult(createArrayFromMat(count,MMatrix::identity));
		return MS::kSuccess;
	}
	else if (args.length() == 1)
	{
		// one args, check if its an int - if yes return int identity arrays
		stat = getIntArg(args, 0, count);
		ERROR_FAIL(stat);
		setResult(createArrayFromMat(count,MMatrix::identity));
		return MS::kSuccess;
	}
	else if (args.length() == 2)
	{

		stat = getIntArg(args, 0, count);
		ERROR_FAIL(stat);

		// get dbl array from the arguments
		MDoubleArray dblA;
		stat = getDoubleArrayArg(args,1,dblA);
		ERROR_FAIL(stat);

		// check if the arguments is a valid single vector
		if (dblA.length() != ELEMENTS_MAT)
		{
			ERROR_ARG(MS::kFailure,1);
		}

		MMatrix temp;
		getMatFromArray(dblA,0,temp);

		setResult(createArrayFromMat(count,temp));
		return MS::kSuccess;
	}
	else
	{
		ERROR_FAIL(MS::kFailure);	
	}
}

/*
   Function: mMatSize
   Get number of matrices in a matrix array

   Parameters:
		matrixArrayA - matrix array 
   Returns:

      The size of the matrix array

*/
#define mel int mMatSize(float $matrixArrayA[]);
#undef mel

CREATOR(mMatSize)
MStatus mMatSize::doIt( const MArgList& args )
{
    // get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgMat(args, dblA, count);
	ERROR_FAIL(stat);
    
    setResult(int(count));

	return MS::kSuccess;
}


/*
   Function: mMatAppend
   Append elements to the end of a matrix array

   Parameters:
		matrixArrayA - matrix array to extract elements to
		matrixArrayB - matrix array to append
   Returns:

      The new matrix array of as a float array of its elements

*/
#define mel mMatAppend(float $matrixArrayA[], float $matrixArrayB[]);
#undef mel

CREATOR(mMatAppend)
MStatus mMatAppend::doIt( const MArgList& args )
{
	// check the argument count
	MStatus stat = argCountCheck(args,2); 
	ERROR_FAIL(stat);

	// get the arguments
    MDoubleArray dblA, dblB;

	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,dblA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);
    
    // check if they are valid matrix arrays
	unsigned int numA, numB, numC;
	stat = matIsValid(dblA,numA);
	ERROR_ARG(stat,1);

	stat = matIsValid(dblB,numB);
	ERROR_ARG(stat,2);
    
        
    // append the two arrays
    MDoubleArray result = dblA;
    for(int i =0; i< dblB.length(); i++)
        result.append(dblB[i]);
    
	setResult(result);
	return MS::kSuccess;
}


/*
   Function: mMatGet
   Get elements from a matrix array

   Parameters:
		matrixArrayA - matrix array to extract elements from
		ids	- int array of ids to extraxt from the matrix array
		optional
		matrixArrayB - matrix array that will be used as a substitution if an id does not exist in array A, must be 1 or same size than id array
   Returns:

      The matrix array of as a float array of its elements

*/
#define mel mMatGet(float $matrixArrayA[], int $id[], float $matrixArrayB[]);
#undef mel

CREATOR(mMatGet)
MStatus mMatGet::doIt( const MArgList& args )
{
	MStatus stat;
	unsigned int count = 1;
	
	// check how many arguments we have
	if (args.length() == 2)
	{
		MDoubleArray matA, dblB;
		// just extract the elements
		stat = getDoubleArrayArg(args,0,matA);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,1,dblB);
		ERROR_FAIL(stat);

		// check if the arguments are valid mattor arrays
		unsigned int numA, numB;
		stat = matIsValid(matA,numA);
		ERROR_ARG(stat,1);

		count = dblB.length();

		// build a result array
		MDoubleArray result = createEmptyMatArray(count);
		
		MMatrix mat;

		for (unsigned int i=0;i<count;i++)
		{
			int id = int(dblB[i]);
			if ((id > -1) && (id<numA))
			{
				getMatFromArray(matA,id,mat);
				setMatToArray(result,i,mat);
			}
			else
			{
				// we've got an invalid index
				MString err = "id array has an invalid index '";
				err = err + id +"' at position '"+i+"'!";
				USER_ERROR_CHECK(MS::kFailure,err)
			}
		}

		setResult(result);

		return MS::kSuccess;
	}
	else if (args.length() == 3)
	{

		MDoubleArray matA, dblB, matC;
		// just extract the elements
		stat = getDoubleArrayArg(args,0,matA);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,1,dblB);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,2,matC);
		ERROR_FAIL(stat);

		// check if the arguments are valid mattor arrays
		unsigned int numA, numB, numC;
		stat = matIsValid(matA,numA);
		ERROR_ARG(stat,1);

		numB = dblB.length();

		stat = matIsValid(matC,numC);
		ERROR_ARG(stat,3);

		// check the counts
		unsigned int incB, incC, count;
		stat = twoArgCountsValid(numB,numC,incB,incC,count);
		ERROR_FAIL(stat);


		// build a result array
		MDoubleArray result = createEmptyMatArray(count);
		unsigned idC =0;
		MMatrix mat;

		for (unsigned int i=0;i<count;i++)
		{
			int id = int(dblB[i]);
			if ((id > -1) && (id<numA))
			{
				getMatFromArray(matA,id,mat);
				setMatToArray(result,i,mat);
			}
			else
			{
                getMatFromArray(matC,idC,mat);
				setMatToArray(result,i,mat);
			}
            idC += incC;
		}

		setResult(result);

		return MS::kSuccess;	
    }
	else
	{
		ERROR_FAIL(MS::kFailure);	
	}
}


/*
   Function: mMatSet
   Set elements in a matrix array, this function will not grow the matrix array, but error when you try to set an invalid array element!

   Parameters:
		matrixArrayA - base matrix array
		ids	- int array of ids defining where in A to insert B
		matrixArrayB - matrix array of elements to be inserted into A, must be size 1 or same size than id array
   Returns:

      The matrix arrayA with elements inserted of of as a float array of its elements

*/
#define mel mMatSet(float $matrixArrayA[], int $id[], float $matrixArrayB[]);
#undef mel

CREATOR(mMatSet)
MStatus mMatSet::doIt( const MArgList& args )
{
	unsigned int count = 1;

	MStatus stat = argCountCheck(args,3); 
	ERROR_FAIL(stat);    
	
    MDoubleArray matA, dblB, matC;
    // just extract the elements
    stat = getDoubleArrayArg(args,0,matA);
    ERROR_FAIL(stat);

    stat = getDoubleArrayArg(args,1,dblB);
    ERROR_FAIL(stat);

    stat = getDoubleArrayArg(args,2,matC);
    ERROR_FAIL(stat);

    // check if the arguments are valid mattor arrays
    unsigned int numA, numB, numC;
    stat = matIsValid(matA,numA);
    ERROR_ARG(stat,1);

    numB = dblB.length();

    stat = matIsValid(matC,numC);
    ERROR_ARG(stat,3);

    // check the counts
    unsigned int incB, incC;
    stat = twoArgCountsValid(numB,numC,incB,incC,count);
    ERROR_FAIL(stat);


    // build a result array
    MDoubleArray result = matA;
    
    int matCId =0;

    MMatrix mat;

    for (unsigned int i=0;i<count;i++)
    {
       int id = int(dblB[i]);
       if ((id > -1) && (id<numA))
       {
           getMatFromArray(matC,matCId,mat);
           setMatToArray(result,id,mat);
       }
       else
       {
           // we've got an invalid index
           MString err = "id array has an invalid index '";
           err = err + id +"' at position '"+i+"'!";
           USER_ERROR_CHECK(MS::kFailure,err)
       }
       
       matCId += incC;
   }

   setResult(result);

   return MS::kSuccess;    
}


/*
   Function: mMatGetComponent

   Get matrix component at specified row and columns within a matrix array.

   Parameters:

		$matArrayA - the matrix array
		$row - the row index [0-3] as an int array
		$column - the column index [0-3] as an int array

   Returns:

  the element at mat[row][column] as a float array

*/
#define mel mMatGetComponent(float[] $matArrayA, int[] $row, int[] $column);
#undef mel

CREATOR(mMatGetComponent)
MStatus mMatGetComponent::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB, dblC;
    unsigned int incA, incB,incC, count;
	MStatus stat = getArgMatDblDbl(args, dblA, dblB, dblC, incA, incB, incC,count);
	ERROR_FAIL(stat);


	// do the actual job
	unsigned int iterA, iterB,iterC;
	iterA = iterB = iterC = 0;

	MMatrix matA;
	MDoubleArray result = MDoubleArray(count,0);
	unsigned int r,c;
	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,iterA,matA);

		// check element is valid
		r = int(dblB[iterB]);
		c = int(dblC[iterC]);
		if (!validMatIndex(r,c))
		{
			MString err="matrix element id error at index ";
			err+=i+"!";
			ERROR_FAIL(MS::kFailure);
		}
		result[i] = matA[r][c];

		iterA += incA;
		iterB += incB;
		iterC += incC;
	}

	setResult(result);
	return MS::kSuccess;
}
/*
   Function: mMatSetComponent

   Set a single component of an entry in a matrix array.

   Parameters:

		$matArray - the matrix array
		$row - the row index [0-3] as an int array
		$column - the column index [0-3] as an int array
		$value - the value to insert as an float array

   Returns:

  the matrix rray with the newly inserted value at [row][column] as a float[]

*/
#define mel mMatSetComponent(float[] $matArrayA, int[] $row, int[] $column, float[] $value);
#undef mel

CREATOR(mMatSetComponent)
MStatus mMatSetComponent::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB, dblC, dblD;
    unsigned int incA, incB,incC, incD, count;
	MStatus stat = getArgMatDblDblDbl(args, dblA, dblB, dblC, dblD, incA, incB, incC, incD, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB,iterC, iterD;
	iterA = iterB = iterC = iterD =0;

	MMatrix matA;
	MDoubleArray dblE = createEmptyMatArray(count);

	unsigned int r,c;
	for (unsigned int i=0;i<count;i++)
	{
		getMatFromArray(dblA,iterA,matA);

		// check element is valid
		r = int(dblB[iterB]);
		c = int(dblC[iterC]);
		if (!validMatIndex(r,c))
		{
			MString err="matrix element id error at index ";
			err+=i+"!";
			ERROR_FAIL(MS::kFailure);
		}
		matA[r][c] = dblD[iterD];
		setMatToArray(dblE,i,matA);

		iterA += incA;
		iterB += incB;
		iterC += incC;
		iterD += incD;
	}

	setResult(dblE);
	return MS::kSuccess;
}


} // end namespace
