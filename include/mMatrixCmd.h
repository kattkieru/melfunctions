#ifndef _mMatrixCmd_h_
#define _mMatrixCmd_h_

#include "mHelperMacros.h"


namespace melfunctions
{
// declare the various matrix cmd classes
// wrapped in a macro, check out "helperMacros.h"

DECLARE_COMMAND(mMatCreate)
DECLARE_COMMAND(mMatSize)
DECLARE_COMMAND(mMatAppend)
DECLARE_COMMAND(mMatGet)
DECLARE_COMMAND(mMatSet)

DECLARE_COMMAND(mMatAdd)
DECLARE_COMMAND(mMatSub)
DECLARE_COMMAND(mMatMult)
DECLARE_COMMAND(mMatDblMult)
DECLARE_COMMAND(mMatIsEqual)
DECLARE_COMMAND(mMatIsNotEqual)
DECLARE_COMMAND(mMatIsEquivalent)
DECLARE_COMMAND(mMatIsSingular)
DECLARE_COMMAND(mMatInverse)
DECLARE_COMMAND(mMatTranspose)
DECLARE_COMMAND(mMatHomogenize)
DECLARE_COMMAND(mMatAdjoint)
DECLARE_COMMAND(mMatDet4x4)
DECLARE_COMMAND(mMatDet3x3)
DECLARE_COMMAND(mMatGetComponent)
DECLARE_COMMAND(mMatSetComponent)
/*
DECLARE_COMMAND(mMatGetRow)
DECLARE_COMMAND(mMatSetRow)
DECLARE_COMMAND(mMatGetColumn)
DECLARE_COMMAND(mMatSetColumn)
DECLARE_COMMAND(mMatGetTranslation)
DECLARE_COMMAND(mMatSetTranslation)
DECLARE_COMMAND(mMatAddTranslation)
DECLARE_COMMAND(mMatGetRotation)
DECLARE_COMMAND(mMatSetRotation)
DECLARE_COMMAND(mMatAddRotation)
DECLARE_COMMAND(mMatGetScale)
DECLARE_COMMAND(mMatSetScale)
DECLARE_COMMAND(mMatAddScale)
DECLARE_COMMAND(mMatGetShear)
DECLARE_COMMAND(mMatSetShear)
DECLARE_COMMAND(mMatAddShear)
*/
}//end namespace
#endif
