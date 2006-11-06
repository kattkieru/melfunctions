//
// plugin.cpp, de/register all commands with maya
//

#include <maya/MFnPlugin.h>

#include "../include/mHelperMacros.h"
#include "../include/mMatrixCmd.h"
#include "../include/mVectorCmd.h"
#include "../include/mDoubleCmd.h"
#include "../include/mUVManagementCmd.h"

#include "../include/mNeighbourInfoCmd.h"

#include "../include/mUVMeshInfoCmd.h"
#include "../include/mVertexMeshInfoCmd.h"



MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "Carsten Kolve", "0.3", "Any");
	MGlobal::displayInfo("----------------------------------------------------------------------");
	MGlobal::displayInfo("melfunctions 0.3 (c) Carsten Kolve, 2006");
	MGlobal::displayInfo("Free for educational and private use - charityware for commercial use!");
	MGlobal::displayInfo("Visit www.kolve.com for news, updates and information on the licenses!");
	MGlobal::displayInfo("-----------------------------------------------------------------------");
    
	// matrix
    REGISTER_COMMAND(melfunctions,mMatCreate)
    REGISTER_COMMAND(melfunctions,mMatSize)
    REGISTER_COMMAND(melfunctions,mMatAppend)
    REGISTER_COMMAND(melfunctions,mMatGet)
    REGISTER_COMMAND(melfunctions,mMatSet)
	REGISTER_COMMAND(melfunctions,mMatAdd)
	REGISTER_COMMAND(melfunctions,mMatSub)
	REGISTER_COMMAND(melfunctions,mMatMult)
	REGISTER_COMMAND(melfunctions,mMatDblMult)
	REGISTER_COMMAND(melfunctions,mMatIsEqual)
	REGISTER_COMMAND(melfunctions,mMatIsNotEqual)    
	REGISTER_COMMAND(melfunctions,mMatIsEquivalent)
	REGISTER_COMMAND(melfunctions,mMatIsSingular)
	REGISTER_COMMAND(melfunctions,mMatInverse)
	REGISTER_COMMAND(melfunctions,mMatTranspose)
	REGISTER_COMMAND(melfunctions,mMatHomogenize)
	REGISTER_COMMAND(melfunctions,mMatAdjoint)
	REGISTER_COMMAND(melfunctions,mMatDet4x4)
	REGISTER_COMMAND(melfunctions,mMatDet3x3)
	REGISTER_COMMAND(melfunctions,mMatGetComponent)
	REGISTER_COMMAND(melfunctions,mMatSetComponent)

/*	REGISTER_COMMAND(mMatGetRow)
	REGISTER_COMMAND(mMatSetRow)
	REGISTER_COMMAND(mMatGetColumn)
	REGISTER_COMMAND(mMatSetColumn)
	REGISTER_COMMAND(mMatGetTranslation)
	REGISTER_COMMAND(mMatSetTranslation)
	REGISTER_COMMAND(mMatAddTranslation)
	REGISTER_COMMAND(mMatGetRotation)
	REGISTER_COMMAND(mMatSetRotation)
	REGISTER_COMMAND(mMatAddRotation)
	REGISTER_COMMAND(mMatGetScale)
	REGISTER_COMMAND(mMatSetScale)
	REGISTER_COMMAND(mMatAddScale)
	REGISTER_COMMAND(mMatGetShear)
	REGISTER_COMMAND(mMatSetShear)
	REGISTER_COMMAND(mMatAddShear)
*/

	// vector
    REGISTER_COMMAND(melfunctions,mVecCreate)
    REGISTER_COMMAND(melfunctions,mVecSize)
    REGISTER_COMMAND(melfunctions,mVecAppend)
    REGISTER_COMMAND(melfunctions,mVecGet)
    REGISTER_COMMAND(melfunctions,mVecSet)
    
	REGISTER_COMMAND(melfunctions,mVecAdd)
	REGISTER_COMMAND(melfunctions,mVecSub)
	REGISTER_COMMAND(melfunctions,mVecDblMult)
	REGISTER_COMMAND(melfunctions,mVecDblDiv)
	REGISTER_COMMAND(melfunctions,mVecNegate)    
	REGISTER_COMMAND(melfunctions,mVecMatMult)
	REGISTER_COMMAND(melfunctions,mVecCross)
	REGISTER_COMMAND(melfunctions,mVecDot)
	REGISTER_COMMAND(melfunctions,mVecMult)    
	REGISTER_COMMAND(melfunctions,mVecRotateByEuler)
