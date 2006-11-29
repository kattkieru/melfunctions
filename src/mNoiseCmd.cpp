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
#include "../include/Noise.h"

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

//************************************************************************************************//
/*
   Function: mDbl1dNoise

   Create Improved Perlin noise for single values in an array

   Parameters:

		$dblArrayA - the double array

   Returns:

		noise values as a float[]

*/
#define mel mDbl1dNoise(float[] $dblArrayA);
#undef mel

CREATOR(mDbl1dNoise)
MStatus mDbl1dNoise::doIt( const MArgList& args )
{
	// get the arguments
    MDoubleArray dblA;
    unsigned int count;
	MStatus stat = getArgDbl(args, dblA, count);
	ERROR_FAIL(stat);
    
	// do the actual job
	Noise noiseGen;
    
	for (unsigned int i=0;i<dblA.length();i++)
	{
        dblA[i] = noiseGen.improvedPerlin1dU(float(dblA[i]));
	}

	setResult(dblA);
	return MS::kSuccess;
}

//************************************************************************************************//
/*
   Function: mDbl2dNoise

   Create Improved Perlin noise in 2 dimensions

   Parameters:

		either provide
		$dblArrayA - the double array with sample values for dimension 1
        $dblArrayB - the double array with sample values for dimension 2
        
        or
        
        $uvArray - single uv array with the sample values
        

   Returns:

		noise values as a float[]

*/
#define mel mDbl2dNoise(float[] $dblArrayA),float[] $dblArrayB);
#undef mel

CREATOR(mDbl2dNoise)
MStatus mDbl2dNoise::doIt( const MArgList& args )
{
	if (args.length() == 1)
	{
		// get the arguments
    	MDoubleArray dblA;
	    unsigned int count;
		MStatus stat = getArgUV(args, dblA, count);
		ERROR_FAIL(stat);
    
		// do the job
		MDoubleArray result = MDoubleArray(count);		

        Noise noiseGen;
        for(int i=0;i<count;i++)
	    	result[i] = noiseGen.improvedPerlin2dU(float(dblA[i*ELEMENTS_UV]), float(dblA[i*ELEMENTS_UV+1]));
            
        setResult(result);
        
	}
	else if (args.length() == 2)
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
		Noise noiseGen;

		for (unsigned int i=0;i<count;i++)
		{
        	result[i] = noiseGen.improvedPerlin2dU(float(dblA[iterA]), float(dblB[iterB]));

			iterA += incA;
			iterB += incB;
		}
		
        setResult(result);
	}
	else
	{
		USER_ERROR_CHECK(MS::kFailure,("mDbl2dNoise: wrong number of arguments, should be 1 uvArray or 2 dblArrays!"));
	}
    
	return MS::kSuccess;

}

//************************************************************************************************//
/*
   Function: mDbl3dNoise

   Create Improved Perlin noise in 3 dimensions

   Parameters:

		either provide
		$dblArrayA - the double array with sample values for dimension 1
        $dblArrayB - the double array with sample values for dimension 2
        $dblArrayC - the double array with sample values for dimension 3
        
        or
        
        $vecArray - single vector array with the sample values
        

   Returns:

		noise values as a float[]

*/
#define mel mDbl3dNoise(float[] $dblArrayA),float[] $dblArrayB,float[] $dblArrayC);
#undef mel

CREATOR(mDbl3dNoise)
MStatus mDbl3dNoise::doIt( const MArgList& args )
{
	if (args.length() == 1)
	{
    	// vector array
		// get the arguments
    	MDoubleArray dblA;
	    unsigned int count;
		MStatus stat = getArgVec(args, dblA, count);
		ERROR_FAIL(stat);
    
		// do the job
		MDoubleArray result = MDoubleArray(count);		

        Noise noiseGen;
        for(int i=0;i<count;i++)
        {
        	int id = ELEMENTS_VEC *i;
	    	result[i] = noiseGen.improvedPerlin3dU(float(dblA[id]), float(dblA[id+1]),float(dblA[id+2]));
        }
            
        setResult(result);
        
	}
	else if (args.length() == 3)
	{
		// get the arguments
	    MDoubleArray dblA, dblB, dblC;
    	unsigned int incA, incB, incC, count;
		MStatus stat = getArgDblDblDbl(args, dblA, dblB, dblC, incA, incB, incC, count);
		ERROR_FAIL(stat);
	
		// do the actual job
		unsigned int iterA, iterB, iterC;
		iterA = iterB = iterC = 0;

		MDoubleArray result(count);
		Noise noiseGen;

		for (unsigned int i=0;i<count;i++)
		{
        	result[i] = noiseGen.improvedPerlin3dU(float(dblA[iterA]), float(dblB[iterB]),  float(dblC[iterC]));

			iterA += incA;
			iterB += incB;
			iterC += incC;            
		}
		
        setResult(result);
	}
	else
	{
		USER_ERROR_CHECK(MS::kFailure,("mDbl3dNoise: wrong number of arguments, should be 1 vecArray or 3 dblArrays!"));
	}
    
	return MS::kSuccess;

}

