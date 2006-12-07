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

#ifndef _mDoubleManagementCmd_h_
#define _mDoubleManagementCmd_h_

#include "mHelperMacros.h"


namespace melfunctions
{
// wrapped in a macro, check out "helperMacros.h"

DECLARE_COMMAND(mDblCreate)
DECLARE_COMMAND(mDblSize)
DECLARE_COMMAND(mDblAppend)
DECLARE_COMMAND(mDblGet)
DECLARE_COMMAND(mDblSet)

}//end namespace
#endif
