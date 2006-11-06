// Title: Double Array Commands
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
#include "../include/mDoubleCmd.h"

namespace melfunctions
{

////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
// single function functions
// wrapped in a macro for convinience

#define M_DBL_SINGLE_FUNC(NAME,FUNCTION)\
CREATOR(NAME)\
MStatus NAME::doIt( const MArgList& args )\
{\
    MDoubleArray dblA;\
    unsigned int count;\
	MStatus stat = getArgDbl(args, dblA, count);\
	ERROR_FAIL(stat);\
	for (unsigned int i=0;i<dblA.length();i++)\
        dblA[i] = FUNCTION(dblA[i]);\
	setResult(dblA);\
	return MS::kSuccess;\
}\


#define M_DBL_SINGLE_FUNC_POSITIVE_ONLY(NAME,FUNCTION)\
CREATOR(NAME)\
MStatus NAME::doIt( const MArgList& args )\
{\
    MDoubleArray dblA;\
    unsigned int count;\
	MStatus stat = getArgDbl(args, dblA, count);\
	ERROR_FAIL(stat);\
	for (unsigned int i=0;i<dblA.length();i++)\
        {\
            if (dblA[i]<0.0)\
            {\
                MString e = #NAME; e = e +": negative element '"+dblA[i]+"' at index '"+i+"'. All elements must be >= 0.0!";\
                USER_ERROR_CHECK(MS::kFailure,(e));            \
            }\
            else\
                dblA[i] = FUNCTION(dblA[i]);\
        }\
	setResult(dblA);\
	return MS::kSuccess;\
}\

#define TRUNC(a)\
double(int(a))


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

//************************************************************************************************//
/*
   Function: mDblAdd

   Add two double arrays together elementwise

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      $dblArrayA + $dblArrayB, the sum of the two double arrays elementswise as a float[]

*/
#define mel mDblAdd(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblAdd)
MStatus mDblAdd::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
	
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;


	MDoubleArray dblC(count);

	for (unsigned int i=0;i<count;i++)
	{
		dblC[i] = dblA[iterA] +dblB[iterB];

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}


//************************************************************************************************//
/*
   Function: mDblSub

   Subtract two double arrays elementwise

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      $dblArrayA - $dblArrayB, the difference of the two double arrays elementswise as a float[]

*/
#define mel mDblSub(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblSub)
MStatus mDblSub::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
    
    
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MDoubleArray dblC = createEmptyDblArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		dblC[i] = dblA[iterA] -dblB[iterB];

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblMult

   Multiply componentwise two double arrays together

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      $dblArrayA * $dblArrayB, the  produkt of the two double arrays as a float[]

*/
#define mel mDblMult(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblMult)
MStatus mDblMult::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);
	
	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;

	MDoubleArray dblC = createEmptyDblArray(count);

	for (unsigned int i=0;i<count;i++)
	{
		dblC[i] = dblA[iterA] *dblB[iterB];

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}


//************************************************************************************************//
/*
   Function: mDblDiv

   Division of double and double

   Parameters:

		$dblArrayA - the double array
		$dblArrayB - the double array

   Returns:

      $dblArrayA / $dblArrayB, the product of the two arrays as a float[]

*/
#define mel mDblDiv(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblDiv)
MStatus mDblDiv::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB;
    unsigned int incA, incB, count;
	MStatus stat = getArgDblDbl(args, dblA, dblB, incA, incB, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB;
	iterA = iterB = 0;


	MDoubleArray result(count);

	for (unsigned int i=0;i<count;i++)
	{
        if (dblB[iterB] == 0.0)
        {
            MString e = ("mDblDblDiv: division by 0 at element num ");
            USER_ERROR_CHECK(MS::kFailure,(e+iterB));
        }  
        else 
	    	result[i] = dblA[iterA]/dblB[iterB];


		iterA += incA;
		iterB += incB;
	}

	setResult(result);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblNegate

   Negate doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      -($dblArrayA) , the negated doubles in an array as a float[]

*/
#define mel mDblNegate(float[] $dblArrayA);
#undef mel

CREATOR(mDblNegate)
MStatus mDblNegate::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgDbl(args, dblA, count);
	ERROR_FAIL(stat);
    
    
	// do the actual job

	for (unsigned int i=0;i<dblA.length();i++)
	{
        dblA[i] *= -1;
	}

	setResult(dblA);
	return MS::kSuccess;
}

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

