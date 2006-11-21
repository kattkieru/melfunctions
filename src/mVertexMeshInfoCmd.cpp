/* COPYRIGHT --
 *
 * This file is part of melfunctions, a collection of mel commands to for Autodesk Maya.
 * melfunctions is (c) 2006 Carsten Kolve <carsten@kolve.com>
 * This contribution to melfunctions is (c) Rising Sun Pictures PTY Ltd, www.rsp.com.au
 * and distributed under the terms of the GNU GPL V2.
 * See the ./License-GPL.txt file in the source tree root for more information.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


// Title: mVertexMeshInfo Command
//

#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnMesh.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>


#include "../include/mHelperMacros.h"
#include "../include/mVertexMeshInfoCmd.h"
#include "../include/mHelperFunctions.h"


/*
   Function: mVertexMeshInfo

   This command gives you information about a mesh at its vertices.

   Parameters:

	-vert|-vertex  - 	[float[]]  Optional: Specify list of vertex ids to be used in the query, if none is specified information for ALL vertices will be returned
 	-pos|-position  -    		  Return position vector at specified vertices
	-norm|-normal     -  		  Return normals vector at specified vertices
	 -tan|-tangent     -          Return tangent vector at specified vertices in u direction (only works if mesh has a valid uvSet!)
	 -bin|-binormal     -         Return binormal vector at specified vertices (only works if mesh has a valid uvSet!)
  -ws|-worldSpace    -          Return results in world space (default) (only valid in conjunction with -pos|-norm and -tan flags!)
  -os|-objectSpace    -         Return results in object space (only valid in conjunction with -pos|-norm and -tan flags!) 
  -uv|-UV              -        Return uv information at specified vertices (only the first uv per vertex is returned!)(Maya 8 only)
 -uvs|-UVSet       - [string]   Specify a string with the UVSet to use (by default the first one is used, flag only valid in conjunction with -uv|-tan|-bin flag!)
 -col|-color       -            Return color information at specified vertices (only the first color per vertex one is returned!)
-cols|-colorSet    - [string]   Specify a string with the colorSet to use (by default the first one is used, only valid in conjunction with -col flag!)
  -co|-count       -            Return number of vertices.
   -h|-help        -            Displays the help.
  mesh             - [string]       Manadatory: Name of mesh.


   Returns:

      The result is dependent on the provided flags

   Examples:

      polySphere;
      
      mVertexMeshInfo -count pSphere1;
      
      // Result:382//
      
	  mVertexMeshInfo -position -worldSpace pSphere1;
      
      // Result:0.148778 -0.987688 -0.0483409 0.126558 -0.987688 -0.0919499 0.0919499 -0.987688 -0.126558 ...
      
      mVertexMeshInfo -tangent -worldSpace -uvSet "map1" pSphere1;
      
      // Result:-0.450486 -0.0241735 -0.892456 -0.457224 0.0241743 -0.889023 -0.466618 ...
*/


