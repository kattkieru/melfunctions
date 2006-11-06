
#include "../include/mHelperFunctions.h"
#include "../include/mHelperMacros.h"



using namespace std;

// helper functions to access and verify data

namespace melfunctions
{



/*************************************************************/
MString doubleArrayToString(MDoubleArray a)
{
    MString s="{";
    
    int i=0;
    for(i; i<(a.length()-1); i++)
    {
        s = s + a[i] + ",";
    }
    
    s = s + a[i] + "}";
    
    return s;
}

/*************************************************************/
MString pointArrayToString(MPointArray a)
{
    
    // use an stl string 
    string s= "{";
//    s = s +  8.9;
/*    
    int i=0;
    for(i; i<(a.length()-1); i++)
    {
        s = s + a[i].x +"," + a[i].y + "," + a[i].z + ",";
    }
        
    s = s + a[i].x + "," + a[i].y + "," + a[i].z + "}";
    */
    return MString(s.c_str());
}

/*************************************************************/
MString matrixToString(MMatrix m)
{
    MString s="{";
    s = s + m[0][0] + ","
    + m[0][1] + ","
    + m[0][2] + ","
    + m[0][3] + ","
    + m[1][0] + ","
    + m[1][1] + ","
    + m[1][2] + ","
    + m[1][3] + ","
    + m[2][0] + ","
    + m[2][1] + ","
    + m[2][2] + ","
    + m[2][3] + ","
    + m[3][0] + ","
    + m[3][1] + ","
    + m[3][2] + ","
    + m[3][3] + "}";
    return s;
}


/*************************************************************/
MDoubleArray vectorArrayToDoubleArray(const MVectorArray &a)
{
	// TODO: convert vector array to double array, can we us memcpy?
    MDoubleArray result(a.length()*3,0);
    for(int i=0;i<a.length();i++)
    {
    	result[i*3]   = a[i].x;
        result[i*3+1] = a[i].y;
        result[i*3+2] = a[i].z;                        
	}    
    return result;
}

/*************************************************************/

MStatus validMatIndex(const unsigned int row,const unsigned int column)
{
	if ( (row<0)||(row>3))
	{
		MString err="row value out of bounds, must be between [0-3] but is ";
		err=err+row+"!";
		MGlobal::displayError(err);
		return MS::kFailure;
	}
	if ( (column<0)||(column>3))
	{
		MString err="column value out of bounds, must be between [0-3] but is ";
		err=err+column+"!";
		MGlobal::displayError(err);
		return MS::kFailure;
	}
	return MS::kSuccess;
}

MStatus validVecIndex(const unsigned int index)
{
    if ( (index<0)||(index>2))
	{
		MString err="index value out of bounds, must be between [0-2] but is ";
		err+=index+"!";
		MGlobal::displayError(err);
		return MS::kFailure;
	}
    return MS::kSuccess;
}

MStatus validUVIndex(const unsigned int index)
{
    if ( (index<0)||(index>1))
	{
		MString err="index value out of bounds, must be between [0-1] but is ";
		err+=index+"!";
		MGlobal::displayError(err);
		return MS::kFailure;
	}
    return MS::kSuccess;
}

//
// gets / set functions to read and write complex datatypes into simple double arrays
void getVecFromArray(const MDoubleArray & a, const unsigned int &id, MVector &v)
{
	int offset = id * ELEMENTS_VEC;
	v=MVector(a[offset],a[offset+1],a[offset+2]);
}

void getUVFromArray(const MDoubleArray & a, const unsigned int &id, double2 &uv)
{
	int offset = id * ELEMENTS_UV;
	uv[0] = a[offset];
    uv[1] = a[offset+1];
}


void getQuatFromArray(const MDoubleArray & a, const unsigned int &id, MQuaternion &q)
{
	int offset = id * ELEMENTS_QUAT;
	q=MQuaternion(a[offset],a[offset+1],a[offset+2],a[offset+3]);
}
void getMatFromArray(const MDoubleArray  & a, const unsigned int &id, MMatrix &m)
{
	int offset = id * ELEMENTS_MAT;
	m[0][0]=a[offset];
	m[0][1]=a[offset+1]; 
	m[0][2]=a[offset+2]; 
	m[0][3]=a[offset+3]; 
	m[1][0]=a[offset+4];
	m[1][1]=a[offset+5]; 
	m[1][2]=a[offset+6];
	m[1][3]=a[offset+7];
	m[2][0]=a[offset+8]; 
	m[2][1]=a[offset+9];
	m[2][2]=a[offset+10]; 
	m[2][3]=a[offset+11]; 
	m[3][0]=a[offset+12]; 
	m[3][1]=a[offset+13]; 
	m[3][2]=a[offset+14];
	m[3][3]=a[offset+15];
}



void setMatToArray(MDoubleArray  & result,const unsigned int &id,const MMatrix &m)
{
	int offset = id * ELEMENTS_MAT;
	result[offset] = m[0][0];
	result[offset+1] = m[0][1];
	result[offset+2] = m[0][2];
	result[offset+3] = m[0][3];
	result[offset+4] = m[1][0];
	result[offset+5] = m[1][1];
	result[offset+6] = m[1][2];
	result[offset+7] = m[1][3];
	result[offset+8] = m[2][0];
	result[offset+9] = m[2][1];
	result[offset+10] = m[2][2];
	result[offset+11] = m[2][3];
	result[offset+12] = m[3][0];
	result[offset+13] = m[3][1];
	result[offset+14] = m[3][2];
	result[offset+15] = m[3][3];
}

void setVecToArray(MDoubleArray  & result,const unsigned int &id,const MVector &v)
{
	int offset = id * ELEMENTS_VEC;
	result[offset] = v.x;
	result[offset+1] = v.y;
	result[offset+2] = v.z;
}

void setUVToArray(MDoubleArray  & result,const unsigned int &id,const double2 &uv)
{
	int offset = id * ELEMENTS_UV;
	result[offset] = uv[0];
	result[offset+1] = uv[1];
}

void setQuatToArray(MDoubleArray  & result,const unsigned int &id,const MQuaternion &q)
{
	int offset = id * ELEMENTS_QUAT;
	result[offset] = q.x;
	result[offset+1] = q.y;
	result[offset+2] = q.z;
	result[offset+3] = q.w;
}

//
// validator, check if double array could be valid to treat as a more complex array type
// returns 0 if not valid, the numer of elemts of the specific type else
MStatus uvIsValid(const MDoubleArray  & a, unsigned int &size)
{
	size = a.length();
	if ((size % ELEMENTS_UV)==0)
	{
		size /= ELEMENTS_UV;
		return MS::kSuccess;
	}
	else
	{
		MGlobal::displayError("float array is not the right size to be a uv array!");
		return MS::kFailure;
	}
}

MStatus vecIsValid(const MDoubleArray  & a, unsigned int &size)
{
	size = a.length();
	if ((size % ELEMENTS_VEC)==0)
	{
		size /= ELEMENTS_VEC;
		return MS::kSuccess;
	}
	else
	{
		MGlobal::displayError("float array is not the right size to be a vector array!");
		return MS::kFailure;
	}
}
MStatus quatIsValid(const MDoubleArray  & a, unsigned int &size)
{
	size = a.length();
	if ((size % ELEMENTS_QUAT)==0)
	{
		size /= ELEMENTS_QUAT;
		return MS::kSuccess;
	}
	else
	{
		MGlobal::displayError("float array is not the right size to be a quaternion array!");
		return MS::kFailure;
	}
}
MStatus matIsValid(const MDoubleArray  & a, unsigned int &size)
{
	size = a.length();
	if ((size % ELEMENTS_MAT)==0)
	{
		size /= ELEMENTS_MAT;
		return MS::kSuccess;
	}
	else
	{
		MGlobal::displayError("float array is not the right size to be a matrix array!");
		return MS::kFailure;
	}
}



//
// creator, create a dbl array to hold data of specifix type using a template
MDoubleArray createArrayFromMat(const unsigned int count,const MMatrix &m)
{
	MDoubleArray result(count * ELEMENTS_MAT,0.0);
	for (unsigned int i=0;i<count;i++)
		setMatToArray(result,i,m);
	return result;
}

MDoubleArray createArrayFromVec(const unsigned int count,const MVector &v)
{
	MDoubleArray result(count * ELEMENTS_VEC,0.0);
	for (unsigned int i=0;i<count;i++)
		setVecToArray(result,i,v);
	return result;
}

MDoubleArray createArrayFromUV(const unsigned int count,const double2 &uv)
{
	MDoubleArray result(count * ELEMENTS_UV,0.0);
	for (unsigned int i=0;i<count;i++)
		setUVToArray(result,i,uv);
	return result;
}

MDoubleArray createArrayFromQuat(const unsigned int count,const MQuaternion &q)
{
	MDoubleArray result(count * ELEMENTS_QUAT,0.0);
	for (unsigned int i=0;i<count;i++)
		setQuatToArray(result,i,q);
	return result;
}

MDoubleArray createEmptyMatArray(const unsigned int count)
{
	return MDoubleArray(count*ELEMENTS_MAT,0.0);
}

MDoubleArray createEmptyQuatArray(const unsigned int count)
{
	return MDoubleArray(count*ELEMENTS_QUAT,0.0);
}

MDoubleArray createEmptyVecArray(const unsigned int count)
{
	return MDoubleArray(count*ELEMENTS_VEC,0.0);
}

MDoubleArray createEmptyUVArray(const unsigned int count)
{
	return MDoubleArray(count*ELEMENTS_UV,0.0);
}

MDoubleArray createEmptyDblArray(const unsigned int count)
{
	return MDoubleArray(count,0.0);
}

//
// arg list parsing

//
// size check
MStatus argCountCheck( const MArgList& args, unsigned int count)
{
	if (args.length() !=  count)
	{
		MString err("wrong number of arguments, expected ");
		err = err + count + " but found " + args.length() +"!";
		MGlobal::displayError(err);
		return MS::kFailure;
	}
	else
		return MS::kSuccess;
}

//
// get double array from argument float, matrix or vector elemtens will all be converted
// into an array
MStatus getDoubleArrayArg( const MArgList& args, unsigned int argIndex, MDoubleArray &a)
{
	MStatus stat;
	
	// try getting the double array from the specified argIndex
	stat = args.get( argIndex, a );
	if (stat) return stat;
	
	// didn't work? try a single double
	double scal = args.asDouble(argIndex,&stat);
	if (stat)
	{
		// make a double array of a single
		a = MDoubleArray(1,scal);
		return stat;
	}

	// didn't work? try a vector
	MVector vec = args.asVector(argIndex,3,&stat);
	if (stat)
	{
		// make a double array of a vector
		a = MDoubleArray(3,vec.x);
		a[1] = vec.y;
		a[2] = vec.z;
		return stat;
	}

	// didn't work? try a matrix
	MMatrix m; 
	stat = args.get( argIndex, m );
	if (stat)
	{
		a = MDoubleArray(16,0);
		setMatToArray(a,0,m);	
		return stat;
	}
	
	// didn't work? try an int array
	MIntArray ia;
	stat = args.get( argIndex, ia );
	if (stat)
	{
		// convert the int array into a double array
		a = MDoubleArray(ia.length(), double(ia[0]));
		
		for (unsigned int i=1; i < ia.length(); i++)
			a[i] = double(ia[i]);
		
		return stat;
	}

	// nothing worked? give up...
	MString err("can't get or convert argument ");
	err = err + (argIndex+1) + " into a doubleArray!";
	MGlobal::displayError(err);
	return MS::kFailure;
}




// get a single int fomr the arg list
MStatus getIntArg( const MArgList& args, unsigned int argIndex, int &a)
{
	MStatus stat;
	int d = args.asInt(argIndex,&stat);
	if (stat)
	{
		a = d;
		return stat;
	}
	else
	{
		// give up...
		MString err("can't get int argument ");
		err = err + (argIndex+1) + "!";
		MGlobal::displayError(err);
		return MS::kFailure;
	}
}

// get a single double fomr the arg list
MStatus getDoubleArg( const MArgList& args, unsigned int argIndex, double &a)
{
	MStatus stat;
	double d = args.asDouble(argIndex,&stat);
	if (stat)
	{
		a = d;
		return stat;
	}
	else
	{
		// give up...
		MString err("can't get double argument ");
		err = err + (argIndex+1) + "!";
		MGlobal::displayError(err);
		return MS::kFailure;
	}
}


//
//
// max 
unsigned int maximum(const unsigned int &a,const unsigned int &b)
{
	if (a>b) 
		return a;
	else
		return b;
}
//
// check the count validities and get
MStatus twoArgCountsValid(const unsigned int &numA,const unsigned int& numB,
						  unsigned int& incA, unsigned int& incB,
						  unsigned int& count)
{
	if ((numA==1)&&(numB==1))
	{
		incA = incB = 0;
		count = 1;
	} 
	else if (numA==numB)
	{
		incA = incB = 1;
		count = numA;
	} 
	else if (numA==1)
	{
		incA = 0;
		incB = 1;
		count = numB;
	} 
	else if (numB==1)
	{
		incA = 1;
		incB = 0;
		count = numA;
	} 
	else
	{
		MString err( "the two arguments have different sizes, ");
		err = err + "arg1: "+numA+", arg2: "+numB+" elements!"; 
		MGlobal::displayError(err);
		return MS::kFailure;
	}
	
//    cerr <<"\nnumA "<<numA<<" numB "<<numB<<" cnt "<<count;
	return MS::kSuccess;
}

MStatus threeArgCountsValid(const unsigned int &numA,const unsigned int& numB,const unsigned int& numC,
						  unsigned int& incA, unsigned int& incB,unsigned int& incC,
						  unsigned int& count)
{
	// get the maximum
	count = maximum(numA,numB);
	count = maximum(count,numC);

	bool error = 0;
	//
	if (numA==1)
		incA = 0;
	else if (numA==count)
		incA =1;
	else
		error =1;

	if (numB==1)
		incB = 0;
	else if (numB==count)
		incB =1;
	else
		error =1;

	if (numC==1)
		incC = 0;
	else if (numC==count)
		incC =1;
	else
		error =1;

	if (error)
	{
		MString err( "the three arguments have different sizes, ");
		err = err + "arg1: "+numA+", arg2: "+numB+", arg3: "+numC+" elements!"; 
		MGlobal::displayError(err);
		return MS::kFailure;
	}
	
	return MS::kSuccess;
}

MStatus fourArgCountsValid(const unsigned int &numA,const unsigned int& numB,const unsigned int& numC,const unsigned int& numD,
						  unsigned int& incA, unsigned int& incB,unsigned int& incC,unsigned int& incD,
						  unsigned int& count)
{
	// get the maximum
	count = maximum(numA,numB);
	int count2 = maximum(numC,numD);
	count = maximum(count,count2);

	bool error = 0;
	//
	if (numA==1)
		incA = 0;
	else if (numA==count)
		incA =1;
	else
		error =1;

	if (numB==1)
		incB = 0;
	else if (numB==count)
		incB =1;
	else
		error =1;

	if (numC==1)
		incC = 0;
	else if (numC==count)
		incC =1;
	else
		error =1;

	if (numD==1)
		incD = 0;
	else if (numD==count)
		incD =1;
	else
		error =1;

	if (error)
	{
		MString err( "the four arguments have different sizes, ");
		err = err + "arg1: "+numA+", arg2: "+numB+", arg3: "+numC+" arg4: "+numD+" elements!"; 
		MGlobal::displayError(err);
		return MS::kFailure;
	}
	
	return MS::kSuccess;
}

MStatus fiveArgCountsValid(const unsigned int &numA,const unsigned int& numB,const unsigned int& numC,const unsigned int& numD,const unsigned int& numE,
						  unsigned int& incA, unsigned int& incB,unsigned int& incC,unsigned int& incD,unsigned int& incE,
						  unsigned int& count)
{
	// get the maximum
	count = maximum(numA,numB);
	int count2 = maximum(numC,numD);
	count = maximum(count,count2);
    count = maximum(count,numE);

	bool error = 0;
	//
	if (numA==1)
		incA = 0;
	else if (numA==count)
		incA =1;
	else
		error =1;

	if (numB==1)
		incB = 0;
	else if (numB==count)
		incB =1;
	else
		error =1;

	if (numC==1)
		incC = 0;
	else if (numC==count)
		incC =1;
	else
		error =1;

	if (numD==1)
		incD = 0;
	else if (numD==count)
		incD =1;
	else
		error =1;

	if (numE==1)
		incE = 0;
	else if (numE==count)
		incE =1;
	else
		error =1;
                

	if (error)
	{
		MString err( "the five arguments have different sizes, ");
		err = err + "arg1: "+numA+", arg2: "+numB+", arg3: "+numC+" arg4: "+numD+" arg5: "+numE+" elements!"; 
		MGlobal::displayError(err);
		return MS::kFailure;
	}
	
	return MS::kSuccess;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// get elements of specified type from argument list
//
//

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// double
//

MStatus getArgDbl(const MArgList& args, MDoubleArray  & dblA,  unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,1); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,dblA);
	count = dblA.length();
	ERROR_FAIL(stat);

