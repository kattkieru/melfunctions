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

#ifndef _mAttrCmd_h_
#define _mAttrCmd_h_

#include <maya/MPxCommand.h> 
#include <maya/MArgList.h> 
#include <maya/MVectorArray.h> 

namespace melfunctions
{

class mDblSetAttr : public MPxCommand 
{ 

public: 
                   mDblSetAttr(); 
    virtual        ~mDblSetAttr(){}; 
    MStatus        doIt( const MArgList& ); 
    MStatus        redoIt(); 
    MStatus        undoIt(); 
    bool           isUndoable() const; 
    static void*   creator(); 
    
private:
	MDoubleArray mDblArg, mDblOld;
    MString		 mNodeAttr;
    
}; 

class mVecSetAttr : public MPxCommand 
{ 

public: 
                   mVecSetAttr(); 
    virtual        ~mVecSetAttr(){}; 
    MStatus        doIt( const MArgList& ); 
    MStatus        redoIt(); 
    MStatus        undoIt(); 
    bool           isUndoable() const; 
    static void*   creator(); 
    
private:
	MVectorArray mVecArg, mVecOld;
    MString		 mNodeAttr;
    
}; 


}//end namespace
#endif