namespace melfunctions
{


void* mVertexMeshInfo::creator()
{
	return new mVertexMeshInfo;
}

//************************************************************************//
// init vars
//void mVertexMeshInfo::initVariables()
mVertexMeshInfo::mVertexMeshInfo()
{

	mAction = MVMI_CMD_ACTION_NONE;
	mSpace = MVMI_CMD_SPACE_WORLD;
	mHelpFlagSet = false;    
	mUVSetSet = false;    
	mColorSetSet = false;    	
	mVertIdSet = false;

}

//************************************************************************//

MStatus mVertexMeshInfo::help() const
{
    MString help( "mVertexMeshInfo, melfunctions, (c) Carsten Kolve, 2006\n" );
    help += "This allows you to query information at vertices of apped mesh\n\n";
    help += "USAGE: mVertexMeshInfo\n";
    help += "//\t -vert|-vertex       [float[]] [C]  Optional: Specify list of vertex ids to be used in the query,\n";
	help += "//\t                                    if none is specified information for all vertices will be returned.\n";
    help += "//\t  -pos|-position               [C]  Return position vector at specified vertices\n";
    help += "//\t -norm|-normal                 [C]  Return normals vector at specified vertices\n";;
    help += "//\t  -tan|-tangent                [C]  Return tangent vector at specified vertices in u direction\n";;
    help += "//\t  -bin|-binormal               [C]  Return binormal vector at specified vertices\n";;	
    help += "//\t   -ws|-worldSpace             [C]  Only valid in conjunction with -pos|-norm and -tan flags, return results in world space (default)\n";		
    help += "//\t   -os|-objectSpace            [C]  Only valid in conjunction with -pos|-norm and -tan flags, return results in object space\n\n"; 	   
    help += "//\t   -uv|-UV                     [C]  Return uv information at specified vertices (only the first one is returned!).\n";
    help += "//\t  -uvs|-UVSet        [string]  [C]  Only valid in conjunction with -uv|-tan flag, specify a string with the UVSet to use (by default the first one is used)\n\n";    
    help += "//\t  -col|-color                  [C]  Return color information at specified vertices (only the first one is returned!).\n";
    help += "//\t -cols|-colorSet     [string]  [C]  Only valid in conjunction with -col flag, specify a string with the colorSet to use (by default the first one is used)\n\n";    
    help += "//\t   -co|-count                  [C]  Return number of vertices.\n";    
    help += "//\t    -h|-help                   [C]  Displays this help.\n\n";
    help += "//\t   mesh              [string]       Manadatory: Name of mesh.\n";

    MGlobal::displayInfo( help );
    return MS::kSuccess;
}

//************************************************************************//
// now this gets quite elaborate as we can't use the arg parser as it doesn't support arrays


bool mVertexMeshInfo::argParseIsFlagSet( const MArgList& args, 
                        const char *shortFlag, const char *longFlag, 
                        int &flagIndex)
{
    flagIndex = args.flagIndex( shortFlag, longFlag);
    return ( MArgList::kInvalidArgIndex != flagIndex );
}


MStatus mVertexMeshInfo::argParseGetObjectStringArg (const MArgList& args,MString& name)
{
	MStatus status = MS::kSuccess;
	int argIndex = args.length() -1;
    
	name = args.asString(argIndex,&status);
	USER_ERROR_CHECK(status,("mVertexMeshInfo: can't get name of mesh"));

	return status;
}

MStatus mVertexMeshInfo::argParseGetDblArrayArg( const MArgList& args,  
                                const char *shortFlag, const char *longFlag, 
                                MDoubleArray &argDblA, bool &flagSet)
{
    int flagIndex;
	MStatus status = MS::kSuccess;
    if(argParseIsFlagSet(args,shortFlag, longFlag,flagIndex))
	{
		status = getDoubleArrayArg(args, (flagIndex+1), argDblA);
		USER_ERROR_CHECK(status,("mVertexMeshInfo: can't get doubleArray argument for argument "+(flagIndex+1)));
		flagSet = true;
	}
	else
		flagSet = false;

    return status;
}

MStatus mVertexMeshInfo::argParseGetStringArg( const MArgList& args,  
                                const char *shortFlag, const char *longFlag, 
                                MString &argStr, bool &flagSet)
{
    int flagIndex;
	MStatus status = MS::kSuccess;
    if(argParseIsFlagSet(args,shortFlag, longFlag,flagIndex))
	{
		argStr = args.asString((flagIndex+1),&status);
		USER_ERROR_CHECK(status,(" mVertexMeshInfo: can't get string argument for argument nr"+(flagIndex+1)));
		flagSet = true;
	}
	else
		flagSet = false;

    return status;
}

//************************************************************************//


MStatus mVertexMeshInfo::parseArgs( const MArgList& args )
{
 
	MStatus status = MS::kSuccess;
	int flagNum = args.length(); // number of flags that still need to be processed
    int index;
    bool flagSet;

    
    // do we want the help
	if (argParseIsFlagSet(args,MVMI_HELP_FLAG,MVMI_HELP_FLAG_LONG,index) || (flagNum ==0))
	{
	     mHelpFlagSet = true;
	     return status;   
	}
    
    // get the vertices
	status = argParseGetDblArrayArg(args,MVMI_VERTEX_FLAG,MVMI_VERTEX_FLAG_LONG, mVertId, flagSet );
	if (status.error()) return status;

	if (flagSet)
	{
		mVertIdSet = flagSet;
		flagNum-=2;
	}


	// do we want uv's?
 	if (argParseIsFlagSet(args,MVMI_UV_FLAG,MVMI_UV_FLAG_LONG,index))
	{
		mAction = MVMI_CMD_ACTION_UV;
		flagNum--;
		
	}

	// do we want colors?
 	if (argParseIsFlagSet(args,MVMI_COLOR_FLAG,MVMI_COLOR_FLAG_LONG,index))
	{
		mAction = MVMI_CMD_ACTION_COLOR;
		flagNum--;
		
		// check if we want a specific COLOR set
		status = argParseGetStringArg(args,MVMI_COLOR_SET_FLAG,MVMI_COLOR_SET_FLAG_LONG, mColorSet, flagSet );
		if (status.error()) return status;
		if (flagSet)
		{
			mColorSetSet = true;
			flagNum-=2;
		}        
	}
	
	
	MString singleFlagError = "mVertexMeshInfo: you can only query for one vertex parameter, -pos|-norm|-col|-tan|-bin or -uv!";
	
	// do we want positions?
	if (argParseIsFlagSet(args,MVMI_POSITION_FLAG,MVMI_POSITION_FLAG_LONG,index))
	{
		if (mAction == MVMI_CMD_ACTION_NONE)
			mAction = MVMI_CMD_ACTION_POSITION;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,singleFlagError);    
		}			
		
