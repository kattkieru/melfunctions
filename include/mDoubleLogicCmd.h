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

#ifndef _mDoubleLogicCmd_h_
#define _mDoubleLogicCmd_h_

#include "mHelperMacros.h"


namespace melfunctions
{
// wrapped in a macro, check out "helperMacros.h"

DECLARE_COMMAND(mDblIsEqual)
DECLARE_COMMAND(mDblIsNotEqual)
DECLARE_COMMAND(mDblIsEquivalent)

DECLARE_COMMAND(mDblIsBigger)
DECLARE_COMMAND(mDblIsSmaller)

DECLARE_COMMAND(mDblNot)
DECLARE_COMMAND(mDblAnd)
DECLARE_COMMAND(mDblOr)
DECLARE_COMMAND(mDblXOr)


}//end namespace
#endif
