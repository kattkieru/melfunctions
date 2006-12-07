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

#ifndef _mMatrixMathCmd_h_
#define _mMatrixMathCmd_h_

#include "mHelperMacros.h"


namespace melfunctions
{
// declare the various matrix cmd classes
// wrapped in a macro, check out "helperMacros.h"

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
