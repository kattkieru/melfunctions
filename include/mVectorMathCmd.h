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

#ifndef _mVectorCmd_h_
#define _mVectorCmd_h_

#include "mHelperMacros.h"


namespace melfunctions
{
// declare the various matrix cmd classes
// wrapped in a macro, check out "helperMacros.h"

DECLARE_COMMAND(mVecAdd)
DECLARE_COMMAND(mVecSub)

DECLARE_COMMAND(mVecDblMult)
DECLARE_COMMAND(mVecDblDiv)
DECLARE_COMMAND(mVecNegate)

DECLARE_COMMAND(mVecDegToRad)
DECLARE_COMMAND(mVecRadToDeg)

DECLARE_COMMAND(mVecMatMult)

DECLARE_COMMAND(mVecMult)
DECLARE_COMMAND(mVecCross)
DECLARE_COMMAND(mVecDot)

DECLARE_COMMAND(mVecRotateByEuler)
//DECLARE_COMMAND(mVecRotateByQuat)
DECLARE_COMMAND(mVecRotateByAxisAngle)
DECLARE_COMMAND(mVecAimUpToEuler)
DECLARE_COMMAND(mVecAngle)

DECLARE_COMMAND(mVecIsEqual)
DECLARE_COMMAND(mVecIsNotEqual)
DECLARE_COMMAND(mVecIsEquivalent)
DECLARE_COMMAND(mVecSetLength)

DECLARE_COMMAND(mVecLength)
DECLARE_COMMAND(mVecLengthSqr)
DECLARE_COMMAND(mVecDistance)
DECLARE_COMMAND(mVecNormal)



DECLARE_COMMAND(mVecLerp)    
DECLARE_COMMAND(mVecSlerp) 
    

}//end namespace
#endif