    return stat;
}

MStatus getArgDblDbl(const MArgList& args, MDoubleArray  & dblA,MDoubleArray  & dblB, unsigned int &incA,unsigned int &incB, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,2); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,dblA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	unsigned int numA, numB;
	numA = dblA.length();
	numB = dblB.length();

	// validate the counts and get iterators increasors
	stat = twoArgCountsValid(numA,numB,incA,incB,count);
	ERROR_FAIL(stat);
    
    return MS::kSuccess;

}

MStatus getArgDblDblDbl(const MArgList& args, MDoubleArray  & dblA,MDoubleArray  & dblB,MDoubleArray  & dblC, unsigned int &incA,unsigned int &incB, unsigned int &incC,unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,3); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,dblA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,2,dblC);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	unsigned int numA, numB, numC;
	numA = dblA.length();
	numB = dblB.length();
	numC = dblC.length();

	// validate the counts and get iterators increasors
	stat = threeArgCountsValid(numA,numB,numC,incA,incB,incC,count);
	ERROR_FAIL(stat);

    return MS::kSuccess;
}

MStatus getArgDblDblDblDblDbl(const MArgList& args, 
								MDoubleArray  & dblA,
                                MDoubleArray  & dblB,
                                MDoubleArray  & dblC, 
                                MDoubleArray  & dblD, 
                                MDoubleArray  & dblE, 
                                unsigned int &incA,
                                unsigned int &incB, 
                                unsigned int &incC,
                                unsigned int &incD,
                                unsigned int &incE,                                                                
                                unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,5); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,dblA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,2,dblC);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,3,dblD);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,4,dblE);
	ERROR_FAIL(stat);


	// check if the arguments are valid vector arrays
	unsigned int numA, numB, numC, numD, numE;
	numA = dblA.length();
	numB = dblB.length();
	numC = dblC.length();
	numD = dblD.length();
	numE = dblE.length();    

	// validate the counts and get iterators increasors
	stat = fiveArgCountsValid(numA,numB,numC,numD,numE,incA,incB,incC,incD,incE,count);
	ERROR_FAIL(stat);

    return MS::kSuccess;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// uv
