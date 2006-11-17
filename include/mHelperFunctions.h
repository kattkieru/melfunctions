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


#ifndef __mHelperFunctions_h__
#define __mHelperFunctions_h__

#include <maya/MArgList.h>
#include <maya/MVector.h>
#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MQuaternion.h>
#include <maya/MDoubleArray.h>
#include <maya/MVectorArray.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MArgList.h>
#include <maya/MString.h>

#include <math.h>


// helper functions to access and verify data

namespace melfunctions
{

#define ELEMENTS_MAT 16
#define ELEMENTS_VEC 3
#define ELEMENTS_UV 2
#define ELEMENTS_QUAT 4


// definitions
inline double distanceSqr(MVector a, MVector b)
{
	return (pow((a.x-b.x),2)+pow((a.y-b.y),2)+pow((a.z-b.z),2));
}

MString doubleArrayToString(MDoubleArray a);
MString pointArrayToString(MPointArray a);
MString matrixToString(MMatrix m);

MDoubleArray vectorArrayToDoubleArray(const MVectorArray &a);

MStatus validMatIndex(const unsigned int row,const unsigned int column);
MStatus validVecIndex(const unsigned int index);
MStatus validUVIndex(const unsigned int index);

void getVecFromArray(const MDoubleArray & a, const unsigned int &id, MVector &v);
void getUVFromArray(const MDoubleArray & a, const unsigned int &id, double2 &uv);
void getQuatFromArray(const MDoubleArray & a, const unsigned int &id, MQuaternion &q);
void getMatFromArray(const MDoubleArray  & a, const unsigned int &id, MMatrix &m);


void setMatToArray(MDoubleArray  & result,const unsigned int &id,const MMatrix &m);
void setVecToArray(MDoubleArray  & result,const unsigned int &id,const MVector &v);
void setUVToArray(MDoubleArray  & result,const unsigned int &id,const double2 &v);
void setQuatToArray(MDoubleArray  & result,const unsigned int &id,const MQuaternion &q);

// validator, check if double array could be valid to treat as a more complex array type
// returns 0 if not valid, the numer of elemts of the specific type else
MStatus uvIsValid(const MDoubleArray  & a, unsigned int &size);
MStatus vecIsValid(const MDoubleArray  & a, unsigned int &size);
MStatus quatIsValid(const MDoubleArray  & a, unsigned int &size);
MStatus matIsValid(const MDoubleArray  & a, unsigned int &size);

//
// creator, create a dbl array to hold data of specifix type using a template
MDoubleArray createArrayFromMat(const unsigned int count,const MMatrix &m);
MDoubleArray createArrayFromUV(const unsigned int count,const double2 &uv);
MDoubleArray createArrayFromVec(const unsigned int count,const MVector &v);
MDoubleArray createArrayFromQuat(const unsigned int count,const MQuaternion &q);

MDoubleArray createEmptyDblArray(const unsigned int count);
MDoubleArray createEmptyMatArray(const unsigned int count);
MDoubleArray createEmptyQuatArray(const unsigned int count);
MDoubleArray createEmptyVecArray(const unsigned int count);
MDoubleArray createEmptyUVArray(const unsigned int count);

//
// arg list parsing

MStatus argCountCheck( const MArgList& args, unsigned int count);
MStatus getDoubleArrayArg( const MArgList& args, unsigned int argIndex, MDoubleArray &a);
MStatus getIntArg( const MArgList& args, unsigned int argIndex, int &a);
MStatus getDoubleArg( const MArgList& args, unsigned int argIndex, double &a);

unsigned int maximum(const unsigned int &a,const unsigned int &b);

MStatus twoArgCountsValid(const unsigned int &numA,const unsigned int& numB,
						  unsigned int& incA, unsigned int& incB,
						  unsigned int& count);
MStatus threeArgCountsValid(const unsigned int &numA,const unsigned int& numB,const unsigned int& numC,
						  unsigned int& incA, unsigned int& incB,unsigned int& incC,
						  unsigned int& count);
MStatus fourArgCountsValid(const unsigned int &numA,const unsigned int& numB,const unsigned int& numC,const unsigned int& numD,
						  unsigned int& incA, unsigned int& incB,unsigned int& incC,unsigned int& incD,
						  unsigned int& count);

MStatus getArgDbl(const MArgList& args, MDoubleArray  & dblA,  unsigned int &count);
MStatus getArgDblDbl(const MArgList& args, MDoubleArray  & dblA,MDoubleArray  & dblB, unsigned int &incA,unsigned int &incB, unsigned int &count);
MStatus getArgDblDblDbl(const MArgList& args, MDoubleArray  & dblA,MDoubleArray  & dblB,MDoubleArray  & dblC, unsigned int &incA,unsigned int &incB, unsigned int &incC,unsigned int &count);
MStatus getArgDblDblDblDblDbl(const MArgList& args, MDoubleArray  & dblA,MDoubleArray  & dblB,MDoubleArray  & dblC, MDoubleArray  & dblD, MDoubleArray  & dblE, unsigned int &incA,unsigned int &incB, unsigned int &incC,unsigned int &incD,unsigned int &incE,unsigned int &count);

MStatus getArgUV(const MArgList& args, MDoubleArray  & uvA,  unsigned int &count);
MStatus getArgUVDbl(const MArgList& args, MDoubleArray  & uvA,MDoubleArray  & dblB, unsigned int &incA,unsigned int &incB, unsigned int &count);
MStatus getArgUVDblDbl(const MArgList& args, MDoubleArray  & uvA,MDoubleArray  & dblB, MDoubleArray  & dblC,unsigned int &incA,unsigned int &incB,unsigned int &incC, unsigned int &count);

MStatus getArgVec(const MArgList& args, MDoubleArray  & vecA,  unsigned int &count);
MStatus getArgVecVec(const MArgList& args, MDoubleArray  & vecA,MDoubleArray  & vecB, unsigned int &incA,unsigned int &incB, unsigned int &count);
MStatus getArgVecDbl(const MArgList& args, MDoubleArray  & vecA,MDoubleArray  & dblB, unsigned int &incA,unsigned int &incB, unsigned int &count);
MStatus getArgVecMat(const MArgList& args, MDoubleArray  & vecA,MDoubleArray  & matB, unsigned int &incA,unsigned int &incB, unsigned int &count);
MStatus getArgVecDblDbl(const MArgList& args, MDoubleArray  & vecA,MDoubleArray  & dblB, MDoubleArray  & dblC,unsigned int &incA,unsigned int &incB,unsigned int &incC, unsigned int &count);
MStatus getArgVecVecDbl(const MArgList& args, MDoubleArray  & vecA,MDoubleArray  & vecB, MDoubleArray  & dblC,unsigned int &incA,unsigned int &incB,unsigned int &incC, unsigned int &count);

MStatus getArgMat(const MArgList& args, MDoubleArray  & matA,  unsigned int &count);
MStatus getArgMatMat(const MArgList& args, MDoubleArray  & matA,MDoubleArray  & matB, unsigned int &incA,unsigned int &incB, unsigned int &count);
MStatus getArgMatDbl(const MArgList& args, MDoubleArray  & matA,MDoubleArray  & dblB, unsigned int &incA,unsigned int &incB, unsigned int &count);
MStatus getArgMatDblDbl(const MArgList& args, MDoubleArray  & matA,MDoubleArray  & dblB,MDoubleArray  & dblC, unsigned int &incA,unsigned int &incB,unsigned int &incC,unsigned int &count);
MStatus getArgMatDblDblDbl(const MArgList& args, MDoubleArray  & matA,MDoubleArray  & dblB,MDoubleArray  & dblC,MDoubleArray  & dblD, unsigned int &incA,unsigned int &incB,unsigned int &incC,unsigned int &incD, unsigned int &count);
MStatus getArgMatMatDbl(const MArgList& args, MDoubleArray  & matA,MDoubleArray  & matB, MDoubleArray  & dblC,unsigned int &incA,unsigned int &incB, unsigned int &incC, unsigned int &count);
}// namespace

#endif
