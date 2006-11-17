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

// Title: UV Array Management Commands
//
// About:
// Common management functions for UV arrays.
//
// Important conventions:
// A mUV in melfunctions is represented as an array of 2 floats.
// All commands don't change the actual data in place, but create a new output.


#include <maya/MDoubleArray.h>
#include <maya/MArgList.h>

#include "../include/mHelperFunctions.h"
#include "../include/mUVManagementCmd.h"

namespace melfunctions
{

/*
   Function: mUVCreate
   Create a UV array

   Parameters:

		none - return the null UV
		or
		count - return count null UVs 
		or
		count - the number of UV elements to create
		template - the template UV which will be used for creation

   Returns:

      The UV array  as a float array of its elements

*/
#define mel mUVCreate(int $count, float $template[2]);
#undef mel

CREATOR(mUVCreate)
MStatus mUVCreate::doIt( const MArgList& args )
{
	MStatus stat;
	int count = 1;

    double2 uv={0.0,0.0};
        	
	// check how many arguments we have
	if (args.length() == 0)
	{
		// no args
		// just return a single uvtor
		setResult(createArrayFromUV(count,uv));
		return MS::kSuccess;
	}
	else if (args.length() == 1)
	{
		// one args, check if its an int - if yes return int identity arrays
		stat = getIntArg(args, 0, count);
		ERROR_FAIL(stat);
		setResult(createArrayFromUV(count,uv));
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

		// check if the arguments is a valid single uvtor
		if (dblA.length() != ELEMENTS_UV)
		{
			ERROR_ARG(MS::kFailure,1);
		}

		double2 temp;
		getUVFromArray(dblA,0,temp);

		setResult(createArrayFromUV(count,temp));
		return MS::kSuccess;
	}
	else
	{
		ERROR_FAIL(MS::kFailure);	
	}
}

/*
   Function: mUVSize
   Get number of uvtors in a UV array

   Parameters:
		uvArrayA - UV array 
   Returns:

      The size of the UV array

*/
#define mel int mUVSize(float $uvArrayA[]);
#undef mel

CREATOR(mUVSize)
MStatus mUVSize::doIt( const MArgList& args )
{
    // get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgUV(args, dblA, count);
	ERROR_FAIL(stat);
    
    setResult(int(count));

	return MS::kSuccess;
}


/*
   Function: mUVAppend
   Append elements to the end of a UV array

   Parameters:
		uvArrayA - UV array to extract elements to
		uvArrayB - UV array to append
   Returns:

      The new UV array of as a float array of its elements

*/
#define mel mUVAppend(float $uvArrayA[], float $uvArrayB[]);
#undef mel

CREATOR(mUVAppend)
MStatus mUVAppend::doIt( const MArgList& args )
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
    
    // check if they are valid UV arrays
	unsigned int numA, numB, numC;
	stat = uvIsValid(dblA,numA);
	ERROR_ARG(stat,1);

	stat = uvIsValid(dblB,numB);
	ERROR_ARG(stat,2);
    
        
    // append the two arrays
    MDoubleArray result = dblA;
    for(int i =0; i< dblB.length(); i++)
        result.append(dblB[i]);
    
	setResult(result);
	return MS::kSuccess;
}


/*
   Function: mUVGet
   Get elements from a UV array

   Parameters:
		uvArrayA - UV array to extract elements from
		ids	- int array of ids to extraxt from the UV array
		optional
		uvArrayB - UV array that will be used as a substitution if an id does not exist in array A, must be 1 or same size than id array
   Returns:

      The UV array of as a float array of its elements

*/
#define mel mUVGet(float $uvArrayA[], int $id[], float $uvArrayB[]);
#undef mel