//

MStatus getArgUV(const MArgList& args, MDoubleArray  & uvA,  unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,1); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,uvA);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	stat = uvIsValid(uvA,count);
	ERROR_ARG(stat,1);

    return stat;

}

/********************************************************************************************/
MStatus getArgUVDbl(const MArgList& args, MDoubleArray  & uvA,MDoubleArray  & dblB, 
                        unsigned int &incA,unsigned int &incB, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,2); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,uvA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	unsigned int numA, numB;
	stat = uvIsValid(uvA,numA);
	ERROR_ARG(stat,1);

	numB = dblB.length();

	// validate the counts and get iterators increasors
	stat = twoArgCountsValid(numA,numB,incA,incB,count);
	ERROR_FAIL(stat);
    
    return MS::kSuccess;    
}
/********************************************************************************************/
MStatus getArgUVDblDbl(const MArgList& args, MDoubleArray  & uvA,MDoubleArray  & dblB, MDoubleArray  & dblC,
                        unsigned int &incA,unsigned int &incB,unsigned int &incC, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,3); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,uvA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,2,dblC);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	unsigned int numA, numB, numC;
	stat = uvIsValid(uvA,numA);
	ERROR_ARG(stat,1);

	numB = dblB.length();
	numC = dblC.length();

	// validate the counts and get iterators increasors
	stat = threeArgCountsValid(numA,numB,numC,incA,incB,incC,count);
	ERROR_FAIL(stat);

    return MS::kSuccess;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// vector
