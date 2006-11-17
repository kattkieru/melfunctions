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
// cmd that allows querying of mesh parameters based on uv information
//
#ifndef _mUVMeshInfoCmd_h_
#define _mUVMeshInfoCmd_h_


#include <maya/MDoubleArray.h> 
#include <maya/MDagPath.h> 


namespace melfunctions
{

static const char* MUVMI_COMMAND_NAME = "mUVMeshInfo";

static const char* MUVMI_HELP_FLAG = "h";
static const char* MUVMI_HELP_FLAG_LONG = "help";

// things that can be edited and queried
static const char* MUVMI_UV_FLAG = "uv";
static const char* MUVMI_UV_FLAG_LONG = "UV";

static const char* MUVMI_UV_SET_FLAG = "uvs";
static const char* MUVMI_UV_SET_FLAG_LONG = "uvSet";

static const char* MUVMI_WORLD_SPACE_FLAG = "ws";
static const char* MUVMI_WORLD_SPACE_FLAG_LONG = "worldSpace";

static const char* MUVMI_OBJECT_SPACE_FLAG = "os";
static const char* MUVMI_OBJECT_SPACE_FLAG_LONG = "objectSpace";

static const char* MUVMI_POSITION_FLAG = "pos";
static const char* MUVMI_POSITION_FLAG_LONG = "position";

static const char* MUVMI_NORMAL_FLAG = "norm";
static const char* MUVMI_NORMAL_FLAG_LONG = "normal";

static const char* MUVMI_TANGENT_FLAG = "tan";
static const char* MUVMI_TANGENT_FLAG_LONG = "tangent";

static const char* MUVMI_FACE_INDEX_FLAG = "fi";
static const char* MUVMI_FACE_INDEX_FLAG_LONG = "faceIndex";


#define MUVMI_CMD_ACTION_NONE 0
#define MUVMI_CMD_ACTION_POSITION 1
#define MUVMI_CMD_ACTION_NORMAL 2
#define MUVMI_CMD_ACTION_TANGENT 3
//#define CMD_ACTION_FACE_INDEX 4

#define MUVMI_CMD_SPACE_WORLD 0
#define MUVMI_CMD_SPACE_OBJECT 1


class mUVMeshInfo : public MPxCommand 
{
		public:
						mUVMeshInfo();
                        ~mUVMeshInfo(){};
			
			MStatus		doIt( const MArgList& );
			MStatus		redoIt();            

    		static		void* creator();
            bool 		argParseIsFlagSet( const MArgList& args, const char *shortFlag, const char *longFlag, int &flagIndex);
			MStatus 	argParseGetObjectStringArg (const MArgList& args,MString& name);
            MStatus		argParseGetDblArrayArg( const MArgList& args,const char *shortFlag, const char *longFlag, MDoubleArray &argDblA, bool &flagSet);
			MStatus 	argParseGetStringArg( const MArgList& args, const char *shortFlag, const char *longFlag, MString &argStr, bool &flagSet);
                                
            MStatus     parseArgs( const MArgList& args );
            MStatus     help() const;
            
        private:            
                          
			short			mAction;
			short			mSpace;            

            bool            mHelpFlagSet;
            bool            mUVSetSet;            
            MString			mUVSet;
            MDoubleArray	mUV;

			MString 		mMeshName;			
            MDagPath 		mMeshDP;			
            
};


}//end namespace


#endif