CREATOR(mUVGet)
MStatus mUVGet::doIt( const MArgList& args )
{
	MStatus stat;
	unsigned int count = 1;
	
	// check how many arguments we have
	if (args.length() == 2)
	{
		MDoubleArray uvA, dblB;
		// just extract the elements
		stat = getDoubleArrayArg(args,0,uvA);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,1,dblB);
		ERROR_FAIL(stat);

		// check if the arguments are valid UV arrays
		unsigned int numA, numB;
		stat = uvIsValid(uvA,numA);
		ERROR_ARG(stat,1);

		count = dblB.length();

		// build a result array
		MDoubleArray result = createEmptyUVArray(count);
		
		double2 uv;

		for (unsigned int i=0;i<count;i++)
		{
			int id = int(dblB[i]);
			if ((id > -1) && (id<numA))
			{
				getUVFromArray(uvA,id,uv);
				setUVToArray(result,i,uv);
			}
			else
			{
				// we've got an invalid index
				MString err = "mUVGet:id array has an invalid index '";
				err = err + id +"' at position '"+i+"'!";
				USER_ERROR_CHECK(MS::kFailure,err)
			}
		}

		setResult(result);

		return MS::kSuccess;
	}
	else if (args.length() == 3)
	{

		MDoubleArray uvA, dblB, uvC;
		// just extract the elements
		stat = getDoubleArrayArg(args,0,uvA);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,1,dblB);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,2,uvC);
		ERROR_FAIL(stat);

		// check if the arguments are valid UV arrays
		unsigned int numA, numB, numC;
		stat = uvIsValid(uvA,numA);
		ERROR_ARG(stat,1);

		numB = dblB.length();

		stat = uvIsValid(uvC,numC);
		ERROR_ARG(stat,3);

		// check the counts
		unsigned int incB, incC, count;
		stat = twoArgCountsValid(numB,numC,incB,incC,count);
		ERROR_FAIL(stat);


		// build a result array
		MDoubleArray result = createEmptyUVArray(count);
		unsigned idC =0;
		double2 uv;

		for (unsigned int i=0;i<count;i++)
		{
			int id = int(dblB[i]);
			if ((id > -1) && (id<numA))
			{
				getUVFromArray(uvA,id,uv);
				setUVToArray(result,i,uv);
			}
			else
			{
                getUVFromArray(uvC,idC,uv);
				setUVToArray(result,i,uv);
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
   Function: mUVSet
   Set elements in a UV array, this function will not grow the UV array, but error when you try to set an invalid array element!

   Parameters:
		uvArrayA - base UV array
		ids	- int array of ids defining where in A to insert B
		uvArrayB - UV array of elements to be inserted into A, must be size 1 or same size than id array
   Returns:

      The UV arrayA with elements inserted of of as a float array of its elements

*/
#define mel mUVSet(float $uvArrayA[], int $id[], float $uvArrayB[]);
#undef mel

CREATOR(mUVSet)
MStatus mUVSet::doIt( const MArgList& args )
{
	unsigned int count = 1;

	MStatus stat = argCountCheck(args,3); 
	ERROR_FAIL(stat);    
	
    MDoubleArray uvA, dblB, uvC;
    // just extract the elements
    stat = getDoubleArrayArg(args,0,uvA);
    ERROR_FAIL(stat);

    stat = getDoubleArrayArg(args,1,dblB);
    ERROR_FAIL(stat);

    stat = getDoubleArrayArg(args,2,uvC);
    ERROR_FAIL(stat);

    // check if the arguments are valid UV arrays
    unsigned int numA, numB, numC;
    stat = uvIsValid(uvA,numA);
    ERROR_ARG(stat,1);

    numB = dblB.length();

    stat = uvIsValid(uvC,numC);
    ERROR_ARG(stat,3);

    // check the counts
    unsigned int incB, incC;
    stat = twoArgCountsValid(numB,numC,incB,incC,count);
    ERROR_FAIL(stat);


    // build a result array
    MDoubleArray result = uvA;
    
    int uvCId =0;

    double2 uv;

    for (unsigned int i=0;i<count;i++)
    {
       int id = int(dblB[i]);
       if ((id > -1) && (id<numA))
       {
           getUVFromArray(uvC,uvCId,uv);
           setUVToArray(result,id,uv);
       }
       else
       {
           // we've got an invalid index
           MString err = "mUVSet:id array has an invalid index '";
           err = err + id +"' at position '"+i+"'!";
           USER_ERROR_CHECK(MS::kFailure,err)
       }
       
       uvCId += incC;
   }

   setResult(result);

   return MS::kSuccess;    
}




/*
   Function: mUVGetU

   Get UV component X within a UV array.

   Parameters:

		$uvArrayA - the UV array

   Returns:

  the elements at $vec.x as a float array

*/
#define mel mUVGetU(float $uvArrayA[]);
#undef mel

CREATOR(mUVGetU)
MStatus mUVGetU::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgUV(args, dblA, count);
	ERROR_FAIL(stat);

	// do the job
	MDoubleArray result = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
		result[i] = dblA[i*ELEMENTS_UV];

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mUVGetV

   Get UV component Y within a UV array.

   Parameters:

		$uvArrayA - the UV array

   Returns:

  the elements at $vec.y as a float array

*/
#define mel mUVGetV(float $uvArrayA[]);
#undef mel

CREATOR(mUVGetV)
MStatus mUVGetV::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgUV(args, dblA, count);
	ERROR_FAIL(stat);
    
	// do the job
	MDoubleArray dblE = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
		dblE[i] = dblA[i*ELEMENTS_UV+1];

	setResult(dblE);
	return MS::kSuccess;
}



/*
   Function: mUVGetComponent

   Get UV component at specified poistion within a UV array.

   Parameters:

		$uvArrayA - the UV array
		$component - the index [0-2] as an int array

   Returns:

  the element at $vec[$component] as a float array

*/
#define mel mUVGetComponent(float $uvArrayA[], int $component[]);
#undef mel

CREATOR(mUVGetComponent)
MStatus mUVGetComponent::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgUVDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA  = iterB = 0;

	double2 uvA;
	MDoubleArray dblE = MDoubleArray(count,0);
	unsigned int c;
    
	for (unsigned int i=0;i<count;i++)
	{
		getUVFromArray(dblA,iterA,uvA);

		// check element is valid
		c = int(dblB[iterB]);
		if (!validVecIndex(c))
		{
			MString err="vector element id error at index ";
			err+=i+"!";
			ERROR_FAIL(MS::kFailure);
		}
		dblE[i] = uvA[c];

		iterA += incA;
		iterB += incB;
	}

	setResult(dblE);
	return MS::kSuccess;
}
/*
   Function: mUVSetU

   Set UV component X 

   Parameters:

		$uvArrayA - the UV array
		$value - the double array to be set into the component

   Returns:

  the UV array with the inserted as a float array

*/
#define mel mUVSetU(float $uvArrayA[], float $value[]);
#undef mel