#define mel mDblIsEquivalent(float[] $dblArrayA, float[] $dblArrayB), float[] $toleranceArray);
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
   Function: mDblMin

   return the minimum of the two values

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      the result  a float[]

*/
#define mel mDblMin(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblMin)
MStatus mDblMin::doIt( const MArgList& args )
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
			dblC[i] = dblA[iterA];
        else
	        dblC[i] = dblB[iterB];

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblMax

   return the maximum of the two values

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array

   Returns:

      the result  a float[]

*/
#define mel mDblMax(float[] $dblArrayA, float[] $dblArrayB);
#undef mel

CREATOR(mDblMax)
MStatus mDblMax::doIt( const MArgList& args )
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
			dblC[i] = dblA[iterA];
        else
	        dblC[i] = dblB[iterB];

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDblAbs

   Get absolute value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the absolut doubles as a float[]

*/
#define mel mDblAbs(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblAbs,fabs)

//************************************************************************************************//

/*
   Function: mDblSign

   get the sign of a double, -1 if dbl<0, 0 if dbl=0 and 1 if dbl>0

   Parameters:

		$dblArrayA - the double array

   Returns:

       the result in an array of float[]

*/
#define mel mDblSign(float[] $dblArrayA);
#undef mel

CREATOR(mDblSign)
MStatus mDblSign::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgDbl(args, dblA, count);
	ERROR_FAIL(stat);
    
   
	// do the actual job

	for (unsigned int i=0;i<dblA.length();i++)
	{
        if (dblA[i] > 0.0)
        	dblA[i] = 1.0;
        else if (dblA[i] < 0.0)
        	dblA[i] = -1.0;
	}

	setResult(dblA);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblDegToRad

   Convert degrees to radians

   Parameters:

		$dblArrayA - the double array

   Returns:

       the converted doubles in an array of float[]

*/
#define mel mDblDegToRad(float[] $dblArrayA);
#undef mel

CREATOR(mDblDegToRad)
MStatus mDblDegToRad::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgDbl(args, dblA, count);
	ERROR_FAIL(stat);
    
	const double mult = M_PI / 180.0;
    
	// do the actual job

	for (unsigned int i=0;i<dblA.length();i++)
	{
        dblA[i] *= mult;
	}

	setResult(dblA);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblRadToDeg

   Convert  radians to degrees

   Parameters:

		$dblArrayA - the double array

   Returns:

       the converted doubles in an array of float[]

*/
#define mel mDblRadToDeg(float[] $dblArrayA);
#undef mel

CREATOR(mDblRadToDeg)
MStatus mDblRadToDeg::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgDbl(args, dblA, count);
	ERROR_FAIL(stat);
    
	const double mult =  180.0/M_PI;
    
	// do the actual job

	for (unsigned int i=0;i<dblA.length();i++)
	{
        dblA[i] *= mult;
	}

	setResult(dblA);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblSin

   Get sine value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the sine of doubles as a float[]

*/
#define mel mDblSin(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblSin,sin)

//************************************************************************************************//

/*
   Function: mDblCos

   Get cosine value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the cosine of doubles as a float[]

*/
#define mel mDblCos(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblCos,cos)

//************************************************************************************************//

/*
   Function: mDblTan

   Get tangent value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the tangent of doubles as a float[]

*/
#define mel mDblTan(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblTan,tan)


//************************************************************************************************//

/*
   Function: mDblASin

   Get arc sine value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the arc sine of doubles as a float[]

*/
#define mel mDblASin(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblASin,asin)

//************************************************************************************************//

/*
   Function: mDblACos

   Get arc cosine value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the arc cosine of doubles as a float[]

*/
#define mel mDblACos(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblACos,acos)

//************************************************************************************************//

/*
   Function: mDblATan

   Get arc tangent value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the arc tangent of doubles as a float[]

*/
#define mel mDblATan(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblATan,atan)

//************************************************************************************************//

/*
   Function: mDblHSin

   Get hyperbolic sine value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the hyperbolic sine of doubles as a float[]

*/
#define mel mDblHSin(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblHSin,sinh)

//************************************************************************************************//

