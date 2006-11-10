
// Title: Vector Array Management Commands
//
// About:
// This functionset gives you all the commands you already know from mel (+ a few extras) wrapped so they work with arrays.
// Vector arrays are frequently used throughout Maya (eg. in particle systems or deformers) and being able to
// handle them efficiently, greatly reduces the amount of mel work needed to do something useful with them.
//
// Important conventions:
// A mVector in melfunctions is represented as an array of 3 floats.
// All commands don't change the actual data in place, but create a new output.


#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MArgList.h>



#include "../include/mHelperFunctions.h"
#include "../include/mVectorManagementCmd.h"

namespace melfunctions
{



/*
   Function: mVecCreate
   Create a vector array

   Parameters:

		none - return the null vector
		or
		count - return count null vectors
		or
		count - the number of vector elements to create
		template - the template vector which will be used for creation

   Returns:

      The vector array  as a float array of its elements

*/
#define mel mVecCreate(int $count, float $template[3]);
#undef mel

CREATOR(mVecCreate)
MStatus mVecCreate::doIt( const MArgList& args )
{
	MStatus stat;
	int count = 1;
	
	// check how many arguments we have
	if (args.length() == 0)
	{
		// no args
		// just return a single vector
		setResult(createArrayFromVec(count,MVector::zero));
		return MS::kSuccess;
	}
	else if (args.length() == 1)
	{
		// one args, check if its an int - if yes return int identity arrays
		stat = getIntArg(args, 0, count);
		ERROR_FAIL(stat);
		setResult(createArrayFromVec(count,MVector::zero));
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
		if (dblA.length() != ELEMENTS_VEC)
		{
			ERROR_ARG(MS::kFailure,1);
		}

		MVector temp;
		getVecFromArray(dblA,0,temp);

		setResult(createArrayFromVec(count,temp));
		return MS::kSuccess;
	}
	else
	{
		ERROR_FAIL(MS::kFailure);	
	}
}

/*
   Function: mVecSize
   Get number of vectors in a vector array

   Parameters:
		vectorArrayA - vector array 
   Returns:

      The size of the vector array

*/
#define mel int mVecSize(float $vectorArrayA[]);
#undef mel

CREATOR(mVecSize)
MStatus mVecSize::doIt( const MArgList& args )
{
    // get the arguments
    MDoubleArray dblA;
    unsigned int  count;
	MStatus stat = getArgVec(args, dblA, count);
	ERROR_FAIL(stat);
    
    setResult(int(count));

	return MS::kSuccess;
}


/*
   Function: mVecAppend
   Append elements to the end of a vector array

   Parameters:
		vectorArrayA - vector array to extract elements to
		vectorArrayB - vector array to append
   Returns:

      The new vector array of as a float array of its elements

*/
#define mel mVecAppend(float $vectorArrayA[], float $vectorArrayB[]);
#undef mel

CREATOR(mVecAppend)
MStatus mVecAppend::doIt( const MArgList& args )
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
    
    // check if they are valid vector arrays
	unsigned int numA, numB, numC;
	stat = vecIsValid(dblA,numA);
	ERROR_ARG(stat,1);

	stat = vecIsValid(dblB,numB);
	ERROR_ARG(stat,2);
    
        
    // append the two arrays
    MDoubleArray result = dblA;
    for(int i =0; i< dblB.length(); i++)
        result.append(dblB[i]);
    
	setResult(result);
	return MS::kSuccess;
}


/*
   Function: mVecGet
   Get elements from a vector array

   Parameters:
		vectorArrayA - vector array to extract elements from
		ids	- int array of ids to extraxt from the vector array
		optional
		vectorArrayB - vector array that will be used as a substitution if an id does not exist in array A, must be 1 or same size than id array
   Returns:

      The vector array of as a float array of its elements

*/
#define mel mVecGet(float $vectorArrayA[], int $id[], float $vectorArrayB[]);
#undef mel

