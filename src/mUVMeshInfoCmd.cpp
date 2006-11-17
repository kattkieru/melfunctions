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

#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnMesh.h>
#include <maya/MFloatArray.h>


#include "../include/mHelperMacros.h"
#include "../include/mUVMeshInfoCmd.h"
#include "../include/mHelperFunctions.h"

namespace melfunctions
{


void* mUVMeshInfo::creator()
{
	return new mUVMeshInfo;
}

//************************************************************************//
// init vars
//void mUVMeshInfo::initVariables()
mUVMeshInfo::mUVMeshInfo()
{
	
	mAction = MUVMI_CMD_ACTION_NONE;
	mSpace = MUVMI_CMD_SPACE_WORLD;
	mHelpFlagSet = false;    
	mUVSetSet = false;    

}

//************************************************************************//

MStatus mUVMeshInfo::help() const
{
    MString help( "mUVMeshInfo, melfunctions, (c) Carsten Kolve, 2006\n" );
    help += "This allows you to query information of a mapped mesh based on a UV values\n\n";
    help += "USAGE: mUVMeshInfo\n";
    help += "//\t   -uv|-UV           [float[]] [C]  Mandatory: Specify a uv array used for the query.\n";
    help += "//\t  -uvs|-UVSet        [string]  [C]  Specify a string with the UVSet to use (by default the first one is used)\n";    
    help += "//\t   -ws|-worldSpace             [C]  If specified command will return results in world space (default)\n";        
    help += "//\t   -os|-objectSpace            [C]  If specified command will return results in object space\n";        
    help += "//\t  -pos|-position               [C]  If specified command will return positions at specified uvs\n";
    help += "//\t -norm|-normal                 [C]  If specified command will return normals at specified uvs\n";;
    help += "//\t    -h|-help                   [C]  Displays this help.\n";
    help += "//\t   mesh              [string]       Manadatory: Name of mesh.\n";

    MGlobal::displayInfo( help );
    return MS::kSuccess;
}

//************************************************************************//
// now this gets quite elaborate as we can't use the arg parser as it doesn't support arrays


bool mUVMeshInfo::argParseIsFlagSet( const MArgList& args, 
                        const char *shortFlag, const char *longFlag, 
                        int &flagIndex)
{
    flagIndex = args.flagIndex( shortFlag, longFlag);
    return ( MArgList::kInvalidArgIndex != flagIndex );
}


MStatus mUVMeshInfo::argParseGetObjectStringArg (const MArgList& args,MString& name)
{
	MStatus status = MS::kSuccess;
	int argIndex = args.length() -1;
    
	name = args.asString(argIndex,&status);
	USER_ERROR_CHECK(status,("mUVMeshInfo: can't get name of mesh"));

	return status;
}

MStatus mUVMeshInfo::argParseGetDblArrayArg( const MArgList& args,  
                                const char *shortFlag, const char *longFlag, 
                                MDoubleArray &argDblA, bool &flagSet)
{
    int flagIndex;
	MStatus status = MS::kSuccess;
    if(argParseIsFlagSet(args,shortFlag, longFlag,flagIndex))
	{
		status = getDoubleArrayArg(args, (flagIndex+1), argDblA);
		USER_ERROR_CHECK(status,("mUVMeshInfo: can't get doubleArray argument for argument "+(flagIndex+1)));
		flagSet = true;
	}
	else
		flagSet = false;

    return status;
}

MStatus mUVMeshInfo::argParseGetStringArg( const MArgList& args,  
                                const char *shortFlag, const char *longFlag, 
                                MString &argStr, bool &flagSet)
{
    int flagIndex;
	MStatus status = MS::kSuccess;
    if(argParseIsFlagSet(args,shortFlag, longFlag,flagIndex))
	{
		argStr = args.asString((flagIndex+1),&status);
		USER_ERROR_CHECK(status,(" mUVMeshInfo: can't get string argument for argument nr"+(flagIndex+1)));
		flagSet = true;
	}
	else
		flagSet = false;

    return status;
}

MStatus mUVMeshInfo::parseArgs( const MArgList& args )
{
 
	MStatus status = MS::kSuccess;
	int flagNum = args.length(); // number of flags that still need to be processed
    int index;
    bool flagSet;

    
    // do we want the help
	if (argParseIsFlagSet(args,MUVMI_HELP_FLAG,MUVMI_HELP_FLAG_LONG,index) || (flagNum ==0))
	{
	     mHelpFlagSet = true;
	     return status;   
	}
    
    // get the uvs
	status = argParseGetDblArrayArg(args,MUVMI_UV_FLAG,MUVMI_UV_FLAG_LONG, mUV, flagSet );
	if (status.error()) return status;
    if(!flagSet)
    {
    	status = MS::kFailure;
    	USER_ERROR_CHECK(status,("mUVMeshInfo: -uv|UV not set, but this is mandatory!"));
    }
    else
    	flagNum-=2;
        

    // get the uvset
	status = argParseGetStringArg(args,MUVMI_UV_SET_FLAG,MUVMI_UV_SET_FLAG_LONG, mUVSet, flagSet );
	if (status.error()) return status;
    if (flagSet)
    {
    	mUVSetSet = true;
    	flagNum-=2;
    }        

	
	// get the space
	if (argParseIsFlagSet(args,MUVMI_OBJECT_SPACE_FLAG ,MUVMI_OBJECT_SPACE_FLAG_LONG ,index))
	{
		mSpace = MUVMI_CMD_SPACE_OBJECT;
		flagNum--;
	} 
	
	if (argParseIsFlagSet(args,MUVMI_WORLD_SPACE_FLAG,MUVMI_WORLD_SPACE_FLAG_LONG,index)) 
	{
		if (mSpace == MUVMI_CMD_SPACE_OBJECT)
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,("mUVMeshInfo: you can only specify one space parameter, either -ws|-worldSpace or -os|-objectSpace!"));    
		}
		else
		{
			mSpace = MUVMI_CMD_SPACE_WORLD;       
			flagNum--;
		}
	}


	flagSet =-1;


	// position or normal
	if (argParseIsFlagSet(args,MUVMI_POSITION_FLAG,MUVMI_POSITION_FLAG_LONG,index))
	{
		mAction = MUVMI_CMD_ACTION_POSITION;
		flagNum--;
	}
	
	if (argParseIsFlagSet(args,MUVMI_NORMAL_FLAG ,MUVMI_NORMAL_FLAG_LONG ,index))
	{
		if (mSpace == MUVMI_CMD_ACTION_POSITION)
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,("mUVMeshInfo: you most specify what to query for, use -pos|-position or -norm|-normal flag!"));    
		}
		else
		{	
			mAction = MUVMI_CMD_ACTION_NORMAL;
			flagNum--;
		}
	}
	
	if (mAction == MUVMI_CMD_ACTION_NONE)
	{	
		status = MS::kFailure;
		USER_ERROR_CHECK(status,("mUVMeshInfo: you most specify what to query for, use -pos|-position or -norm|-normal flag!"));    
	}


    // get the mesh
    status = argParseGetObjectStringArg(args,mMeshName);
	if (status.error()) 
		return status;
	else 
	{
		// get the dagpath to the object
		MSelectionList meshList;
		status = meshList.add(mMeshName);
		if (status = MS::kInvalidParameter)
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,("mUVMeshInfo: the provided mesh does not exist!!"));    		
		}
		
		if(meshList.length() > 1)
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,("mUVMeshInfo: more than one object match the provided mesh name!"));    		
		}
		
		status = meshList.getDagPath(0,mMeshDP);
		USER_ERROR_CHECK(status,("mUVMeshInfo: error getting mesh object! - does object exist? multiple objects with same name (use full path)?"));    		
		
		flagNum--;
	}


	if (flagNum != 0)
	{
		USER_ERROR_CHECK(MS::kFailure,"mUVMeshInfo: wrong number of arguments!");
	}

	return status;
}

