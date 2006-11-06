/*

//DECLARE_COMMAND(mVecRotateByQuat)

*/

// Title: Vector Array Commands
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
#include "../include/mVectorCmd.h"

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
   Function: mVecAdd

   Add two vector arrays together elementwise

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array

   Returns:

      $vecArrayA + $vecArrayB, the sum of the two vector arrays elementswise as a float[]

*/
#define mel mVecAdd(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecAdd)
MStatus mVecAdd::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecVec(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
	
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecB,vecC;
	MDoubleArray dblC = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		vecC = vecA+vecB;
		setVecToArray(dblC,i,vecC);

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}



/*
   Function: mVecSub

   Subtract two vector arrays elementwise

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array

   Returns:

      $vecArrayA - $vecArrayB, the difference of the two vector arrays elementswise as a float[]

*/
#define mel mVecSub(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecSub)
MStatus mVecSub::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecVec(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
    
    
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecB,vecC;
	MDoubleArray dblC = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		vecC = vecA-vecB;
		setVecToArray(dblC,i,vecC);

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}


/*
   Function: mVecDot

   The dot product of two vector arrays elementwise

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array

   Returns:

      $vecArrayA * $vecArrayB, the dot product of the two vector arrays elementswise as a float[]

*/
#define mel mVecDot(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecDot)
MStatus mVecDot::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecVec(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecB;
	MDoubleArray dblC = createEmptyDblArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		dblC[i] = vecA * vecB;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mVecCross

   Cross product of two vector arrays elementwise

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array

   Returns:

      $vecArrayA ^ $vecArrayB, the cross product of the two vector arrays elementswise as a float[]

*/
#define mel mVecCross(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecCross)
MStatus mVecCross::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecVec(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecB,vecC;
	MDoubleArray dblC = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		vecC = vecA^vecB;
		setVecToArray(dblC,i,vecC);

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mVecMult

   Multiply componentwise two vector arrays together

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array

   Returns:

      $vecArrayA.x * $vecArrayB.x, $vecArrayA.y * $vecArrayB.y, $vecArrayA.z * $vecArrayB.z, the component produkt of the two vector arrays as a float[]

*/
#define mel mVecMult(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecMult)
MStatus mVecMult::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecVec(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
	
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecB,vecC;
	MDoubleArray dblC = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		vecC.x = vecA.x * vecB.x;
        vecC.y = vecA.y * vecB.y;
        vecC.z = vecA.z * vecB.z;        
		setVecToArray(dblC,i,vecC);

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}


/*
   Function: mVecDblMult

   Multiplication of vector and double

   Parameters:

		$vecArrayA - the vector array
		$dblArrayB - the double array

   Returns:

      $vecArrayA * $dblArrayB, the product of the two arrays as a float[]

*/
#define mel mVecDblMult(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecDblMult)
MStatus mVecDblMult::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecC;
	MDoubleArray result = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		vecC = vecA*dblB[iterB];
		setVecToArray(result,i,vecC);

		iterA += incA;
		iterB += incB;
	}

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecDblDiv

   Division of vector and double

   Parameters:

		$vecArrayA - the vector array
		$dblArrayB - the double array

   Returns:

      $vecArrayA / $dblArrayB, the product of the two arrays as a float[]

*/
#define mel mVecDblDiv(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecDblDiv)
MStatus mVecDblDiv::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecC;
	MDoubleArray result = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
        if (dblB[iterB] == 0.0)
        {
            MString e = ("mVecDblDiv: division by 0 at element num ");
            USER_ERROR_CHECK(MS::kFailure,(e+iterB));
        }  
        else 
	    	vecC = vecA/dblB[iterB];
		setVecToArray(result,i,vecC);

		iterA += incA;
		iterB += incB;
	}

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecNegate

   The negate vector of an array

   Parameters:

		$vecArrayA - the vector array

   Returns:

      -($vecArrayA) , the negated vectors in an array as a float[]

*/
#define mel mVecNegate(float[] $vecArrayA);
#undef mel

CREATOR(mVecNegate)
MStatus mVecNegate::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgVec(args, dblA, count);
	ERROR_FAIL(stat);
    
    
	// do the actual job
	MVector vecA;
	MDoubleArray result = createEmptyVecArray(count);

	for (unsigned int i=0;i<result.length();i++)
	{
        result[i] = -dblA[i];
	}

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecMatMult

   Multiplication of vector and matrix, the way to transform a vector between differnt spaces

   Parameters:

		$vecArrayA - the vector array
		$matArrayB - the matrix array

   Returns:

      $vecArrayA * $matArrayB, the product of the two arrays as a float[]

*/
#define mel mVecMatMult(float[] $vecArrayA, float[] $matArrayB);
#undef mel

CREATOR(mVecMatMult)
MStatus mVecMatMult::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecMat(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecC;
    MMatrix matB;
	MDoubleArray result = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getMatFromArray(dblB,iterB,matB);        

    	vecC = MPoint(vecA) * matB;
		setVecToArray(result,i,vecC);

		iterA += incA;
		iterB += incB;
	}

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecNormal

   The normalized vector of an array

   Parameters:

		$vecArrayA - the vector array

   Returns:

      norm($vecArrayA) , the normalized vectors in an array as a float[]

