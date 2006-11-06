#ifndef _mVectorCmd_h_
#define _mVectorCmd_h_

#include "mHelperMacros.h"


namespace melfunctions
{
// declare the various matrix cmd classes
// wrapped in a macro, check out "helperMacros.h"

DECLARE_COMMAND(mVecCreate)
DECLARE_COMMAND(mVecSize)
DECLARE_COMMAND(mVecAppend)
DECLARE_COMMAND(mVecGet)
DECLARE_COMMAND(mVecSet)

DECLARE_COMMAND(mVecAdd)
DECLARE_COMMAND(mVecSub)

DECLARE_COMMAND(mVecDblMult)
DECLARE_COMMAND(mVecDblDiv)
DECLARE_COMMAND(mVecNegate)

DECLARE_COMMAND(mVecMatMult)

DECLARE_COMMAND(mVecMult)
DECLARE_COMMAND(mVecCross)
DECLARE_COMMAND(mVecDot)

DECLARE_COMMAND(mVecRotateByEuler)
//DECLARE_COMMAND(mVecRotateByQuat)
DECLARE_COMMAND(mVecRotateByAxisAngle)

DECLARE_COMMAND(mVecIsEqual)
DECLARE_COMMAND(mVecIsNotEqual)
DECLARE_COMMAND(mVecIsEquivalent)

DECLARE_COMMAND(mVecSetLength)

DECLARE_COMMAND(mVecLength)
DECLARE_COMMAND(mVecLengthSqr)
DECLARE_COMMAND(mVecDistance)

DECLARE_COMMAND(mVecNormal)

DECLARE_COMMAND(mVecAngle)

DECLARE_COMMAND(mVecLerp)    
DECLARE_COMMAND(mVecSlerp) 
    
DECLARE_COMMAND(mVecGetX)
DECLARE_COMMAND(mVecGetY)
DECLARE_COMMAND(mVecGetZ)
DECLARE_COMMAND(mVecGetComponent)


DECLARE_COMMAND(mVecSetX)
DECLARE_COMMAND(mVecSetY)
DECLARE_COMMAND(mVecSetZ)
DECLARE_COMMAND(mVecSetComponent)

}//end namespace
#endif