		flagNum--;
	}

	// do we want normals?
	if (argParseIsFlagSet(args,MVMI_NORMAL_FLAG,MVMI_NORMAL_FLAG_LONG,index))
	{
		if (mAction == MVMI_CMD_ACTION_NONE)
			mAction = MVMI_CMD_ACTION_NORMAL;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,singleFlagError);    
		}			
		
		flagNum--;
	}

	// do we want binormals?
	if (argParseIsFlagSet(args,MVMI_BINORMAL_FLAG,MVMI_BINORMAL_FLAG_LONG,index))
	{
		if (mAction == MVMI_CMD_ACTION_NONE)
			mAction = MVMI_CMD_ACTION_BINORMAL;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,singleFlagError);    
		}			
		
		flagNum--;
	}

	// do we want tangents?
	if (argParseIsFlagSet(args,MVMI_TANGENT_FLAG,MVMI_TANGENT_FLAG_LONG,index))
	{
		if (mAction == MVMI_CMD_ACTION_NONE)
			mAction = MVMI_CMD_ACTION_TANGENT;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,singleFlagError);    
		}			
		
		flagNum--;
	}


	// TODO do we want count?
	if (argParseIsFlagSet(args,MVMI_COUNT_FLAG,MVMI_COUNT_FLAG_LONG,index))
	{
		if (mAction == MVMI_CMD_ACTION_NONE)
			mAction = MVMI_CMD_ACTION_COUNT;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,singleFlagError);    
		}			
		
		flagNum--;
	} 


	// no action selected?	
	if (mAction == MVMI_CMD_ACTION_NONE)
	{	
		status = MS::kFailure;
		USER_ERROR_CHECK(status,singleFlagError);    
	}


	// check if we want a specific uv set
	if ((mAction == MVMI_CMD_ACTION_UV) || (mAction == MVMI_CMD_ACTION_TANGENT)|| (mAction == MVMI_CMD_ACTION_BINORMAL))
	{
		status = argParseGetStringArg(args,MVMI_UV_SET_FLAG,MVMI_UV_SET_FLAG_LONG, mUVSet, flagSet );
		if (status.error()) return status;
		if (flagSet)
		{
			mUVSetSet = true;
			flagNum-=2;
		}        
	}


	// if we want normals or positions, in what space?
	if ((mAction == MVMI_CMD_ACTION_NORMAL) || (mAction == MVMI_CMD_ACTION_POSITION) 
		|| (mAction == MVMI_CMD_ACTION_TANGENT)|| (mAction == MVMI_CMD_ACTION_BINORMAL))
	{
		// get the space
		if (argParseIsFlagSet(args,MVMI_OBJECT_SPACE_FLAG ,MVMI_OBJECT_SPACE_FLAG_LONG ,index))
		{
			mSpace = MVMI_CMD_SPACE_OBJECT;
			flagNum--;
		} 
	
		if (argParseIsFlagSet(args,MVMI_WORLD_SPACE_FLAG,MVMI_WORLD_SPACE_FLAG_LONG,index)) 
		{
			if (mSpace == MVMI_CMD_SPACE_OBJECT)
			{
				status = MS::kFailure;
				USER_ERROR_CHECK(status,("mVertexMeshInfo: you can only specify one space parameter, either -ws|-worldSpace or -os|-objectSpace!"));    
			}
			else
			{
				mSpace = MVMI_CMD_SPACE_WORLD;       
				flagNum--;
			}
		}
	}


	// get the mesh
	status = argParseGetObjectStringArg(args,mMeshName);
	if (status.error()) 
		return status;
	else 
	{
		status = getDagPathFromString(commandString(),mMeshName,mMeshDP);
		flagNum--;
	}


	if (flagNum != 0)
	{
		USER_ERROR_CHECK(MS::kFailure,"mVertexMeshInfo: wrong number of arguments! Maybe you used unknown or incompatible flags?");
	}

	return status;
}