//


MStatus getArgVec(const MArgList& args, MDoubleArray  & vecA,  unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,1); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,vecA);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	stat = vecIsValid(vecA,count);
	ERROR_ARG(stat,1);

    return stat;
}

/********************************************************************************************/
MStatus getArgVecVec(const MArgList& args, MDoubleArray  & vecA,MDoubleArray  & vecB, 
                        unsigned int &incA,unsigned int &incB, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,2); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,vecA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,vecB);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	unsigned int numA, numB;
	stat = vecIsValid(vecA,numA);
	ERROR_ARG(stat,1);

	stat = vecIsValid(vecB,numB);
	ERROR_ARG(stat,2);	

	// validate the counts and get iterators increasors
	stat = twoArgCountsValid(numA,numB,incA,incB,count);
	ERROR_FAIL(stat);
    
    return MS::kSuccess;
}

/********************************************************************************************/
MStatus getArgVecDbl(const MArgList& args, MDoubleArray  & vecA,MDoubleArray  & dblB, 
                        unsigned int &incA,unsigned int &incB, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,2); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,vecA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	unsigned int numA, numB;
	stat = vecIsValid(vecA,numA);
	ERROR_ARG(stat,1);

	numB = dblB.length();

	// validate the counts and get iterators increasors
	stat = twoArgCountsValid(numA,numB,incA,incB,count);
	ERROR_FAIL(stat);
    
    return MS::kSuccess;    
}

