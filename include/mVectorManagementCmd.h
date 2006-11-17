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
