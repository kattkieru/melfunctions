//
// cmd that allows querying of mesh parameters based on uv information
//
#ifndef _mVertexMeshInfoCmd_h_
#define _mVertexMeshInfoCmd_h_


#include <maya/MDoubleArray.h> 
#include <maya/MDagPath.h> 
#include <maya/MFnMesh.h> 


namespace melfunctions
{

static const char* MVMI_COMMAND_NAME = "mVertexMeshInfo";

static const char* MVMI_HELP_FLAG = "h";
static const char* MVMI_HELP_FLAG_LONG = "help";

// things that can be and queried
static const char* MVMI_UV_FLAG = "uv";
static const char* MVMI_UV_FLAG_LONG = "UV";

static const char* MVMI_UV_SET_FLAG = "uvs";
static const char* MVMI_UV_SET_FLAG_LONG = "uvSet";

static const char* MVMI_COLOR_FLAG = "col";
static const char* MVMI_COLOR_FLAG_LONG = "color";

static const char* MVMI_COLOR_SET_FLAG = "cols";
static const char* MVMI_COLOR_SET_FLAG_LONG = "colorSet";

static const char* MVMI_WORLD_SPACE_FLAG = "ws";
static const char* MVMI_WORLD_SPACE_FLAG_LONG = "worldSpace";

static const char* MVMI_OBJECT_SPACE_FLAG = "os";
static const char* MVMI_OBJECT_SPACE_FLAG_LONG = "objectSpace";

static const char* MVMI_POSITION_FLAG = "pos";
static const char* MVMI_POSITION_FLAG_LONG = "position";

static const char* MVMI_NORMAL_FLAG = "norm";
static const char* MVMI_NORMAL_FLAG_LONG = "normal";

static const char* MVMI_BINORMAL_FLAG = "bin";
static const char* MVMI_BINORMAL_FLAG_LONG = "binormal";

static const char* MVMI_TANGENT_FLAG = "tan";
static const char* MVMI_TANGENT_FLAG_LONG = "tangent";

static const char* MVMI_VERTEX_FLAG = "vert";
static const char* MVMI_VERTEX_FLAG_LONG = "vertex";

static const char* MVMI_COUNT_FLAG = "co";
static const char* MVMI_COUNT_FLAG_LONG = "count";


#define MVMI_CMD_ACTION_NONE 0
#define MVMI_CMD_ACTION_POSITION 1
#define MVMI_CMD_ACTION_NORMAL 2
#define MVMI_CMD_ACTION_TANGENT 3
#define MVMI_CMD_ACTION_BINORMAL 4
#define MVMI_CMD_ACTION_UV 5
#define MVMI_CMD_ACTION_COLOR 6

#define MVMI_CMD_SPACE_WORLD 0
#define MVMI_CMD_SPACE_OBJECT 1


class mVertexMeshInfo : public MPxCommand 
{
   public:
                   mVertexMeshInfo();
                   ~mVertexMeshInfo(){};
       
       MStatus     doIt( const MArgList& );
       MStatus     redoIt();            
       MStatus     doVertexUV(MFnMesh &meshFn);                        
       MStatus     doVertexNormal(MFnMesh &meshFn);                                    
       MStatus     doVertexPosition(MFnMesh &meshFn);                                                
       MStatus     doVertexColor(MFnMesh &meshFn);                                                       
       MStatus     doVertexTangent(MFnMesh &meshFn);                   
       MStatus     doVertexBinormal(MFnMesh &meshFn);                          
       
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
       short           mSpace;            

       bool            mHelpFlagSet;

       bool            mUVSetSet;            
       MString         mUVSet;
       
       bool            mColorSetSet;            
       MString         mColorSet;

       MDoubleArray    mVertId;
       bool            mVertIdSet;                        
       int             mVertIdNum;                               

       MString         mMeshName;          
       MDagPath        mMeshDP;            
            
};


}//end namespace


#endif