//************************************************************************//

MStatus mVertexMeshInfo::doIt( const MArgList& args )
{
    MStatus status = parseArgs(args);
    if(!status)
    	return status;
    else
    	return redoIt();
}

//************************************************************************//
// get the vertex colors
MStatus mVertexMeshInfo::doVertexColor(MFnMesh &meshFn)
{
	MStatus status = MS::kSuccess;

	// check if the mesh has any Color sets
	if (!meshFn.numColorSets())
	{
		USER_ERROR_CHECK(MS::kFailure,"mVertexMeshInfo: specified mesh does not have any color information!");     
	}

	// get the Color set
	MString colorSet = NULL;

	if (mColorSetSet)
	{
		MStringArray colorSets;
		meshFn.getColorSetNames(colorSets);

		// is the specified set part of the Color sets
		bool found =false;
		for(int i=0;i<colorSets.length();i++)
		{
			if (colorSets[i] == mColorSet)
			{
				found =true;
				colorSet = mColorSet;
			}
		}
		
		if (!found) 
		{
			USER_ERROR_CHECK(MS::kFailure,"mVertexMeshInfo: specified colorSet could not be found on mesh!");
		}
	}
	else
	{
		MStringArray colorSets;
		meshFn.getColorSetNames(colorSets);
		mColorSet = colorSets[0];
	}

	// get all positions
	MColorArray colors;
	status = meshFn.getColors(colors, &mColorSet);
	MDoubleArray result;
	
	MColor color;
	
	if (mVertIdSet)
	{
		result = MDoubleArray(ELEMENTS_VEC*mVertIdNum);
		MPoint point;
		for (int i=0; i<mVertIdNum; i++)
		{
			int id = i*ELEMENTS_VEC;
			color = colors[(int)mVertId[i]];
			
			result[id] 	 = color.r;
			result[id+1] = color.g;
			result[id+2] = color.b;			
		}
		
	}
	else
	{
		result = MDoubleArray(ELEMENTS_VEC*colors.length());
		for (int i=0; i < colors.length(); i++)
		{
			int id = i*ELEMENTS_VEC;
			
			result[id] 	 = colors[i].r;
			result[id+1] = colors[i].g;
			result[id+2] = colors[i].b;	
		}
	}
	
	setResult(result);
    return status;    
}

//************************************************************************//
// get the vertex uv's

MStatus  mVertexMeshInfo::getUVSet(MFnMesh &meshFn)
{

    // check if the mesh has any uv sets
	if (!meshFn.numUVSets())
	{
		USER_ERROR_CHECK(MS::kFailure,"mVertexMeshInfo: specified mesh does not have any uv information!");     
	}

	// get the uv set
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
			USER_ERROR_CHECK(MS::kFailure,"mVertexMeshInfo: specified uvSet could not be found on mesh!");
		}
	}
	else
	{
		MStringArray uvSets;
		meshFn.getUVSetNames(uvSets);
		mUVSet = uvSets[0];
	}
	

	return MS::kSuccess;

}

