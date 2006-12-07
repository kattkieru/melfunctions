// Title: Double Array Management Commands
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
#include "../include/mDoubleManagementCmd.h"

namespace melfunctions
{


//************************************************************************************************//
/*
   Function: mDblCreate
   Create a double array

   Parameters:

		none - return a single double value 0
		or
		count - return count null doubles
		or
		count - the number of double elements to create
		template - the template double which will be used for creation

   Returns:

      The double array  as a float array of its elements

*/
#define mel mDblCreate(int $count, float $template);
#undef mel

CREATOR(mDblCreate)
MStatus mDblCreate::doIt( const MArgList& args )
{
	MStatus stat;
	int count = 1;
	
	// check how many arguments we have
	if (args.length() == 0)
	{
		// no args
		// just return a single double
		setResult(MDoubleArray(1,0));
		return MS::kSuccess;
	}
	else if (args.length() == 1)
	{
		// one args, check if its an int - if yes return int identity arrays
		stat = getIntArg(args, 0, count);
		ERROR_FAIL(stat);
		setResult(MDoubleArray(count,0));
		return MS::kSuccess;
	}
	else if (args.length() == 2)
	{

		stat = getIntArg(args, 0, count);
		ERROR_FAIL(stat);

		// get dbl array from the arguments
		double temp;
		stat = getDoubleArg(args, 1, temp);
		ERROR_FAIL(stat);

		setResult(MDoubleArray(count,temp));
		return MS::kSuccess;
	}
	else
	{
		ERROR_FAIL(MS::kFailure);	
	}
}

//************************************************************************************************//
/*
   Function: mDblSize
   Get number of doubles in a double array

   Parameters:
		doubleArrayA - double array 
   Returns:

      The size of the double array

*/
#define mel int mDblSize(float $doubleArrayA[]);
#undef mel

CREATOR(mDblSize)
MStatus mDblSize::doIt( const MArgList& args )
{
    // get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgDbl(args, dblA, count);
	ERROR_FAIL(stat);
    
    setResult(int(count));

	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblAppend
   Append elements to the end of a double array

   Parameters:
		doubleArrayA - double array to extract elements to
		doubleArrayB - double array to append
   Returns:

      The new double array of as a float array of its elements

*/
#define mel mDblAppend(float $doubleArrayA[], float $doubleArrayB[]);
#undef mel

CREATOR(mDblAppend)
MStatus mDblAppend::doIt( const MArgList& args )
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
    
        
    // append the two arrays
    MDoubleArray result = dblA;
    for(unsigned int i =0; i < dblB.length(); i++)
        result.append(dblB[i]);
    
	setResult(result);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblGet
   Get elements from a double array

   Parameters:
		doubleArrayA - double array to extract elements from
		ids	- int array of ids to extraxt from the double array
		optional
		doubleArrayB - double array that will be used as a substitution if an id does not exist in array A, must be 1 or same size than id array
   Returns:

      The double array of as a float array of its elements

*/
#define mel mDblGet(float $doubleArrayA[], int $id[], float $doubleArrayB[]);
#undef mel

CREATOR(mDblGet)
MStatus mDblGet::doIt( const MArgList& args )
{
	MStatus stat;
	unsigned int count = 1;
	
	// check how many arguments we have
	if (args.length() == 2)
	{
		MDoubleArray dblA, dblB;
		// just extract the elements
		stat = getDoubleArrayArg(args,0,dblA);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,1,dblB);
		ERROR_FAIL(stat);

		// check if the arguments are valid double arrays
		int numA;
		numA = dblA.length();

		count = dblB.length();

		// build a result array
		MDoubleArray result(count);
		
		for (unsigned int i=0;i<count;i++)
		{
			int id = int(dblB[i]);
			if ((id > -1) && (id<numA))
			{
				result[i] = dblA[id];
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

		MDoubleArray dblA, dblB, dblC;
		// just extract the elements
		stat = getDoubleArrayArg(args,0,dblA);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,1,dblB);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,2,dblC);
		ERROR_FAIL(stat);

		// check if the arguments are valid double arrays
		int numA, numB, numC;
		numA = dblA.length();
		numB = dblB.length();
		numC = dblC.length();

		// check the counts
		unsigned int incB, incC, count;
		stat = twoArgCountsValid(numB,numC,incB,incC,count);
		ERROR_FAIL(stat);


		// build a result array
		MDoubleArray result(count);
		unsigned idC =0;


		for (unsigned int i=0;i<count;i++)
		{
			int id = int(dblB[i]);

			if ((id > -1) && (id<numA))
			{
				result[i] = dblA[id];
			}
			else
			{
				result[i] = dblC[idC];
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

//************************************************************************************************//
/*
   Function: mDblSet
   Set elements in a double array, this function will not grow the double array, but error when you try to set an invalid array element!

   Parameters:
		doubleArrayA - base double array
		ids	- int array of ids defining where in A to insert B
		doubleArrayB - double array of elements to be inserted into A, must be size 1 or same size than id array
   Returns:

      The double arrayA with elements inserted of of as a float array of its elements

*/
#define mel mDblSet(float $doubleArrayA[], int $id[], float $doubleArrayB[]);
#undef mel

CREATOR(mDblSet)
MStatus mDblSet::doIt( const MArgList& args )
{
	unsigned int count = 1;

	MStatus stat = argCountCheck(args,3); 
	ERROR_FAIL(stat);    
	
    MDoubleArray dblA, dblB, dblC;
    // just extract the elements
    stat = getDoubleArrayArg(args,0,dblA);
    ERROR_FAIL(stat);

    stat = getDoubleArrayArg(args,1,dblB);
    ERROR_FAIL(stat);

    stat = getDoubleArrayArg(args,2,dblC);
    ERROR_FAIL(stat);

    // check if the arguments are valid double arrays
    int numA, numB, numC;
	numA = dblA.length();
    numB = dblB.length();
	numC = dblC.length();

    // check the counts
    unsigned int incB, incC;
    stat = twoArgCountsValid(numB,numC,incB,incC,count);
    ERROR_FAIL(stat);


    // build a result array
    MDoubleArray result = dblA;
    
    int dblCId =0;

    for (unsigned int i=0;i<count;i++)
    {
       int id = int(dblB[i]);
       if ((id > -1) && (id<numA))
       {
           result[id] = dblC[dblCId];
       }
       else
       {
           // we've got an invalid index
           MString err = "id array has an invalid index '";
           err = err + id +"' at position '"+i+"'!";
           USER_ERROR_CHECK(MS::kFailure,err)
       }
       
       dblCId += incC;
   }

   setResult(result);

   return MS::kSuccess;    
}


}// namespace