*/
#define mel mVecNormal(float[] $vecArrayA);
#undef mel

CREATOR(mVecNormal)
MStatus mVecNormal::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgVec(args, dblA, count);
	ERROR_FAIL(stat);
    
    
	// do the actual job
	MVector vecA;
	MDoubleArray result = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,i,vecA);
		setVecToArray(result,i,vecA.normal());
	}

	setResult(result);
	return MS::kSuccess;
}


/*
   Function: mVecSetLength

   Set length of vector

   Parameters:

		$vecArrayA - the vector array
		$dblArrayB - the double array

   Returns:

      $vecArrayA * $dblArrayB, the product of the two arrays as a float[]

*/
#define mel mVecSetLength(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecSetLength)
MStatus mVecSetLength::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecC;
	MDoubleArray result = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		vecC = vecA.normal() * dblB[iterB];
		setVecToArray(result,i,vecC);

		iterA += incA;
		iterB += incB;
	}

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecDistance

   Distance between two vector position arrays elementwise

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array

   Returns:

      the distance between the two vector arrays elementswise as a float[]

*/
#define mel mVecDistance(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecDistance)
MStatus mVecDistance::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecVec(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
    
    
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecB,vecC;
	MDoubleArray dblC = createEmptyDblArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		vecC = vecA-vecB;
		dblC[i] = vecC.length();

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mVecAngle

   Angle between two vector arrays elementwise

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array

   Returns:

      the angle between the two vector arrays elementswise in rad as a float[]

*/
#define mel mVecAngle(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecAngle)
MStatus mVecAngle::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecVec(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
    
    
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecB;
	MDoubleArray dblC = createEmptyDblArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
        
		dblC[i] = vecA.angle(vecB);

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}


/*
   Function: mVecRotateByEuler

   Rotate vector by euler angles, euler angles are assumed to be in xyz rotation order

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array of euler angles

   Returns:

      the first vector rotate by the euler angles a float[]

*/
#define mel mVecRotateByEuler(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecRotateByEuler)
MStatus mVecRotateByEuler::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecVec(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
    
    
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecB, vecC;
	MDoubleArray result = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
        
		vecC = vecA.rotateBy(MEulerRotation(vecB));
		setVecToArray(result,i,vecC);

		iterA += incA;
		iterB += incB;
	}

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecRotateByAxisAngle

   Rotate vector by defining an axis to rotate about and an angle (in rad)

   Parameters:

		$vecArrayA - the first vector 
		$vecArrayB - the axis
		$dblArrayC - the angle

   Returns:

      the first vector rotated about the axis by the angle as a float[]

*/
#define mel mVecRotateByAxisAngle(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecRotateByAxisAngle)
MStatus mVecRotateByAxisAngle::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB, dblC;
    unsigned int incA, incB, incC, count;
	MStatus stat = getArgVecVecDbl(args, dblA, dblB,dblC, incA, incB, incC,count);
	ERROR_FAIL(stat);
    
    
	// do the actual job
	unsigned int iterA, iterB, iterC;
	iterA = iterB = iterC = 0;

	MVector vecA,vecB, vecC;
	MDoubleArray result = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);

        MQuaternion q(dblC[iterC],vecB);
        vecC =  vecA.rotateBy(q);
		setVecToArray(result,i,vecC);

		iterA += incA;
		iterB += incB;
		iterC += incC;        
	}

	setResult(result);
	return MS::kSuccess;
}
/*
   Function: mVecIsEqual

   Check if two mVectors are exactly equal

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array

   Returns:

      $vecArrayA == $vecArrayB, the result of the elementwise comparison as a float[] of (0,1)

*/
#define mel mVecIsEqual(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecIsEqual)
MStatus mVecIsEqual::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecVec(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecB;
	MDoubleArray dblC = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		
		if (vecA==vecB)
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

/*
   Function: mVecIsNotEqual

   Check if two mVectors are not  equal

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array

   Returns:

      $vecArrayA != $vecArrayB, the result of the elementwise comparison as a float[] of (0,1)

*/
#define mel mVecIsNotEqual(float[] $vecArrayA, float[] $vecArrayB);
#undef mel

CREATOR(mVecIsNotEqual)
MStatus mVecIsNotEqual::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgVecVec(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MVector vecA,vecB;
	MDoubleArray dblC = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		
		if (vecA!=vecB)
			dblC[i] = 1;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}
/*
   Function: mVecIsEquivalent

   Check if elements of two vector arrays are equivalent within a tolerance

   Parameters:

		$vecArrayA - the first matrix array
		$vecArrayB - the second matrix array
		$toleranceArray - the maximum amount of error allowed

   Returns:

      the result of the comparison as a float array of (0,1)

*/

#define mel mVecIsEquivalent(float[] $vecArrayA, float[] $vecArrayB), float[] $toleranceArray);
#undef mel

CREATOR(mVecIsEquivalent)
MStatus mVecIsEquivalent::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB, dblC;
    unsigned int incA, incB, incC, count;
	MStatus stat = getArgVecVecDbl(args, dblA, dblB, dblC, incA, incB, incC, count);
	ERROR_FAIL(stat);


	// do the actual job
	unsigned int iterA, iterB,iterC;
	iterA = iterB = iterC = 0;

	MVector vecA,vecB;
	MDoubleArray result = MDoubleArray(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		
		result[i] = vecA.isEquivalent(vecB,dblC[iterC]);

		iterA += incA;
		iterB += incB;
		iterC += incC;
	}

	setResult(result);
	return MS::kSuccess;
}