//	REGISTER_COMMAND(melfunctions,mVecRotateByQuat)
	REGISTER_COMMAND(melfunctions,mVecRotateByAxisAngle)
	REGISTER_COMMAND(melfunctions,mVecIsEqual)
	REGISTER_COMMAND(melfunctions,mVecIsNotEqual)
	REGISTER_COMMAND(melfunctions,mVecIsEquivalent)
	REGISTER_COMMAND(melfunctions,mVecLength)
	REGISTER_COMMAND(melfunctions,mVecLengthSqr)
	REGISTER_COMMAND(melfunctions,mVecSetLength)    
	REGISTER_COMMAND(melfunctions,mVecDistance)
	REGISTER_COMMAND(melfunctions,mVecNormal)
	REGISTER_COMMAND(melfunctions,mVecAngle)
	REGISTER_COMMAND(melfunctions,mVecLerp)    
	REGISTER_COMMAND(melfunctions,mVecSlerp)        
	REGISTER_COMMAND(melfunctions,mVecGetX)
	REGISTER_COMMAND(melfunctions,mVecGetY)
	REGISTER_COMMAND(melfunctions,mVecGetZ)
	REGISTER_COMMAND(melfunctions,mVecGetComponent)
	REGISTER_COMMAND(melfunctions,mVecSetX)
	REGISTER_COMMAND(melfunctions,mVecSetY)
	REGISTER_COMMAND(melfunctions,mVecSetZ)
	REGISTER_COMMAND(melfunctions,mVecSetComponent)


//uv
	REGISTER_COMMAND(melfunctions,mUVCreate)

	REGISTER_COMMAND(melfunctions,mUVSize)
	REGISTER_COMMAND(melfunctions,mUVAppend)
	REGISTER_COMMAND(melfunctions,mUVGet)
	REGISTER_COMMAND(melfunctions,mUVSet)
   
	REGISTER_COMMAND(melfunctions,mUVGetU)
	REGISTER_COMMAND(melfunctions,mUVGetV)
	REGISTER_COMMAND(melfunctions,mUVGetComponent)

	REGISTER_COMMAND(melfunctions,mUVSetU)
	REGISTER_COMMAND(melfunctions,mUVSetV)
	REGISTER_COMMAND(melfunctions,mUVSetComponent)


// double
	REGISTER_COMMAND(melfunctions,mDblCreate)
	REGISTER_COMMAND(melfunctions,mDblSize)
	REGISTER_COMMAND(melfunctions,mDblAppend)
	REGISTER_COMMAND(melfunctions,mDblGet)
	REGISTER_COMMAND(melfunctions,mDblSet)

	REGISTER_COMMAND(melfunctions,mDblAdd)
	REGISTER_COMMAND(melfunctions,mDblSub)
	REGISTER_COMMAND(melfunctions,mDblMult)
	REGISTER_COMMAND(melfunctions,mDblDiv)
	REGISTER_COMMAND(melfunctions,mDblNegate)

	REGISTER_COMMAND(melfunctions,mDblIsEqual)
	REGISTER_COMMAND(melfunctions,mDblIsNotEqual)
	REGISTER_COMMAND(melfunctions,mDblIsEquivalent)

	REGISTER_COMMAND(melfunctions,mDblMin)
	REGISTER_COMMAND(melfunctions,mDblMax)

	REGISTER_COMMAND(melfunctions,mDblAbs)
	REGISTER_COMMAND(melfunctions,mDblSign)

	REGISTER_COMMAND(melfunctions,mDblDegToRad)
	REGISTER_COMMAND(melfunctions,mDblRadToDeg)

	REGISTER_COMMAND(melfunctions,mDblSin)
	REGISTER_COMMAND(melfunctions,mDblCos)
	REGISTER_COMMAND(melfunctions,mDblTan)

	REGISTER_COMMAND(melfunctions,mDblASin)
	REGISTER_COMMAND(melfunctions,mDblACos)
	REGISTER_COMMAND(melfunctions,mDblATan)

	REGISTER_COMMAND(melfunctions,mDblHSin)
	REGISTER_COMMAND(melfunctions,mDblHCos)
	REGISTER_COMMAND(melfunctions,mDblHTan)

	REGISTER_COMMAND(melfunctions,mDblLog)
	REGISTER_COMMAND(melfunctions,mDblLog10)
	REGISTER_COMMAND(melfunctions,mDblPow)

	REGISTER_COMMAND(melfunctions,mDblMod)
	REGISTER_COMMAND(melfunctions,mDblTrunc)
	REGISTER_COMMAND(melfunctions,mDblFrac)
	REGISTER_COMMAND(melfunctions,mDblFloor)
	REGISTER_COMMAND(melfunctions,mDblCeil)
	REGISTER_COMMAND(melfunctions,mDblRound)

	REGISTER_COMMAND(melfunctions,mDblSqrt)