CREATOR(mUVSetU)
MStatus mUVSetU::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgUVDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA  = iterB =0;
    double2 uvA;

	MDoubleArray result = createEmptyUVArray(count);
        

	for (unsigned int i=0;i<count;i++)
	{
		getUVFromArray(dblA,iterA,uvA);
		uvA[0] = dblB[iterB];
   		setUVToArray(result,i,uvA);

		iterA += incA;
		iterB += incB;
	}

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mUVSetV

   Set UV component X 

   Parameters:

		$uvArrayA - the UV array
		$value - the double array to be set into the component

   Returns:

  the UV array with the inserted as a float array

*/
#define mel mUVSetV(float $uvArrayA[], float $value[]);
#undef mel

CREATOR(mUVSetV)
MStatus mUVSetV::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgUVDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA  = iterB =0;
    double2 uvA;

	MDoubleArray result = createEmptyUVArray(count);
        
	unsigned int c;
	for (unsigned int i=0;i<count;i++)
	{
		getUVFromArray(dblA,iterA,uvA);
		uvA[1] = dblB[iterB];
   		setUVToArray(result,i,uvA);

		iterA += incA;
		iterB += incB;
	}


	setResult(result);
	return MS::kSuccess;
}


/*
   Function: mUVSetComponent

   Set UV component at specified poistion within a UV array.

   Parameters:

		$uvArrayA - the UV array
		$component - the index [0-2] as an int array
		$value - the double array to be set into the component

   Returns:

  the UV array with the inserted as a float array

*/
#define mel mUVSetComponent(float $uvArrayA[], int $component[], float $value[]);
#undef mel

CREATOR(mUVSetComponent)
MStatus mUVSetComponent::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB,dblC;
    unsigned int incA, incB, incC,count;
	MStatus stat = getArgUVDblDbl(args, dblA, dblB, dblC,incA, incB,incC, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB, iterC;
	iterA  = iterB = iterC =0;

	double2 uvA;
    MDoubleArray result = createEmptyUVArray(count);

	unsigned int c;
	for (unsigned int i=0;i<count;i++)
	{
		getUVFromArray(dblA,iterA,uvA);

		// check component is valid
		c = int(dblB[iterB]);
		if (!validVecIndex(c))
		{
			MString err="mUVSetComponent:uv component id error at index ";
			err+=i+", not in valid range [0,1]!";
			ERROR_FAIL(MS::kFailure);
		}
        
        uvA[c] = dblC[iterC];
        
		setUVToArray(result,i,uvA);

		iterA += incA;
		iterB += incB;
		iterC += incC;
	}

	setResult(result);
	return MS::kSuccess;
}

} // end namespace