/*
   Function: mVecLength

   Get the length of the elements of a vector array.

   Parameters:

		$vecArrayA - the vector array

   Returns:

   the lengths of the vectors as a float[]

*/
#define mel mVecLength(float[] $vecArrayA);
#undef mel

CREATOR(mVecLength)
MStatus mVecLength::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgVec(args, dblA, count);
	ERROR_FAIL(stat);


	// do the actual job
	MVector vecA;
	MDoubleArray result = createEmptyDblArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,i,vecA);
        result[i] =	vecA.length();
    }

	setResult(result);
	return MS::kSuccess;
}

/*
   Function: mVecLengthSqr

   Get the sqr length of the elements of a vector array. If you only need this length for 
   comparison reasons it is faster to compare the sqr(v) as this save you to take the sqrt!

   Parameters:

		$vecArrayA - the vector array

   Returns:

   the sqr lengths of the vectors as a float[]

*/
#define mel mVecLengthSqr(float[] $vecArrayA);
#undef mel

CREATOR(mVecLengthSqr)
MStatus mVecLengthSqr::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgVec(args, dblA, count);
	ERROR_FAIL(stat);


	// do the actual job
	MVector vecA;
	MDoubleArray result = createEmptyDblArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,i,vecA);
        result[i] =	vecA.x*vecA.x+vecA.y*vecA.y+vecA.z*vecA.z;
    }

	setResult(result);
	return MS::kSuccess;
}


/*
   Function: mVecLerp

   linear interpolation of two vectors

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array
		$param - the interpolation parameter [0-1] (will be clamped)

   Returns:

      the interpolated vector array as a float array of (0,1)

*/

#define mel mVecLerp(float[] $vecArrayA, float[] $vecArrayB, float[] $param);
#undef mel

CREATOR(mVecLerp)
MStatus mVecLerp::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB, dblC;
    unsigned int incA, incB, incC, count;
	MStatus stat = getArgVecVecDbl(args, dblA, dblB, dblC, incA, incB, incC, count);
	ERROR_FAIL(stat);


	// do the actual job
	unsigned int iterA, iterB,iterC;
	iterA = iterB = iterC = 0;

	MVector vecA,vecB, vecC;
    double param;
	MDoubleArray result = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		
        param = dblC[iterC];
        
        // interpolate
        if (param <= 0.0)
        {
            vecC = vecA;
        }
        else if (param >= 1.0)
        {
            vecC = vecB;
        }
        else
            vecC = (1-param)*vecA + param * vecB;

        
        //
        setVecToArray(result,i,vecC);
        
		iterA += incA;
		iterB += incB;
		iterC += incC;
	}

	setResult(result);
	return MS::kSuccess;
}


/*
   Function: mVecSlerp

   spherical linear interpolation of two vectors

   Parameters:

		$vecArrayA - the first vector array
		$vecArrayB - the second vector array
		$param - the interpolation parameter [0-1] (will be clamped)

   Returns:

      the interpolated vector array as a float array of (0,1)

*/

#define mel mVecSlerp(float[] $vecArrayA, float[] $vecArrayB, float[] $param);
#undef mel

CREATOR(mVecSlerp)
MStatus mVecSlerp::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB, dblC;
    unsigned int incA, incB, incC, count;
	MStatus stat = getArgVecVecDbl(args, dblA, dblB, dblC, incA, incB, incC, count);
	ERROR_FAIL(stat);


	// do the actual job
	unsigned int iterA, iterB,iterC;
	iterA = iterB = iterC = 0;

	MVector vecA,vecB, vecC;
    double param;
	MDoubleArray result = createEmptyVecArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		getVecFromArray(dblA,iterA,vecA);
		getVecFromArray(dblB,iterB,vecB);
		
        param = dblC[iterC];
        
        // interpolate
        if (param <= 0.0)
            vecC = vecA;
        else if (param >= 1.0)
            vecC = vecB;
        else
        {
            // do the rotation
			MQuaternion quat(vecA,vecB,param);
			vecC = vecA.rotateBy(quat);
			
			// do the length
			double lA = vecA.length();
			double lB = vecB.length();
	
			if (lA != lB)
			{
				double length = lA * (1-param) + lB * param;
				vecC.normalize();
				vecC *= length;
			}
			
        }

        
        //
        setVecToArray(result,i,vecC);
        
		iterA += incA;
		iterB += incB;
		iterC += incC;
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