CREATOR(mVecGet)
MStatus mVecGet::doIt( const MArgList& args )
{
	MStatus stat;
	unsigned int count = 1;
	
	// check how many arguments we have
	if (args.length() == 2)
	{
		MDoubleArray vecA, dblB;
		// just extract the elements
		stat = getDoubleArrayArg(args,0,vecA);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,1,dblB);
		ERROR_FAIL(stat);

		// check if the arguments are valid vector arrays
		unsigned int numA, numB;
		stat = vecIsValid(vecA,numA);
		ERROR_ARG(stat,1);

		count = dblB.length();

		// build a result array
		MDoubleArray result = createEmptyVecArray(count);
		
		MVector vec;

		for (unsigned int i=0;i<count;i++)
		{
			int id = int(dblB[i]);
			if ((id > -1) && (id<numA))
			{
				getVecFromArray(vecA,id,vec);
				setVecToArray(result,i,vec);
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

		MDoubleArray vecA, dblB, vecC;
		// just extract the elements
		stat = getDoubleArrayArg(args,0,vecA);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,1,dblB);
		ERROR_FAIL(stat);

		stat = getDoubleArrayArg(args,2,vecC);
		ERROR_FAIL(stat);

		// check if the arguments are valid vector arrays
		unsigned int numA, numB, numC;
		stat = vecIsValid(vecA,numA);
		ERROR_ARG(stat,1);

		numB = dblB.length();

		stat = vecIsValid(vecC,numC);
		ERROR_ARG(stat,3);

		// check the counts
		unsigned int incB, incC, count;
		stat = twoArgCountsValid(numB,numC,incB,incC,count);
		ERROR_FAIL(stat);


		// build a result array
		MDoubleArray result = createEmptyVecArray(count);
		unsigned idC =0;
		MVector vec;

		for (unsigned int i=0;i<count;i++)
		{
			int id = int(dblB[i]);
			if ((id > -1) && (id<numA))
			{
				getVecFromArray(vecA,id,vec);
				setVecToArray(result,i,vec);
			}
			else
			{
                getVecFromArray(vecC,idC,vec);
				setVecToArray(result,i,vec);
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
   Function: mVecSet
   Set elements in a vector array, this function will not grow the vector array, but error when you try to set an invalid array element!

   Parameters:
		vectorArrayA - base vector array
		ids	- int array of ids defining where in A to insert B
		vectorArrayB - vector array of elements to be inserted into A, must be size 1 or same size than id array
   Returns:

      The vector arrayA with elements inserted of of as a float array of its elements

*/
#define mel mVecSet(float $vectorArrayA[], int $id[], float $vectorArrayB[]);
#undef mel

CREATOR(mVecSet)
MStatus mVecSet::doIt( const MArgList& args )
{
	unsigned int count = 1;

	MStatus stat = argCountCheck(args,3); 
	ERROR_FAIL(stat);    
	
    MDoubleArray vecA, dblB, vecC;
    // just extract the elements
    stat = getDoubleArrayArg(args,0,vecA);
    ERROR_FAIL(stat);

    stat = getDoubleArrayArg(args,1,dblB);
    ERROR_FAIL(stat);

    stat = getDoubleArrayArg(args,2,vecC);
    ERROR_FAIL(stat);

    // check if the arguments are valid vector arrays
    unsigned int numA, numB, numC;
    stat = vecIsValid(vecA,numA);
    ERROR_ARG(stat,1);

    numB = dblB.length();

    stat = vecIsValid(vecC,numC);
    ERROR_ARG(stat,3);

    // check the counts
    unsigned int incB, incC;
    stat = twoArgCountsValid(numB,numC,incB,incC,count);
    ERROR_FAIL(stat);


    // build a result array
    MDoubleArray result = vecA;
    
    int vecCId =0;

    MVector vec;

    for (unsigned int i=0;i<count;i++)
    {
       int id = int(dblB[i]);
       if ((id > -1) && (id<numA))
       {
           getVecFromArray(vecC,vecCId,vec);
           setVecToArray(result,id,vec);
       }
       else
       {
           // we've got an invalid index
           MString err = "id array has an invalid index '";
           err = err + id +"' at position '"+i+"'!";
           USER_ERROR_CHECK(MS::kFailure,err)
       }
       
       vecCId += incC;
   }

   setResult(result);

   return MS::kSuccess;    
}



/*
   Function: mVecGetX

   Get vector component X within a vector array.

   Parameters:

		$vecArrayA - the vector array

   Returns:

  the elements at $vec.x as a float array

*/
#define mel mVecGetX(float $vecArrayA[]);
#undef mel

CREATOR(mVecGetX)
MStatus mVecGetX::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgVec(args, dblA, count);
	ERROR_FAIL(stat);

	// do the job
	MDoubleArray result = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
		result[i] = dblA[i*3];

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecGetY

   Get vector component Y within a vector array.

   Parameters:

		$vecArrayA - the vector array

   Returns:

  the elements at $vec.y as a float array

*/
#define mel mVecGetY(float $vecArrayA[]);
#undef mel

CREATOR(mVecGetY)
MStatus mVecGetY::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgVec(args, dblA, count);
	ERROR_FAIL(stat);
    
	// do the job
	MDoubleArray dblE = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
		dblE[i] = dblA[i*3+1];

	setResult(dblE);
	return MS::kSuccess;
}

/*
   Function: mVecGetZ

   Get vector component Z within a vector array.

   Parameters:

		$vecArrayA - the vector array

   Returns:

  the elements at $vec.z as a float array

*/
#define mel mVecGetZ(float $vecArrayA[]);
#undef mel

CREATOR(mVecGetZ)
MStatus mVecGetZ::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgVec(args, dblA, count);
	ERROR_FAIL(stat);

	// do the job
	MDoubleArray dblE = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
		dblE[i] = dblA[i*3+2];

	setResult(dblE);
	return MS::kSuccess;
}

/*
   Function: mVecGetComponent

   Get vector component at specified poistion within a vector array.

   Parameters:

		$vecArrayA - the vector array
		$component - the index [0-2] as an int array

   Returns:

  the element at $vec[$component] as a float array

*/
#define mel mVecGetComponent(float $vecArrayA[], int $component[]);
#undef mel

CREATOR(mVecGetComponent)
MStatus mVecGetComponent::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA  = iterB = 0;

	MVector vecA;
	MDoubleArray dblE = MDoubleArray(count,0);
	unsigned int c;
    
	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);

		// check element is valid
		c = int(dblB[iterB]);
		if (!validVecIndex(c))
		{
			MString err="vector element id error at index ";
			err+=i+"!";
			ERROR_FAIL(MS::kFailure);
		}
		dblE[i] = vecA[c];

		iterA += incA;
		iterB += incB;
	}

	setResult(dblE);
	return MS::kSuccess;
}
/*
   Function: mVecSetX

   Set vector component X 

   Parameters:

		$vecArrayA - the vector array
		$value - the double array to be set into the component

   Returns:

  the vector array with the inserted as a float array

*/
#define mel mVecSetX(float $vecArrayA[], float $value[]);
#undef mel

