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
#ifndef _mInstancerInfoCmd_h_
#define _mInstancerInfoCmd_h_


#include <maya/MDoubleArray.h> 
#include <maya/MDagPath.h> 
#include <maya/MFnInstancer.h> 


namespace melfunctions
{

static const char* MII_COMMAND_NAME = "mInstancerInfo";

static const char* MII_HELP_FLAG = "h";
static const char* MII_HELP_FLAG_LONG = "help";

// things that can be and queried
static const char* MII_BBMIN_FLAG = "bbmin";
static const char* MII_BBMIN_FLAG_LONG = "boundingBoxMin";

static const char* MII_BBMAX_FLAG = "bbmax";
static const char* MII_BBMAX_FLAG_LONG = "boundingBoxMax";

static const char* MII_MATRIX_FLAG = "mat";
static const char* MII_MATRIX_FLAG_LONG = "matrix";

static const char* MII_COUNT_FLAG = "co";
static const char* MII_COUNT_FLAG_LONG = "count";


#define MII_CMD_ACTION_NONE 		0
#define MII_CMD_ACTION_BBMIN 		1
#define MII_CMD_ACTION_BBMAX 		2
#define MII_CMD_ACTION_COUNT 		3
#define MII_CMD_ACTION_MATRIX 		4

#define MII_BB_MIN 		0
#define MII_BB_MAX 		1


class mInstancerInfo : public MPxCommand 
{
   public:
                   mInstancerInfo();
                   ~mInstancerInfo(){};
       
       MStatus     doIt( const MArgList& );
       MStatus     redoIt();            
       
       static      void* creator();
       bool        argParseIsFlagSet( const MArgList& args, const char *shortFlag, const char *longFlag, int &flagIndex);
       MStatus     argParseGetObjectStringArg (const MArgList& args,MString& name);
       MStatus     argParseGetStringArg( const MArgList& args, const char *shortFlag, const char *longFlag, MString &argStr, bool &flagSet);
 	   MStatus     getBoundingInfo(MFnInstancer &instancerFn, int count, short bbType, MDoubleArray &bbCorner)  ;     
                           
       MStatus     parseArgs( const MArgList& args );
       MStatus     help() const;
       
   private:            
                     
       short           mAction;

       bool            mHelpFlagSet;

       MString         mInstancerName;          
       MDagPath        mInstancerDP;                 
	
};


}//end namespace


#endif