/*
   Function: mDblHCos

   Get hyperbolic cosine value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the hyperbolic cosine of doubles as a float[]

*/
#define mel mDblHCos(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblHCos,cosh)

//************************************************************************************************//

/*
   Function: mDblHTan

   Get hyperbolic tangent value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the hyperbolic tangent of doubles as a float[]

*/
#define mel mDblHTan(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblHTan,tanh)

//************************************************************************************************//

/*
   Function: mDblLog

   Get natural logarithm value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the natural logarithm of doubles as a float[]

*/
#define mel mDblLog(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC_POSITIVE_ONLY(mDblLog,log)

//************************************************************************************************//

/*
   Function: mDblLog10

   Get base 10 logarithm value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the base 10 logarithm of doubles as a float[]

*/
#define mel mDblLog10(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC_POSITIVE_ONLY(mDblLog10,log10)

//************************************************************************************************//
/*
   Function: mDblPow

   Raises the first argument to the power of the second argument

   Parameters:

		$dblArrayA - the divident double array
		$dblArrayB - the divisor double array        

   Returns:

		result as a float array of doubles

*/

#define mel mDblPow(float $dblArrayA[], int $dblArrayB[]);
#undef mel

CREATOR(mDblPow)
MStatus mDblPow::doIt( const MArgList& args )
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
	   	dblC[i] = pow(dblA[iterA],dblB[iterB]);
            
        iterA += incA;
        iterB += incB;           
        
	}

	setResult(dblC);
	return MS::kSuccess;
}


//************************************************************************************************//
/*
   Function: mDblMod

   Returns the floating-point remainder of the division of it arguments A and B

   Parameters:

		$dblArrayA - the divident double array
		$dblArrayB - the divisor double array        

   Returns:

		floating point remainder as a float array of doubles

*/

#define mel mDblMod(float $dblArrayA[], int $dblArrayB[]);
#undef mel

CREATOR(mDblMod)
MStatus mDblMod::doIt( const MArgList& args )
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
    	if (dblB[iterB]==0.0)
        {
            MString e = ("mDblMod: division by 0 at element num ");
            USER_ERROR_CHECK(MS::kFailure,(e+iterB));
        }  
        else 
	    	dblC[i] = fmod(dblA[iterA],dblB[iterB]);
            
        iterA += incA;
        iterB += incB;        
	}

	setResult(dblC);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblTrunc

   round value towards zero to the nearest integer

   Parameters:

		$dblArrayA - the double array

   Returns:

       the nearest integers an array of float[]

*/
#define mel mDblTrunc(float[] $dblArrayA);
#undef mel

CREATOR(mDblTrunc)
MStatus mDblTrunc::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgDbl(args, dblA, count);
	ERROR_FAIL(stat);

	// do the actual job
	for (unsigned int i=0;i<dblA.length();i++)
	{
        
        modf(dblA[i],&dblA[i]); //double(int(dblA[i]));
	}

	setResult(dblA);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblFrac

   get fractional part of a dbl

   Parameters:

		$dblArrayA - the double array

   Returns:

       fractional part as an array of float[]

*/
#define mel mDblFrac(float[] $dblArrayA);
#undef mel

CREATOR(mDblFrac)
MStatus mDblFrac::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgDbl(args, dblA, count);
	ERROR_FAIL(stat);
    double h;
	// do the actual job
	for (unsigned int i=0;i<dblA.length();i++)
	{
		dblA[i] = modf(dblA[i],&h);
		//  dblA[i] = dblA[i] - double(int(dblA[i]));
	}

	setResult(dblA);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblFloor

   Get largest integral value not greater than value of doubles in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA ,  largest integral value not greater than value of doubles as a float[]

*/
#define mel mDblFloor(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblFloor,floor)

//************************************************************************************************//

/*
   Function: mDblCeil

   Compute smallest integral value not less than double in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA , the smallest integral values as a float[]

*/
#define mel mDblCeil(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC(mDblCeil,ceil)

//************************************************************************************************//

/*
   Function: mDblSqrt

   Compute square root of double in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA ,the square root values as a float[]

*/
#define mel mDblSqrt(float[] $dblArrayA);
#undef mel

M_DBL_SINGLE_FUNC_POSITIVE_ONLY(mDblSqrt,sqrt)

//************************************************************************************************//

