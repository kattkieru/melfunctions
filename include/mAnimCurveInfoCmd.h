/* COPYRIGHT --
 *
 * This file is part of melfunctions, a collection of mel commands to for Autodesk Maya.
 * melfunctions is (c) 2006 Carsten Kolve <carsten@kolve.com>
 * and distributed under the terms of the GNU GPL V2.
 * This contribution to melfunctions is (c) Rising Sun Pictures PTY Ltd, www.rsp.com.au 
 * See the ./License-GPL.txt file in the source tree root for more information.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


//
//
#ifndef _mAnimCurveInfoCmd_h_
#define _mAnimCurveInfoCmd_h_


#include <maya/MDoubleArray.h> 
#include <maya/MDagPath.h> 
#include <maya/MFnMesh.h> 


namespace melfunctions
{

static const char* MACI_COMMAND_NAME = "mAnimCurveInfo";


class mAnimCurveInfo : public MPxCommand 
{ 

public: 
                   mAnimCurveInfo(); 
    virtual        ~mAnimCurveInfo(){}; 
    MStatus        doIt( const MArgList& ); 
    MStatus        redoIt(); 
    static void*   creator(); 
    
private:
	MDoubleArray mDblArg;
    MString		 mAnimCurveName;
    
}; 


}//end namespace


#endif