CREATOR(mVecSetX)
MStatus mVecSetX::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA  = iterB =0;
    MVector vecA;

	MDoubleArray result = createEmptyVecArray(count);
        

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		vecA.x = dblB[iterB];
   		setVecToArray(result,i,vecA);

		iterA += incA;
		iterB += incB;
	}

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecSetY

   Set vector component X 

   Parameters:

		$vecArrayA - the vector array
		$value - the double array to be set into the component

   Returns:

  the vector array with the inserted as a float array

*/
#define mel mVecSetY(float $vecArrayA[], float $value[]);
#undef mel

CREATOR(mVecSetY)
MStatus mVecSetY::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA  = iterB =0;
    MVector vecA;

	MDoubleArray result = createEmptyVecArray(count);
        
	unsigned int c;
	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		vecA.y = dblB[iterB];
   		setVecToArray(result,i,vecA);

		iterA += incA;
		iterB += incB;
	}


	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecSetZ

   Set vector component X 

   Parameters:

		$vecArrayA - the vector array
		$value - the double array to be set into the component

   Returns:

  the vector array with the inserted as a float array

*/
#define mel mVecSetZ(float $vecArrayA[], float $value[]);
#undef mel

CREATOR(mVecSetZ)
MStatus mVecSetZ::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA  = iterB =0;
    MVector vecA;

	MDoubleArray result = createEmptyVecArray(count);
        
	unsigned int c;
	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		vecA.z = dblB[iterB];
   		setVecToArray(result,i,vecA);

		iterA += incA;
		iterB += incB;
	}


	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecSetComponent

   Set vector component at specified poistion within a vector array.

   Parameters:

		$vecArrayA - the vector array
		$component - the index [0-2] as an int array
		$value - the double array to be set into the component

   Returns:

  the vector array with the inserted as a float array

*/
#define mel mVecSetComponent(float $vecArrayA[], int $component[], float $value[]);
#undef mel

CREATOR(mVecSetComponent)
MStatus mVecSetComponent::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB,dblC;
    unsigned int incA, incB, incC,count;
	MStatus stat = getArgVecDblDbl(args, dblA, dblB, dblC,incA, incB,incC, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB, iterC;
	iterA  = iterB = iterC =0;

	MVector vecA;
    MDoubleArray result = createEmptyVecArray(count);

	unsigned int c;
	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);

		// check component is valid
		c = int(dblB[iterB]);
		if (!validVecIndex(c))
		{
			MString err="vector component id error at index ";
			err+=i+", not in valid range [0,1,2]!";
			ERROR_FAIL(MS::kFailure);
		}
        
        vecA[c] = dblC[iterC];
        
		setVecToArray(result,i,vecA);

		iterA += incA;
		iterB += incB;
		iterC += incC;
	}

	setResult(result);
	return MS::kSuccess;
}

} // end namespace