/*
   Function: mDblCbrt

   Compute cubic root of double in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

      $dblArrayA ,the cubic root values as a float[]

*/
//#define mel mDblCbrt(float[] $dblArrayA);
//#undef mel

//M_DBL_SINGLE_FUNC(mDblCbrt,cbrt)

//************************************************************************************************//

/*
   Function: mDblLerp

   linear interpolation of two doubles

   Parameters:

		$dblArrayA - the first double array
		$dblArrayB - the second double array
		$param - the interpolation parameter [0-1] (will be clamped)

   Returns:

      the interpolated double array as a float array

*/

#define mel mDblLerp(float[] $dblArrayA, float[] $dblArrayB, float[] $param);
#undef mel

CREATOR(mDblLerp)
MStatus mDblLerp::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB, dblC;
    unsigned int incA, incB, incC, count;
	MStatus stat = getArgDblDblDbl(args, dblA, dblB, dblC, incA, incB, incC, count);
	ERROR_FAIL(stat);


	// do the actual job
	unsigned int iterA, iterB,iterC;
	iterA = iterB = iterC = 0;

	
    double a,b,param;
	MDoubleArray result(count,0);

	for (unsigned int i=0;i<count;i++)
	{
		a = dblA[iterA];
		b = dblB[iterB];	
        param = dblC[iterC];
        
        // interpolate
        if (param >= 1.0)
            result[i] = b;
        else if (param <= 0.0)
            result[i] = a;
        else 
            result[i] = (1-param)*a + param * b;
        
		iterA += incA;
		iterB += incB;
		iterC += incC;
	}

	setResult(result);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblRound

   Round mDblArray elements to a specific digit

   Parameters:

		$dblArrayA - the first double array
		$intArrayB - intArray which defines to which digit after the dot will be rounded, negative numbers will round to the left of the dot

   Returns:

      the rounded dblArray elements as a float[]

*/
#define mel mDblRound(float $dblArrayA[], int $dblArrayB[]);
#undef mel

CREATOR(mDblRound)
MStatus mDblRound::doIt( const MArgList& args )
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
    	double digit = TRUNC(dblB[iterB]);
        double n = pow(10.0,digit);
        dblC[i] = dblA[iterA] * n;
		double trNum = TRUNC(dblC[i]);
        dblC[i] = (trNum+TRUNC((dblC[i]-trNum)*2))/n;

		iterA += incA;
		iterB += incB;
	}

	setResult(dblC);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblClamp

   clamp value between min and max value

   Parameters:

		$dblArrayA - the value to be clamped as a double array
		$dblArrayMin - the min value double array
		$dblArrayMax - the max value double array        

   Returns:

      the clamped values as a float[]

*/
#define mel mDblClamp(float $dblArrayA[], float $dblArrayMin[], float $dblArrayMax[]);
#undef mel

