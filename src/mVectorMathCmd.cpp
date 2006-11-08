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


} // end namespace