/********************************************************************************************/
MStatus getArgVecMat(const MArgList& args, MDoubleArray  & vecA,MDoubleArray  & matB, 
                        unsigned int &incA,unsigned int &incB, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,2); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,vecA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,matB);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	unsigned int numA, numB;
	stat = vecIsValid(vecA,numA);
	ERROR_ARG(stat,1);

	stat = matIsValid(matB,numB);
	ERROR_ARG(stat,2);	

	// validate the counts and get iterators increasors
	stat = twoArgCountsValid(numA,numB,incA,incB,count);
	ERROR_FAIL(stat);

    return MS::kSuccess;
}

/********************************************************************************************/
MStatus getArgVecDblDbl(const MArgList& args, MDoubleArray  & vecA,MDoubleArray  & dblB, MDoubleArray  & dblC,
                        unsigned int &incA,unsigned int &incB,unsigned int &incC, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,3); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,vecA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,2,dblC);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	unsigned int numA, numB, numC;
	stat = vecIsValid(vecA,numA);
	ERROR_ARG(stat,1);

	numB = dblB.length();
	numC = dblC.length();

	// validate the counts and get iterators increasors
	stat = threeArgCountsValid(numA,numB,numC,incA,incB,incC,count);
	ERROR_FAIL(stat);

    return MS::kSuccess;
}