CREATOR(mDblClamp)
MStatus mDblClamp::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB,dblC;
    unsigned int incA, incB, incC, count;
	MStatus stat = getArgDblDblDbl(args, dblA, dblB, dblC, incA, incB, incC, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB, iterC;
	iterA = iterB = iterC = 0;

	MDoubleArray result(count,0);

	for (unsigned int i=0;i<count;i++)
	{
    	double min = dblB[iterB];
        double max = dblC[iterC];
        
        if (min > max)
        {
        	min = max;
            max = dblB[iterB];
        }
        
    	if (dblA[iterA] < min)
        	result[i] =  min;
        else if (dblA[iterA] > max)
        	result[i] =  max;
        else
	        result[i] = dblA[iterA];

		iterA += incA;
		iterB += incB;
		iterC += incC;        
	}

	setResult(result);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblPulse

   returns 1 if A is between min and max (inclusive) 0 else

   Parameters:

		$dblArrayA - the value to be pulsed 
		$dblArrayMin - the min value double array
		$dblArrayMax - the max value double array        

   Returns:

      the result as a float[]

*/
#define mel mDblPulse(float $dblArrayA[], float $dblArrayMin[], float $dblArrayMax[]);
#undef mel

CREATOR(mDblPulse)
MStatus mDblPulse::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB,dblC;
    unsigned int incA, incB, incC, count;
	MStatus stat = getArgDblDblDbl(args, dblA, dblB, dblC, incA, incB, incC, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB, iterC;
	iterA = iterB = iterC = 0;

	MDoubleArray result(count,1.0);

	for (unsigned int i=0;i<count;i++)
	{
       	double min = dblB[iterB];
        double max = dblC[iterC];
        
        if (min > max)
        {
        	min = max;
            max = dblB[iterB];
        }


    	if ((dblA[iterA] < min) || (dblA[iterA] > max))
        	result[i] =  0.0;

		iterA += incA;
		iterB += incB;
		iterC += incC;        
	}

	setResult(result);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblLinStep

   Returns value between 0 and 1 representing percentage of the distance between MIN and MAX represented by A.

   Parameters:

		$dblArrayA - the parameter value double array
		$dblArrayMin - the min value double array
		$dblArrayMax - the max value double array        

   Returns:

      the result as a float[]

*/
#define mel mDblLinStep(float $dblArrayA[], float $dblArrayMin[], float $dblArrayMax[]);
#undef mel

CREATOR(mDblLinStep)
MStatus mDblLinStep::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB,dblC;
    unsigned int incA, incB, incC, count;
	MStatus stat = getArgDblDblDbl(args, dblA, dblB, dblC, incA, incB, incC, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB, iterC;
	iterA = iterB = iterC = 0;

	MDoubleArray result(count,1.0);

	for (unsigned int i=0;i<count;i++)
	{

       	double min = dblB[iterB];
        double max = dblC[iterC];
        
        if (min > max)
        {
        	min = max;
            max = dblB[iterB];
        }

    
   		if (dblA[iterA] < min)
        	result[i] =  0.0;
        else if (dblA[iterA] < max)
        {
          	double range = fabs(max -min);
            if (range ==0.0)
   	        	result[i] =  0.0;
            else
            {
            	result[i]= (dblA[iterA] - min)/range;
            }
		}
        
		iterA += incA;
		iterB += incB;
		iterC += incC;        	
  }

	setResult(result);
	return MS::kSuccess;
}

//************************************************************************************************//

/*
   Function: mDblFit

   Returns value between newMin and newMax that isrelative to A in the range between oldMin and oldMax 

   Parameters:

		$dblArrayA - the parameter value double array
		$dblArrayOldMin - the old min value double array
		$dblArrayOldMax - the old max value double array        
		$dblArrayNewMin - the new min value double array
		$dblArrayNewMax - the new max value double array        

   Returns:

      the result as a float[]

*/
#define mel mDblFit(float $dblArrayA[], float $dblArrayOldMin[], float $dblArrayOldMax[],float $dblArrayNewMin[], float $dblArrayNewMax[]);
#undef mel

CREATOR(mDblFit)
MStatus mDblFit::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA, dblB,dblC,dblD,dblE;
    unsigned int incA, incB, incC, incD,incE,count;
	MStatus stat = getArgDblDblDblDblDbl(args, dblA, dblB, dblC, dblD, dblE, incA, incB, incC, incD, incE, count);
	ERROR_FAIL(stat);

	// do the actual job
	unsigned int iterA, iterB, iterC, iterD, iterE;
	iterA = iterB = iterC = iterD = iterE = 0;

	MDoubleArray result(count,0.0);

	for (unsigned int i=0;i<count;i++)
	{
		// swap values if necessary
        double oldMin = dblB[iterB];
        double oldMax = dblC[iterC];
        
        if (oldMin > oldMax)
        {
        	oldMin = oldMax;
            oldMax = dblB[iterB];
        }
        
        
        double newMin = dblD[iterD];
        double newMax = dblE[iterE];
        
        if (newMin > newMax)
        {
        	newMin = newMax;
            newMax = dblD[iterD];
        }
        
		
   		if (dblA[iterA] <= oldMin)
        	result[i] =  newMin;
        else if (dblA[iterA] >= oldMax)
        	result[i] = newMax;
        else
	        result[i] = ((dblA[iterA] - oldMin)/(oldMax - oldMin))*(newMax - newMin)+newMin; 
            
		iterA += incA;
		iterB += incB;
		iterC += incC;        	
		iterD += incD;        	
		iterE += incE;        	                
  }

	setResult(result);
	return MS::kSuccess;
}

//************************************************************************************************//
//************************************************************************************************//
//************************************************************************************************//
//************************************************************************************************//


}// namespace