#if MAYA_API_VERSION >= 800	   
MStatus mVertexMeshInfo::doVertexUV(MFnMesh &meshFn)
{
	MStatus status = getUVSet(meshFn);
	if(status.error()) return status;

	// get all positions
	MPointArray vertPos;
	status = meshFn.getPoints(vertPos, MSpace::kObject);
	MDoubleArray result;
	MPoint point;
	
	float2 uv;
	
	if (mVertIdSet)
	{
		result = MDoubleArray(ELEMENTS_UV*mVertIdNum);
		MPoint point;
		for (int i=0;i<mVertIdNum;i++)
		{
			int id = i*ELEMENTS_UV;
			point = vertPos[(int)mVertId[i]];
			meshFn.getUVAtPoint(point,uv,MSpace::kObject,&mUVSet);
			
			result[id] 	 = uv[0];
			result[id+1] = uv[1];
		}
		
	}
	else
	{
		result = MDoubleArray(ELEMENTS_UV*vertPos.length());
		for (int i=0;i<vertPos.length();i++)
		{
			int id = i*ELEMENTS_UV;
			point = vertPos[i];			
			meshFn.getUVAtPoint(point,uv,MSpace::kObject,&mUVSet);
			
			result[id] = uv[0];
			result[id+1] = uv[1];
		}
	}
	
	setResult(result);
    return status;    
}
#endif // maya8
//************************************************************************//
MStatus mVertexMeshInfo::doVertexPosition(MFnMesh &meshFn)
{
	MStatus status = MS::kSuccess;
	
	int numVert = meshFn.numVertices();
	
	MSpace::Space space = MSpace::kObject;
	
	// get points in the correct space
	MPointArray vertPos;
	if (mSpace == MVMI_CMD_SPACE_WORLD)
		space = MSpace::kWorld;
        
	status = meshFn.getPoints(vertPos, space);
	
	MDoubleArray result;
	MPoint point;	
	
	if (mVertIdSet)
	{
		result = MDoubleArray(ELEMENTS_VEC*mVertIdNum);
		MPoint point;
		for (int i=0;i<mVertIdNum;i++)
		{
			int id = i*ELEMENTS_VEC;
			point = vertPos[(int)mVertId[i]];
			result[id] 	 = point.x;
			result[id+1] = point.y;
			result[id+2] = point.z;						
		}
	}
	else
	{
		result = MDoubleArray(ELEMENTS_VEC*vertPos.length());
		for (int i=0;i<vertPos.length();i++)
		{
			int id = i*ELEMENTS_VEC;
			point = vertPos[i];			
			result[id] = point.x;
			result[id+1] = point.y;
			result[id+2] = point.z;						
		}
	}
	
	setResult(result);
    return status;    
}
//************************************************************************//
MStatus mVertexMeshInfo::doVertexNormal(MFnMesh &meshFn)
{
	MStatus status = MS::kSuccess;
	
	int numVert = meshFn.numVertices();
	
	MSpace::Space space = MSpace::kObject;
	if (mSpace == MVMI_CMD_SPACE_WORLD)
		space = MSpace::kWorld;
               
	// get points in the correct space
	MPointArray vertPos;

	status = meshFn.getPoints(vertPos, space);
	
	MDoubleArray result;
	MVector normal;	
	
	if (mVertIdSet)
	{
		result = MDoubleArray(ELEMENTS_VEC*mVertIdNum);
		
		for (int i=0;i<mVertIdNum;i++)
		{
			int id = i*ELEMENTS_VEC;
			meshFn.getVertexNormal(((int)mVertId[i]),normal,space);
			
			result[id]   = normal.x;
			result[id+1] = normal.y;
			result[id+2] = normal.z;						
		}

	}
	else // get all normals
	{
		result = MDoubleArray(ELEMENTS_VEC*vertPos.length());

		for (int i=0;i<numVert;i++)
		{
			int id = i*ELEMENTS_VEC;
			meshFn.getVertexNormal(i,normal,space);
			
			result[id]   = normal.x;
			result[id+1] = normal.y;
			result[id+2] = normal.z;						
		}
	}
	
	setResult(result);
        return status;
}

