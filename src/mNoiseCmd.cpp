/* COPYRIGHT --
 *
 * This file is part of melfunctions, a collection of mel commands to for Autodesk Maya.
 * melfunctions is (c) 2006 Carsten Kolve <carsten@kolve.com>
 * and distributed under the terms of the GNU GPL V2.
 * This contribution to melfunctions is (c) Rising Sun Pictures PTY Ltd, www.rsp.com.au 
 * See the ./License-GPL.txt file in the source tree root for more information.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

// Title: Random and Noise Array Commands
//
// About:
// This functionset gives you many commands to generate pseudo random values and sample noise functions.
//
// Important conventions:
// All commands don't change the actual data in place, but create a new output.


#include <maya/MDoubleArray.h>
#include <maya/MArgList.h>
#include <math.h>

#include "../include/mHelperFunctions.h"
#include "../include/mNoiseCmd.h"

namespace melfunctions
{

int mfSeed = 1;

//************************************************************************************************//
/*
   Function: mSeed
   Init the melfunctions random number generator. This is the seed used for array generation and will be used
   whenever you are using one of the random number functions. So if you want a different array, you have
   to reset the seed to a different value.

   Parameters:

		none - init with a seed of 1
		or
		seed - init with this seed int

   Returns:

      nothing

*/
#define mel mSeed(int $seed);
#undef mel

CREATOR(mSeed)
MStatus mSeed::doIt( const MArgList& args )
{
	MStatus stat;
    
	// check how many arguments we have
	if (args.length() == 0)
	{
//		srand(1);
        mfSeed =1;

		return MS::kSuccess;
	}
	else if (args.length() == 1)
	{
		// one args, check if its an int - if yes return int identity arrays
        int s;
		stat = getIntArg(args, 0, s);
		ERROR_FAIL(stat);
        mfSeed = s;

		return MS::kSuccess;
	}
	else
	{
		ERROR_FAIL(MS::kFailure);	
	}
}

//************************************************************************************************//
/*
   Function: mDblRand
   Create a double array of random numbers

   Parameters:

		none - return a single random value between 0 and 1
		or
		count - return count random doubles between 0 and 1
		or
		count - the number of double elements to create between min and max
		min - minimum random value
		max - maximum random value

   Returns:

      The double array filled with random values

*/
#define mel mDblRand(int $count, float $min, float $max);
#undef mel

CREATOR(mDblRand)
MStatus mDblRand::doIt( const MArgList& args )
{
	MStatus stat;
	int count = 1;
    
   	srand(mfSeed);
    
	// check how many arguments we have
	if (args.length() == 0)
	{
		// no args
		// just return a single double
		setResult(MDoubleArray(1,(rand()/((double)RAND_MAX + 1))));
		return MS::kSuccess;
	}
	else if (args.length() == 1)
	{
		// one args, check if its an int - if yes return int identity arrays
		stat = getIntArg(args, 0, count);
		ERROR_FAIL(stat);
        MDoubleArray result(count);
        
        double randDif = ((double)RAND_MAX + 1);
        
        for (int i=0; i< count; i++)
			result[i] = rand()/randDif;
        
        setResult(result);
		return MS::kSuccess;
	}
	else if (args.length() == 3)
	{
		double min, max;
        
		stat = getIntArg(args, 0, count);
		ERROR_FAIL(stat);

		stat = getDoubleArg(args, 1, min);
		ERROR_FAIL(stat);

		stat = getDoubleArg(args, 2, max);
		ERROR_FAIL(stat);

      	MDoubleArray result(count);
        
        if (min == max)
        {
        	result =  MDoubleArray(count,max);
        }
        else
        {
            if (max < min)
        	{
        		//swap
	            double help = max;
    	        max = min;
        	    min = help;
	        }    
        
    	    double randDif = ((double)RAND_MAX + 1) / (max-min);
            
	        for (int i=0; i< count; i++)
				result[i] = (rand()/randDif)+min;
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
   Function: mDblGauss
   Create a double array of random numbers with gaussian distribution (bell curve)

   Parameters:

		none - return a single random gauss number with mean 0 and standard deviation 1
		or
		count - return count random gauss number with mean 0 and standard deviation 1
		or
		count - the number of double elements to create with mean and standard deviation as desribed in the following parameters
		mean - mean value the gauss distribution is centred around
		deviation - standard deviation around the mean

   Returns:

      The double array filled with random values in gaussian distribution

*/
#define mel mDblGauss(int $count, float $mean, float $deviation);
#undef mel

double gauss()
{
    float x;		// Note: to avoid numerical problems with very small
    float y;		// numbers, we make these variables singe-precision
    float length2;	// floats, but later we call the double-precision log()
			// and sqrt() functions instead of logf() and sqrtf().
    do
    {
    	// generate random numbers between -1 and 1
		x = rand()/(((float)RAND_MAX + 1.0)/2.0)-1.0;
		y = rand()/(((float)RAND_MAX + 1.0)/2.0)-1.0;
		length2 = x * x + y * y;
    }
    while (length2 >= 1 || length2 == 0);

    return x * sqrt (-2 * log (length2) / length2);
}


CREATOR(mDblGauss)
MStatus mDblGauss::doIt( const MArgList& args )
{
	MStatus stat;
	int count = 1;
    
    srand(mfSeed);
    
	// check how many arguments we have
	if (args.length() == 0)
	{
		// no args
		// just return a single double
		setResult(MDoubleArray(1,gauss()));
		return MS::kSuccess;
	}
	else if (args.length() == 1)
	{
		// one args, check if its an int - if yes return int identity arrays
		stat = getIntArg(args, 0, count);
		ERROR_FAIL(stat);
        MDoubleArray result(count);
        
        double randDif = ((double)RAND_MAX + 1);
        
        for (int i=0; i< count; i++)
			result[i] = gauss();
        
        setResult(result);
		return MS::kSuccess;
	}
	else if (args.length() == 3)
	{
		double mean, diff;
        
		stat = getIntArg(args, 0, count);
		ERROR_FAIL(stat);

		stat = getDoubleArg(args, 1, mean);
		ERROR_FAIL(stat);

		stat = getDoubleArg(args, 2, diff);
		ERROR_FAIL(stat);

      	MDoubleArray result(count);
                  
        for (int i=0; i< count; i++)
			result[i] = gauss()*diff+mean;
        
        setResult(result);
		return MS::kSuccess;
	}
	else
	{
		ERROR_FAIL(MS::kFailure);	
	}
}



}// namespace