//	REGISTER_COMMAND(melfunctions,mDblCbrt)
	REGISTER_COMMAND(melfunctions,mDblLerp)

	REGISTER_COMMAND(melfunctions,mDblClamp)
	REGISTER_COMMAND(melfunctions,mDblPulse)
	REGISTER_COMMAND(melfunctions,mDblLinStep)
	REGISTER_COMMAND(melfunctions,mDblFit)

	// neighbour info
    REGISTER_COMMAND(melfunctions,mNeighbourInfo)
    
    // mesh functionality
//    REGISTER_COMMAND(melfunctions,mUVMeshInfo)    
    REGISTER_COMMAND(melfunctions,mVertexMeshInfo)        


	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );

	// matrix
    DEREGISTER_COMMAND(mMatCreate)
    DEREGISTER_COMMAND(mMatSize)
    DEREGISTER_COMMAND(mMatAppend)
    DEREGISTER_COMMAND(mMatGet)
    DEREGISTER_COMMAND(mMatSet)
    
	DEREGISTER_COMMAND(mMatAdd)
	DEREGISTER_COMMAND(mMatSub)
	DEREGISTER_COMMAND(mMatMult)
	DEREGISTER_COMMAND(mMatDblMult)
	DEREGISTER_COMMAND(mMatIsEqual)
    DEREGISTER_COMMAND(mMatIsNotEqual)    
	DEREGISTER_COMMAND(mMatIsEquivalent)
	DEREGISTER_COMMAND(mMatIsSingular)
	DEREGISTER_COMMAND(mMatInverse)
	DEREGISTER_COMMAND(mMatTranspose)
	DEREGISTER_COMMAND(mMatHomogenize)
	DEREGISTER_COMMAND(mMatAdjoint)
	DEREGISTER_COMMAND(mMatDet4x4)
	DEREGISTER_COMMAND(mMatDet3x3)
	DEREGISTER_COMMAND(mMatGetComponent)
	DEREGISTER_COMMAND(mMatSetComponent)

	// vector
    DEREGISTER_COMMAND(mVecCreate)
    DEREGISTER_COMMAND(mVecSize)
    DEREGISTER_COMMAND(mVecAppend)
    DEREGISTER_COMMAND(mVecGet)
    DEREGISTER_COMMAND(mVecSet)
    
	DEREGISTER_COMMAND(mVecAdd)
	DEREGISTER_COMMAND(mVecSub)
    
	DEREGISTER_COMMAND(mVecDblMult)
	DEREGISTER_COMMAND(mVecDblDiv)
    DEREGISTER_COMMAND(mVecNegate)    
	DEREGISTER_COMMAND(mVecMatMult)
	DEREGISTER_COMMAND(mVecCross)
	DEREGISTER_COMMAND(mVecDot)
    DEREGISTER_COMMAND(mVecMult)
	DEREGISTER_COMMAND(mVecRotateByEuler)