//************************************************************************//
MStatus mVertexMeshInfo::doVertexTangent(MFnMesh &meshFn)
{

	MStatus status = getUVSet(meshFn);
	if(status.error()) return status;

	int numVert = meshFn.numVertices();
	
	// get tangents in the correct space
    
	MSpace::Space space = MSpace::kObject;
	if (mSpace == MVMI_CMD_SPACE_WORLD)
		space = MSpace::kWorld;

    
	MFloatVectorArray tangents;
	status = meshFn.getTangents(tangents, space,&mUVSet);
	
    USER_ERROR_CHECK(status,"mVertexMeshInfo: error getting tangents, has the specified mesh a valid uv set?");
		
    
	MDoubleArray result;
	MFloatVector tangent;	
	
	if (mVertIdSet)
	{
		result = MDoubleArray(ELEMENTS_VEC*mVertIdNum);

		for (int i=0;i<mVertIdNum;i++)
		{
			int id = i*ELEMENTS_VEC;
			tangent = tangents[(int)mVertId[i]];
			result[id] 	 = tangent.x;
			result[id+1] = tangent.y;
			result[id+2] = tangent.z;						
		}
	}
	else
	{
		result = MDoubleArray(ELEMENTS_VEC*numVert);
		for (int i=0;i<numVert;i++)
		{
			int id = i*ELEMENTS_VEC;
			tangent = tangents[i];			
			result[id] = tangent.x;
			result[id+1] = tangent.y;
			result[id+2] = tangent.z;						
		}
	}

	setResult(result);
    return status;    
}
//************************************************************************//
MStatus mVertexMeshInfo::doVertexBinormal(MFnMesh &meshFn)
{
	MStatus status = getUVSet(meshFn);
	if(status.error()) return status;
	
	int numVert = meshFn.numVertices();
	
	// get binormals in the correct space
	MFloatVectorArray binormals;
    
	MSpace::Space space = MSpace::kObject;
	if (mSpace == MVMI_CMD_SPACE_WORLD)
		space = MSpace::kWorld;

	status = meshFn.getBinormals(binormals, space,&mUVSet);

    USER_ERROR_CHECK(status,"mVertexMeshInfo: error getting binormals, has the specified mesh a valid uv set?");
    	
	MDoubleArray result;
	MFloatVector binormal;	
	
	if (mVertIdSet)
	{
		result = MDoubleArray(ELEMENTS_VEC*mVertIdNum);

		for (int i=0;i<mVertIdNum;i++)
		{
			int id = i*ELEMENTS_VEC;
			binormal = binormals[(int)mVertId[i]];
			result[id] 	 = binormal.x;
			result[id+1] = binormal.y;
			result[id+2] = binormal.z;						
		}
	}
	else
	{
		result = MDoubleArray(ELEMENTS_VEC*numVert);
		for (int i=0;i<numVert;i++)
		{
			int id = i*ELEMENTS_VEC;
			binormal = binormals[i];			
			result[id] = binormal.x;
			result[id+1] = binormal.y;
			result[id+2] = binormal.z;						
		}
	}
		
	setResult(result);
    return status;
}

//************************************************************************//
MStatus mVertexMeshInfo::doVertexCount(MFnMesh &meshFn)
{
	MStatus status = MS::kSuccess;
	int numVert = meshFn.numVertices();
	setResult(numVert);
    return status;    
}
//************************************************************************//

//************************************************************************//
// do the actual computation
MStatus mVertexMeshInfo::redoIt()
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
    cerr <<"\nvert "<<mVertId;                
    cerr <<"\nvertSet "<<mVertIdSet;                
    cerr <<"\nMesh "<<mMeshDP.fullPathName();                    
*/

	// verify the arguments
	//

	
	// specified node is mesh,, or a transform of a mesh
	if ((mMeshDP.node().hasFn(MFn::kMesh) || (mMeshDP.node().hasFn(MFn::kTransform) && mMeshDP.hasFn(MFn::kMesh))))
	{
		// extend the dp if we're on the transform
		if ((mMeshDP.node().hasFn(MFn::kTransform) && mMeshDP.hasFn(MFn::kMesh)))
			mMeshDP.extendToShape();
		
		// create the fn
		MFnMesh meshFn(mMeshDP);
		
		// if the vert flag is set, check if all the vertex indices are actually valid
		if (mVertIdSet)
		{
			mVertIdNum = mVertId.length();			
			int max = meshFn.numVertices() -1;
			for(int i=0;i<mVertIdNum;i++)
			{
				int id = (int) mVertId[i];
				if ((id > max) || (id<0))
				{
					MString e = "mVertexMeshInfo: vertexId[";
					e = e+ i+"] = "+ id + " out of range of valid ids [0-"+max+"]!";
					USER_ERROR_CHECK(MS::kInvalidParameter,e);
				}
				else
					mVertId[i] = id;
			}
		}
		
		switch (mAction)
		{
        	#if MAYA_API_VERSION >= 800	  
			case MVMI_CMD_ACTION_UV:		return doVertexUV(meshFn);
            #endif
			case MVMI_CMD_ACTION_POSITION:	return doVertexPosition(meshFn);
			case MVMI_CMD_ACTION_NORMAL:	return doVertexNormal(meshFn);
			case MVMI_CMD_ACTION_TANGENT:	return doVertexTangent(meshFn);
			case MVMI_CMD_ACTION_BINORMAL:	return doVertexBinormal(meshFn); 			
			case MVMI_CMD_ACTION_COLOR:		return doVertexColor(meshFn);
			case MVMI_CMD_ACTION_COUNT:		return doVertexCount(meshFn);            
		}
	}
	else
	{
		USER_ERROR_CHECK(MS::kFailure,"mVertexMeshInfo: invalid type, specify a mesh or its transform!");
	}

	return status;
}


	

} // end namespace

