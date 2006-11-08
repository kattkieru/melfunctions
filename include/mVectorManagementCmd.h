#ifndef _mVectorManagementCmd_h_
#define _mVectorManagementCmd_h_

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
