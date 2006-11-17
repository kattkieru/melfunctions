/* COPYRIGHT --
 *
 * This file is part of melfunctions, a collection of mel commands to for Autodesk Maya.
 * melfunctions is (c) 2006 Carsten Kolve <carsten@kolve.com>
 * This contribution to melfunctions is (c) Rising Sun Pictures PTY Ltd, www.rsp.com.au 
 * See the ./License-GPL.txt file in the source tree root for more information.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

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