//	DEREGISTER_COMMAND(mVecRotateByQuat)
	DEREGISTER_COMMAND(mVecRotateByAxisAngle)
	DEREGISTER_COMMAND(mVecIsEqual)
	DEREGISTER_COMMAND(mVecIsNotEqual)
	DEREGISTER_COMMAND(mVecIsEquivalent)
	DEREGISTER_COMMAND(mVecLength)
	DEREGISTER_COMMAND(mVecLengthSqr)
    DEREGISTER_COMMAND(mVecSetLength)    
	DEREGISTER_COMMAND(mVecDistance)
	DEREGISTER_COMMAND(mVecNormal)
	DEREGISTER_COMMAND(mVecAngle)
	DEREGISTER_COMMAND(mVecLerp)        
	DEREGISTER_COMMAND(mVecSlerp)            
	DEREGISTER_COMMAND(mVecGetX)
	DEREGISTER_COMMAND(mVecGetY)
	DEREGISTER_COMMAND(mVecGetZ)
	DEREGISTER_COMMAND(mVecGetComponent)
	DEREGISTER_COMMAND(mVecSetX)
	DEREGISTER_COMMAND(mVecSetY)
	DEREGISTER_COMMAND(mVecSetZ)
	DEREGISTER_COMMAND(mVecSetComponent)


//uv
	DEREGISTER_COMMAND(mUVCreate)
	DEREGISTER_COMMAND(mUVSize)
	DEREGISTER_COMMAND(mUVAppend)
	DEREGISTER_COMMAND(mUVGet)
	DEREGISTER_COMMAND(mUVSet)
	DEREGISTER_COMMAND(mUVGetU)
	DEREGISTER_COMMAND(mUVGetV)
	DEREGISTER_COMMAND(mUVGetComponent)
	DEREGISTER_COMMAND(mUVSetU)
	DEREGISTER_COMMAND(mUVSetV)
	DEREGISTER_COMMAND(mUVSetComponent)

    // double
	DEREGISTER_COMMAND(mDblCreate)
	DEREGISTER_COMMAND(mDblSize)
	DEREGISTER_COMMAND(mDblAppend)
	DEREGISTER_COMMAND(mDblGet)
	DEREGISTER_COMMAND(mDblSet)

	DEREGISTER_COMMAND(mDblAdd)
	DEREGISTER_COMMAND(mDblSub)
	DEREGISTER_COMMAND(mDblMult)
	DEREGISTER_COMMAND(mDblDiv)
	DEREGISTER_COMMAND(mDblNegate)

	DEREGISTER_COMMAND(mDblIsEqual)
	DEREGISTER_COMMAND(mDblIsNotEqual)
	DEREGISTER_COMMAND(mDblIsEquivalent)

	DEREGISTER_COMMAND(mDblMin)
	DEREGISTER_COMMAND(mDblMax)
    
	DEREGISTER_COMMAND(mDblAbs)
	DEREGISTER_COMMAND(mDblSign)

	DEREGISTER_COMMAND(mDblDegToRad)
	DEREGISTER_COMMAND(mDblRadToDeg)

	DEREGISTER_COMMAND(mDblSin)
	DEREGISTER_COMMAND(mDblCos)
	DEREGISTER_COMMAND(mDblTan)

	DEREGISTER_COMMAND(mDblASin)
	DEREGISTER_COMMAND(mDblACos)
	DEREGISTER_COMMAND(mDblATan)

	DEREGISTER_COMMAND(mDblHSin)
	DEREGISTER_COMMAND(mDblHCos)
	DEREGISTER_COMMAND(mDblHTan)

	DEREGISTER_COMMAND(mDblLog)
	DEREGISTER_COMMAND(mDblLog10)
	DEREGISTER_COMMAND(mDblPow)

	DEREGISTER_COMMAND(mDblMod)
	DEREGISTER_COMMAND(mDblTrunc)
	DEREGISTER_COMMAND(mDblFrac)
	DEREGISTER_COMMAND(mDblFloor)
	DEREGISTER_COMMAND(mDblCeil)
	DEREGISTER_COMMAND(mDblRound)

	DEREGISTER_COMMAND(mDblSqrt)
//	DEREGISTER_COMMAND(mDblCbrt)
	DEREGISTER_COMMAND(mDblLerp)

	DEREGISTER_COMMAND(mDblClamp)
	DEREGISTER_COMMAND(mDblPulse)
	DEREGISTER_COMMAND(mDblLinStep)
	DEREGISTER_COMMAND(mDblFit)


	// info
    DEREGISTER_COMMAND(mNeighbourInfo)
    
//    DEREGISTER_COMMAND(mUVMeshInfo)    
    DEREGISTER_COMMAND(mVertexMeshInfo)        

	return status;
}