/********************************************************************************************/
MStatus getArgVecVecDbl(const MArgList& args, MDoubleArray  & vecA,MDoubleArray  & vecB, MDoubleArray  & dblC,
                        unsigned int &incA,unsigned int &incB,unsigned int &incC, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,3); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,vecA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,vecB);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,2,dblC);
	ERROR_FAIL(stat);

	// check if the arguments are valid vector arrays
	unsigned int numA, numB, numC;
	stat = vecIsValid(vecA,numA);
	ERROR_ARG(stat,1);

	stat = vecIsValid(vecB,numB);
	ERROR_ARG(stat,2);

	numC = dblC.length();

	// validate the counts and get iterators increasors
	stat = threeArgCountsValid(numA,numB,numC,incA,incB,incC,count);
	ERROR_FAIL(stat);

    return MS::kSuccess;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// matrix
//


MStatus getArgMat(const MArgList& args, MDoubleArray  & matA,  unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,1); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,matA);
	ERROR_FAIL(stat);

	// check if the arguments are valid mattor arrays
	stat = matIsValid(matA,count);
	ERROR_ARG(stat,1);

    return stat;
}

/********************************************************************************************/
MStatus getArgMatMat(const MArgList& args, MDoubleArray  & matA,MDoubleArray  & matB, 
                        unsigned int &incA,unsigned int &incB, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,2); 
	USER_ERROR_CHECK(stat,"wrong number of arguments, should be 2!")
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,matA);
	USER_ERROR_CHECK(stat,"error getting dbl array argument 1!")

	stat = getDoubleArrayArg(args,1,matB);
	USER_ERROR_CHECK(stat,"error getting dbl array argument 2!")

	// check if the arguments are valid mattor arrays
	unsigned int numA, numB;
	stat = matIsValid(matA,numA);
	ERROR_ARG(stat,1);

	stat = matIsValid(matB,numB);
	ERROR_ARG(stat,2);	

	// validate the counts and get iterators increasors
	stat = twoArgCountsValid(numA,numB,incA,incB,count);
    
  //  cerr <<"\nc :"<<count;
	USER_ERROR_CHECK(stat,"sizes of arguments are invalid, must be either 1 or size of biggest array in arguments!")
    
    return MS::kSuccess;
}