//************************************************************************//

MStatus mUVMeshInfo::doIt( const MArgList& args )
{
    MStatus status = parseArgs(args);
    if(!status)
    	return status;
    else
    	return redoIt();
}

//************************************************************************//
// do the actual computation
MStatus mUVMeshInfo::redoIt()
{
    MStatus status = MS::kSuccess;

	if (mHelpFlagSet)
		return help();
/*
	cerr <<"\n---------";
    cerr <<"\naction "<<mAction;
    cerr <<"\nspace "<<mSpace;
    cerr <<"\nuvsetset "<<mUVSetSet;        
    cerr <<"\nuvSet "<<mUVSet;            
    cerr <<"\nuv "<<mUV;                
    cerr <<"\nMesh "<<mMeshDP.fullPathName();                    
*/

	// verify the arguments
	//

	// uv array
	unsigned int numUV;
	status = uvIsValid(mUV,numUV);
	if (status.error()) return status;
	
	// mesh
	
	// specified node is mesh,, or a transform of a mesh
	if ((mMeshDP.node().hasFn(MFn::kMesh) || (mMeshDP.node().hasFn(MFn::kTransform) && mMeshDP.hasFn(MFn::kMesh))))
	{
		// extend the dp if we're on the transform
		if ((mMeshDP.node().hasFn(MFn::kTransform) && mMeshDP.hasFn(MFn::kMesh)))
			mMeshDP.extendToShape();
		
		// create the fn
		MFnMesh meshFn(mMeshDP);

		// check if the mesh has any uv sets
		if (!meshFn.numUVSets())
		{
			USER_ERROR_CHECK(MS::kFailure,"mUVMeshInfo: specified mesh does not have any uv information!");		
		}
		
		
		// check the uv set
		MString uvSet = NULL;
		
		if (mUVSetSet)
		{
			MStringArray uvSets;
			meshFn.getUVSetNames(uvSets);
			
			// is the specified set part of the uv sets
			bool found =false;
			for(int i=0;i<uvSets.length();i++)
			{
				if (uvSets[i] == mUVSet)
				{
					found =true;
					uvSet = mUVSet;
				}
			}
			
			if (!found)
			{
				USER_ERROR_CHECK(MS::kFailure,"mUVMeshInfo: specified uvSet could not be found on mesh!");
			}
		}
			
		// sample for positions or normals
		float2 UV;
		MPoint point;
		MDoubleArray result(numUV*3);
		
		MSpace space;
		// WORLD SPACE
		if (mSpace == MUVMI_CMD_SPACE_WORLD)
		{		
			if (mAction == MUVMI_CMD_ACTION_POSITION)
			{
				for (int i=0;i<(numUV);i=i+1)
				{
					UV[0] = mUV[2*i];
					UV[1] = mUV[2*i+1];
					
					meshFn.getPointAtUV(0,point, UV, MSpace::kWorld, &uvSet);
					result[3*i] = point.x;
					result[3*i+1] = point.y;
					result[3*i+2] = point.z;								
				}
			}
			else if (mAction == MUVMI_CMD_ACTION_NORMAL)
			{
				MVector normal;
			
				for (int i=0;i<(numUV);i=i+1)
				{
					UV[0] = mUV[2*i];
					UV[1] = mUV[2*i+1];
				
					meshFn.getPointAtUV(0,point, UV, MSpace::kWorld, &uvSet);
					meshFn.getClosestNormal(point, normal, MSpace::kWorld);

					result[3*i] = normal.x;
					result[3*i+1] = normal.y;
					result[3*i+2] = normal.z;								
				}
			} // action
		}
		else /// OBJECT SPACE
		{
			if (mAction == MUVMI_CMD_ACTION_POSITION)
			{
				for (int i=0;i<(numUV);i=i+1)
				{
					UV[0] = mUV[2*i];
					UV[1] = mUV[2*i+1];

					cerr <<"\n-----\nUVn "<<UV[0]<<" / "<<UV[1];;
					
					meshFn.getPointAtUV(0,point, UV, MSpace::kObject, &uvSet);
					
					cerr << "\npoint " << point;
					result[3*i] = point.x;
					result[3*i+1] = point.y;
					result[3*i+2] = point.z;								
				}
			}
			else if (mAction == MUVMI_CMD_ACTION_NORMAL)
			{
				MVector normal;
			
				for (int i=0;i<(numUV);i=i+1)
				{
					UV[0] = mUV[2*i];
					UV[1] = mUV[2*i+1];
				
					meshFn.getPointAtUV(0,point, UV, MSpace::kObject, &uvSet);
					meshFn.getClosestNormal(point, normal, MSpace::kObject);

					result[3*i] = normal.x;
					result[3*i+1] = normal.y;
					result[3*i+2] = normal.z;								
				}
			} // action
		}		
		
		//output
		setResult(result);
	}
	else
	{
		USER_ERROR_CHECK(MS::kFailure,"mUVMeshInfo: invalid type, specify a mesh or its transform!");
	}

	return status;
}

} // end namespace
