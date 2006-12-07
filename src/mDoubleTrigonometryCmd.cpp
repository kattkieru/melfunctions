// Title: Double Array Trigonometry Commands
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
#include "../include/mDoubleTrigonometryCmd.h"

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



}// namespace
