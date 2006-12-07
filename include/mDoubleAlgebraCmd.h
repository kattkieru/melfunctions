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

#ifndef _mDoubleAlgebraCmd_h_
#define _mDoubleAlgebraCmd_h_

#include "mHelperMacros.h"


namespace melfunctions
{
// wrapped in a macro, check out "helperMacros.h"

DECLARE_COMMAND(mDblAdd)
DECLARE_COMMAND(mDblSub)
DECLARE_COMMAND(mDblMult)
DECLARE_COMMAND(mDblDiv)
DECLARE_COMMAND(mDblNegate)

DECLARE_COMMAND(mDblMin)
DECLARE_COMMAND(mDblMax)

DECLARE_COMMAND(mDblAbs)
DECLARE_COMMAND(mDblSign)

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
DECLARE_COMMAND(mDblLerp)

DECLARE_COMMAND(mDblClamp)
DECLARE_COMMAND(mDblPulse)
DECLARE_COMMAND(mDblLinStep)
DECLARE_COMMAND(mDblFit)

}//end namespace
#endif
