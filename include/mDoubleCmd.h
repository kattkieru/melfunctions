#ifndef _mDoubleCmd_h_
#define _mDoubleCmd_h_

#include "mHelperMacros.h"


namespace melfunctions
{
// wrapped in a macro, check out "helperMacros.h"

DECLARE_COMMAND(mDblCreate)
DECLARE_COMMAND(mDblSize)
DECLARE_COMMAND(mDblAppend)
DECLARE_COMMAND(mDblGet)
DECLARE_COMMAND(mDblSet)

DECLARE_COMMAND(mDblAdd)
DECLARE_COMMAND(mDblSub)
DECLARE_COMMAND(mDblMult)
DECLARE_COMMAND(mDblDiv)
DECLARE_COMMAND(mDblNegate)

DECLARE_COMMAND(mDblIsEqual)
DECLARE_COMMAND(mDblIsNotEqual)
DECLARE_COMMAND(mDblIsEquivalent)

DECLARE_COMMAND(mDblMin)
DECLARE_COMMAND(mDblMax)

DECLARE_COMMAND(mDblAbs)
DECLARE_COMMAND(mDblSign)

DECLARE_COMMAND(mDblDegToRad)
DECLARE_COMMAND(mDblRadToDeg)

DECLARE_COMMAND(mDblSin)
DECLARE_COMMAND(mDblCos)
DECLARE_COMMAND(mDblTan)

DECLARE_COMMAND(mDblASin)
DECLARE_COMMAND(mDblACos)
DECLARE_COMMAND(mDblATan)

DECLARE_COMMAND(mDblHSin)
DECLARE_COMMAND(mDblHCos)
DECLARE_COMMAND(mDblHTan)

DECLARE_COMMAND(mDblLog)
DECLARE_COMMAND(mDblLog10)
DECLARE_COMMAND(mDblPow)

DECLARE_COMMAND(mDblMod)
DECLARE_COMMAND(mDblTrunc)
DECLARE_COMMAND(mDblFrac)
DECLARE_COMMAND(mDblFloor)
DECLARE_COMMAND(mDblCeil)
DECLARE_COMMAND(mDblRound)

DECLARE_COMMAND(mDblSqrt)
//DECLARE_COMMAND(mDblCbrt)
DECLARE_COMMAND(mDblLerp)

DECLARE_COMMAND(mDblClamp)
DECLARE_COMMAND(mDblPulse)
DECLARE_COMMAND(mDblLinStep)
DECLARE_COMMAND(mDblFit)

}//end namespace
#endif
