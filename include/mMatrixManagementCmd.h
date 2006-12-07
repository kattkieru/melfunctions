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

#ifndef _mMatrixManagementCmd_h_
#define _mMatrixManagementCmd_h_

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
DECLARE_COMMAND(mMatGetComponent)
DECLARE_COMMAND(mMatSetComponent)

}//end namespace
#endif