//************************************************************************************************//
/*
   Function: mDbl4dNoise

   Create Improved Perlin noise in 4 dimensions

   Parameters:

		either provide
        
		$dblArrayA - the double array with sample values for dimension 1
        $dblArrayB - the double array with sample values for dimension 2
        $dblArrayC - the double array with sample values for dimension 3
        $dblArrayT - the double array with sample values for dimension 4
        
        or
        
        $vecArray - single vector array with the sample values
        $dblArrayT - the double array with sample values for dimension 4        

   Returns:

		noise values as a float[]

*/
#define mel mDbl4dNoise(float[] $dblArrayA),float[] $dblArrayB,float[] $dblArrayC,float[] $dblArrayT);
#undef mel

CREATOR(mDbl4dNoise)
MStatus mDbl4dNoise::doIt( const MArgList& args )
{
	if (args.length() == 2)
	{
    	// vector array and single value
		// get the arguments
    	MDoubleArray dblA,dblD;
	    unsigned int iterA, iterD, incA,incD,count;
        iterA=iterD=0;
		MStatus stat = getArgVecDbl(args, dblA,dblD,incA,incD, count);
		ERROR_FAIL(stat);
    
		// do the job
		MDoubleArray result = MDoubleArray(count);		

        Noise noiseGen;
        for(int i=0;i<count;i++)
        {
        	int id = ELEMENTS_VEC *iterA;
	    	result[i] = noiseGen.improvedPerlin4dU(float(dblA[id]), float(dblA[id+1]),float(dblA[id+2]),float(dblD[iterD]));
            
            iterD += incD;
            iterA += incA;
        }
            
        setResult(result);
        
	}
	else if (args.length() == 4)
	{
		// get the arguments
	    MDoubleArray dblA, dblB, dblC, dblD;
    	unsigned int incA, incB, incC, incD, count;
		MStatus stat = getArgDblDblDblDbl(args, dblA, dblB, dblC, dblD, incA, incB, incC, incD, count);
		ERROR_FAIL(stat);
	
		// do the actual job
		unsigned int iterA, iterB, iterC, iterD;
		iterA = iterB = iterC = iterD = 0;

		MDoubleArray result(count);
		Noise noiseGen;

		for (unsigned int i=0;i<count;i++)
		{
        	result[i] = noiseGen.improvedPerlin4dU(float(dblA[iterA]), float(dblB[iterB]),  float(dblC[iterC]),  float(dblD[iterD]));

			iterA += incA;
			iterB += incB;
			iterC += incC;            
			iterD += incD;                        
		}
		
        setResult(result);
	}
	else
	{
		USER_ERROR_CHECK(MS::kFailure,("mDbl4dNoise: wrong number of arguments, should be 1 vecArray and 1 dblArray or 4 dblArrays!"));
	}
    
	return MS::kSuccess;

}

//************************************************************************************************//
/*
   Function: mVec3dNoise

   Create Improved Perlin noise vector 

   Parameters:

		either provide
		$dblArrayA - the double array with sample values for dimension 1
        $dblArrayB - the double array with sample values for dimension 2
        $dblArrayC - the double array with sample values for dimension 3
        
        or
        
        $vecArray - single vector array with the sample values
        

   Returns:

		noise vector as a float[]

*/
#define mel mVec3dNoise(float[] $dblArrayA),float[] $dblArrayB,float[] $dblArrayC);
#undef mel

CREATOR(mVec3dNoise)
MStatus mVec3dNoise::doIt( const MArgList& args )
{
	if (args.length() == 1)
	{
    	// vector array
		// get the arguments
    	MDoubleArray dblA;
	    unsigned int count;
		MStatus stat = getArgVec(args, dblA, count);
		ERROR_FAIL(stat);
    
		// do the job
		MDoubleArray result = MDoubleArray(count*ELEMENTS_VEC);		

        Noise noiseGen;
        float v[3];
        
        for(int i=0;i<count;i++)
        {
        	int id = ELEMENTS_VEC *i;
            noiseGen.noiseVector(float (dblA[id]), float(dblA[id+1]),float(dblA[id+2]), NOISE_IMPROVED_PERLIN, v);
			result[id] = v[0];
			result[id+1] = v[1];
			result[id+2] = v[2];                        
        }
            
        setResult(result);
        
	}
	else if (args.length() == 3)
	{
		// get the arguments
	    MDoubleArray dblA, dblB, dblC;
    	unsigned int incA, incB, incC, count;
		MStatus stat = getArgDblDblDbl(args, dblA, dblB, dblC, incA, incB, incC, count);
		ERROR_FAIL(stat);
	
		// do the actual job
		unsigned int iterA, iterB, iterC;
		iterA = iterB = iterC = 0;

		MDoubleArray result = MDoubleArray(count*ELEMENTS_VEC);	
		Noise noiseGen;
		float v[3];
         
		for (unsigned int i=0;i<count;i++)
		{
        	noiseGen.noiseVector(float(dblA[iterA]), float(dblB[iterB]),  float(dblC[iterC]),NOISE_IMPROVED_PERLIN, v);
			
            int id = ELEMENTS_VEC *i;
            
            result[id] = v[0];
			result[id+1] = v[1];
			result[id+2] = v[2];
            
			iterA += incA;
			iterB += incB;
			iterC += incC;            
		}
		
        setResult(result);
	}
	else
	{
		USER_ERROR_CHECK(MS::kFailure,("mVec3dNoise: wrong number of arguments, should be 1 vecArray or 3 dblArrays!"));
	}
    
	return MS::kSuccess;

}

}// namespace