/********************************************************************************************/
MStatus getArgMatDbl(const MArgList& args, MDoubleArray  & matA,MDoubleArray  & dblB, 
                        unsigned int &incA,unsigned int &incB, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,2); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,matA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);

	// check if the arguments are valid mattor arrays
	unsigned int numA, numB;
	stat = matIsValid(matA,numA);
	ERROR_ARG(stat,1);

	numB = dblB.length();

	// validate the counts and get iterators increasors
	stat = twoArgCountsValid(numA,numB,incA,incB,count);
	ERROR_FAIL(stat);

    return MS::kSuccess;    
}


/********************************************************************************************/
MStatus getArgMatDblDbl(const MArgList& args, MDoubleArray  & matA,MDoubleArray  & dblB, MDoubleArray  & dblC,
                        unsigned int &incA,unsigned int &incB,unsigned int &incC, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,3); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,matA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,2,dblC);
	ERROR_FAIL(stat);

	// check if the arguments are valid mattor arrays
	unsigned int numA, numB, numC;
	stat = matIsValid(matA,numA);
	ERROR_ARG(stat,1);

	numB = dblB.length();
	numC = dblC.length();

	// validate the counts and get iterators increasors
	stat = threeArgCountsValid(numA,numB,numC,incA,incB,incC,count);
	ERROR_FAIL(stat);
    
    return MS::kSuccess;
        
}

/********************************************************************************************/
MStatus getArgMatMatDbl(const MArgList& args, MDoubleArray  & matA,MDoubleArray  & matB, MDoubleArray  & dblC,
                        unsigned int &incA,unsigned int &incB,unsigned int &incC, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,3); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,matA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,matB);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,2,dblC);
	ERROR_FAIL(stat);

	// check if the arguments are valid mattor arrays
	unsigned int numA, numB, numC;
	stat = matIsValid(matA,numA);
	ERROR_ARG(stat,1);

	stat = matIsValid(matB,numB);
	ERROR_ARG(stat,2);

	numC = dblC.length();

	// validate the counts and get iterators increasors
	stat = threeArgCountsValid(numA,numB,numC,incA,incB,incC,count);
	ERROR_FAIL(stat);
    
    return MS::kSuccess;
}


/********************************************************************************************/
MStatus getArgMatDblDblDbl(const MArgList& args, MDoubleArray  & matA,MDoubleArray  & dblB,MDoubleArray  & dblC,MDoubleArray  & dblD, 
                        unsigned int &incA,unsigned int &incB,unsigned int &incC,unsigned int &incD, unsigned int &count)
{
	// check the argument count
	MStatus stat = argCountCheck(args,4); 
	ERROR_FAIL(stat);
	
	// get dbl arrays from the arguments
	stat = getDoubleArrayArg(args,0,matA);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,1,dblB);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,2,dblC);
	ERROR_FAIL(stat);

	stat = getDoubleArrayArg(args,3,dblD);
	ERROR_FAIL(stat);

	// check if the arguments are valid mattor arrays
	unsigned int numA, numB, numC,numD;
	stat = matIsValid(matA,numA);
	ERROR_ARG(stat,1);

	numB = dblB.length();
	numC = dblC.length();
	numD = dblD.length();    

	// validate the counts and get iterators increasors
	stat = fourArgCountsValid(numA,numB,numC,numD,incA,incB,incC,incD,count);
	ERROR_FAIL(stat);

    return MS::kSuccess;
}



}// namespace


