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
#ifndef _m2dShaderInfoCmd_h_
#define _m2dShaderInfoCmd_h_


#include <maya/MDoubleArray.h> 
#include <maya/MDagPath.h> 
#include <maya/MFnMesh.h> 


namespace melfunctions
{

static const char* MSI_COMMAND_NAME = "m2dShaderInfo";

static const char* MSI_HELP_FLAG = "h";
static const char* MSI_HELP_FLAG_LONG = "help";

// things that can be and queried
static const char* MSI_U_FLAG = "u";
static const char* MSI_U_FLAG_LONG = "U";

static const char* MSI_V_FLAG = "v";
static const char* MSI_V_FLAG_LONG = "V";

static const char* MSI_UV_FLAG = "uv";
static const char* MSI_UV_FLAG_LONG = "UV";

static const char* MSI_COLOR_FLAG = "col";
static const char* MSI_COLOR_FLAG_LONG = "color";

static const char* MSI_VALUE_FLAG = "val";
static const char* MSI_VALUE_FLAG_LONG = "value";

static const char* MSI_TRANSPARENCY_FLAG = "trans";
static const char* MSI_TRANSPARENCY_FLAG_LONG = "transparency";


#define MSI_CMD_ACTION_NONE 0
#define MSI_CMD_ACTION_COLOR 1
#define MSI_CMD_ACTION_VALUE 2
#define MSI_CMD_ACTION_TRANSPARENCY 3


class m2dShaderInfo : public MPxCommand 
{
   public:
                   m2dShaderInfo();
                   ~m2dShaderInfo(){};
       
       MStatus     doIt( const MArgList& );
       MStatus     redoIt();            
       MStatus     doShaderValue(MFnMesh &meshFn);                        
       MStatus     doShaderColor(MFnMesh &meshFn);                                    
       
       MStatus     getUVSet(MFnMesh &meshFn);                          
       
       static      void* creator();
       bool        argParseIsFlagSet( const MArgList& args, const char *shortFlag, const char *longFlag, int &flagIndex);
       MStatus     argParseGetObjectStringArg (const MArgList& args,MString& name);
       MStatus     argParseGetDblArrayArg( const MArgList& args,const char *shortFlag, const char *longFlag, MDoubleArray &argDblA, bool &flagSet);
       MStatus     argParseGetStringArg( const MArgList& args, const char *shortFlag, const char *longFlag, MString &argStr, bool &flagSet);
                           
       MStatus     parseArgs( const MArgList& args );
       MStatus     help() const;
       
   private:            
                     
       short           mAction;

       bool            mHelpFlagSet;
       bool				mUVSet;
       bool				mUSet;
       bool				mVSet;              

       MString         m2dShaderName;          
	
    	MDoubleArray mU,mV, mUV;
            
};


}//end namespace


#endif


