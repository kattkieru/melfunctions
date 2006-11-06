#ifndef _mUVManagementCmd_h_
#define _mUVManagementCmd_h_

#include "mHelperMacros.h"


namespace melfunctions
{
// declare the various matrix cmd classes
// wrapped in a macro, check out "helperMacros.h"

DECLARE_COMMAND(mUVCreate)

DECLARE_COMMAND(mUVSize)
DECLARE_COMMAND(mUVAppend)
DECLARE_COMMAND(mUVGet)
DECLARE_COMMAND(mUVSet)
   
DECLARE_COMMAND(mUVGetU)
DECLARE_COMMAND(mUVGetV)
DECLARE_COMMAND(mUVGetComponent)

DECLARE_COMMAND(mUVSetU)
DECLARE_COMMAND(mUVSetV)
DECLARE_COMMAND(mUVSetComponent)

}//end namespace
#endif
