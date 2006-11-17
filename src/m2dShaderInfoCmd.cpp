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



// Title: m2dShaderInfo Command
//

#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnMesh.h>
#include <maya/MRenderUtil.h>
#include <maya/M3dView.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatMatrix.h>


#include "../include/mHelperMacros.h"
#include "../include/m2dShaderInfoCmd.h"
#include "../include/mHelperFunctions.h"


/*
   Function: m2dShaderInfo

   This command gives you information about a mesh at its vertices.

   Parameters:

	-vert|-vertex  - 	[float[]]  Optional: Specify list of vertex ids to be used in the query, if none is specified information for ALL vertices will be returned
 	-pos|-position  -    		  Return position vector at specified vertices
	-norm|-normal     -  		  Return normals vector at specified vertices
	 -tan|-tangent     -          Return tangent vector at specified vertices in u direction (only works if mesh has a valid uvSet!)
	 -bin|-binormal     -         Return binormal vector at specified vertices (only works if mesh has a valid uvSet!)
  -ws|-worldSpace    -          Return results in world space (default) (only valid in conjunction with -pos|-norm and -tan flags!)
  -os|-objectSpace    -         Return results in object space (only valid in conjunction with -pos|-norm and -tan flags!) 
  -uv|-UV              -        Return uv information at specified vertices (only the first uv per vertex is returned!)
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
      
      m2dShaderInfo -count pSphere1;
      
      // Result:382//
      
	  m2dShaderInfo -position -worldSpace pSphere1;
      
      // Result:0.148778 -0.987688 -0.0483409 0.126558 -0.987688 -0.0919499 0.0919499 -0.987688 -0.126558 ...
      
      m2dShaderInfo -tangent -worldSpace -uvSet "map1" pSphere1;
      
      // Result:-0.450486 -0.0241735 -0.892456 -0.457224 0.0241743 -0.889023 -0.466618 ...
*/


namespace melfunctions
{


void* m2dShaderInfo::creator()
{
	return new m2dShaderInfo;
}

//************************************************************************//
// init vars
//void m2dShaderInfo::initVariables()
m2dShaderInfo::m2dShaderInfo()
{


    mAction = MSI_CMD_ACTION_NONE;
	mHelpFlagSet = false;    
	mUVSet = false;    
	mVSet = false;    
	mUSet = false;        
}

//************************************************************************//

MStatus m2dShaderInfo::help() const
{
    MString help( "m2dShaderInfo, melfunctions, (c) Carsten Kolve, 2006\n" );
    help += "This allows you to query information of a shader at specific uv's\n\n";
    help += "USAGE: m2dShaderInfo\n";
    help += "//\t   -uv|-UV       [float[]] [C]  Mandatory: Specify list of UV's to be used in the query\n";
    help += "//\t    -u|-U        [float[]] [C]  Or: Specify list of U's to be used in the query, don't use in conjunction with -uv flag\n";
    help += "//\t    -v|-V        [float[]] [C]  Or: Specify list of V's to be used in the query, don't use in conjunction with -uv flag\n";
    help += "//\t  -col|-color              [C]  Default: Return color of shader at specified UV's.\n";
    help += "//\t  -val|-value              [C]  Or: Return value of shader at specified UV's.\n";
    help += "//\t-trans|-transparency       [C]  or: Return transparency of shader at specified UV's.\n";    
    help += "//\t    -h|-help               [C]  Displays this help.\n\n";
    help += "//\t   shader        [string]       Manadatory: Name of shading node.\n";
    MGlobal::displayInfo( help );
    return MS::kSuccess;
}

//************************************************************************//
// now this gets quite elaborate as we can't use the arg parser as it doesn't support arrays


bool m2dShaderInfo::argParseIsFlagSet( const MArgList& args, 
                        const char *shortFlag, const char *longFlag, 
                        int &flagIndex)
{
    flagIndex = args.flagIndex( shortFlag, longFlag);
    return ( MArgList::kInvalidArgIndex != flagIndex );
}


MStatus m2dShaderInfo::argParseGetObjectStringArg (const MArgList& args,MString& name)
{
	MStatus status = MS::kSuccess;
	int argIndex = args.length() -1;
    
	name = args.asString(argIndex,&status);
	USER_ERROR_CHECK(status,("m2dShaderInfo: can't get name of shader, either specify name of shading engine or shading node output attribute!"));

	return status;
}

MStatus m2dShaderInfo::argParseGetDblArrayArg( const MArgList& args,  
                                const char *shortFlag, const char *longFlag, 
                                MDoubleArray &argDblA, bool &flagSet)
{
    int flagIndex;
	MStatus status = MS::kSuccess;
    if(argParseIsFlagSet(args,shortFlag, longFlag,flagIndex))
	{
		status = getDoubleArrayArg(args, (flagIndex+1), argDblA);
		USER_ERROR_CHECK(status,("m2dShaderInfo: can't get doubleArray argument for argument "+(flagIndex+1)));
		flagSet = true;
	}
	else
		flagSet = false;

    return status;
}

MStatus m2dShaderInfo::argParseGetStringArg( const MArgList& args,  
                                const char *shortFlag, const char *longFlag, 
                                MString &argStr, bool &flagSet)
{
    int flagIndex;
	MStatus status = MS::kSuccess;
    if(argParseIsFlagSet(args,shortFlag, longFlag,flagIndex))
	{
		argStr = args.asString((flagIndex+1),&status);
		USER_ERROR_CHECK(status,(" m2dShaderInfo: can't get string argument for argument nr"+(flagIndex+1)));
		flagSet = true;
	}
	else
		flagSet = false;

    return status;
}

//************************************************************************//


MStatus m2dShaderInfo::parseArgs( const MArgList& args )
{
 
	MStatus status = MS::kSuccess;
	int flagNum = args.length(); // number of flags that still need to be processed
    int index;
    bool flagSet;

    
    // do we want the help
	if (argParseIsFlagSet(args,MSI_HELP_FLAG,MSI_HELP_FLAG_LONG,index) || (flagNum ==0))
	{
	     mHelpFlagSet = true;
	     return status;   
	}
    
    // get the uv's
	status = argParseGetDblArrayArg(args,MSI_UV_FLAG,MSI_UV_FLAG_LONG, mUV, flagSet );
	if (status.error()) return status;

	if (flagSet)
	{
		mUVSet = flagSet;
		flagNum-=2;
	}
    
	// u
	status = argParseGetDblArrayArg(args,MSI_U_FLAG,MSI_U_FLAG_LONG, mU, flagSet );
	if (status.error()) return status;

	mUSet = flagSet;
    
	if (mUSet && !mUVSet )
		flagNum -= 2;
    else if (mUSet && mUVSet )
    {
		status = MS::kFailure;
		USER_ERROR_CHECK(status,"m2dShaderInfo: can't use the -uv in conjunction with the -u flag!");    
    }
	    
	// v
	status = argParseGetDblArrayArg(args,MSI_V_FLAG,MSI_V_FLAG_LONG, mV, flagSet );
	if (status.error()) return status;
	mVSet = flagSet;
    
	if (mVSet && !mUVSet )
		flagNum -= 2;
    else if (mVSet && mUVSet )
    {
		status = MS::kFailure;
		USER_ERROR_CHECK(status,"m2dShaderInfo: can't use the -uv in conjunction with the -v flag!");    
    }
	

	// do we want colors?
 	if (argParseIsFlagSet(args,MSI_COLOR_FLAG,MSI_COLOR_FLAG_LONG,index))
	{
		mAction = MSI_CMD_ACTION_COLOR;
		flagNum--;
	}
    
    // or do we want values
 	if (argParseIsFlagSet(args,MSI_VALUE_FLAG,MSI_VALUE_FLAG_LONG,index))
	{
		if (mAction == MSI_CMD_ACTION_NONE)
			mAction = MSI_CMD_ACTION_VALUE;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,"m2dShaderInfo: you can only query for either -col|-color or -val|-value or -trans|-transparency !");    
		}			
		
		flagNum--;
	}

    // or do we want transparencies
 	if (argParseIsFlagSet(args,MSI_TRANSPARENCY_FLAG,MSI_TRANSPARENCY_FLAG_LONG,index))
	{
		if (mAction == MSI_CMD_ACTION_NONE)
			mAction = MSI_CMD_ACTION_TRANSPARENCY;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,"m2dShaderInfo: you can only query for either -col|-color or -val|-value or -trans|-transparency !");    
		}			
		
		flagNum--;
	}


	if(mAction == MSI_CMD_ACTION_NONE)
    {
		status = MS::kFailure;
		USER_ERROR_CHECK(status,"m2dShaderInfo: you must specify what to query for, either -col|-color, -val|-value or -trans|-transparency !");    
    }
    
	// get the shader
	status = argParseGetObjectStringArg(args,m2dShaderName);
	if (status.error()) 
		return status;
	else 
	{
		flagNum--;
	}


	if (flagNum != 0)
	{
		USER_ERROR_CHECK(MS::kFailure,"m2dShaderInfo: wrong number of arguments! Maybe you used unknown or incompatible flags?");
	}

	return status;
}

//************************************************************************//

MStatus m2dShaderInfo::doIt( const MArgList& args )
{
    MStatus status = parseArgs(args);
    if(!status)
    	return status;
    else
    	return redoIt();
}


//************************************************************************//
// do the actual computation
MStatus m2dShaderInfo::redoIt()
{
    MStatus status = MS::kSuccess;

	if (mHelpFlagSet)
		return help();

	unsigned int count;
	MFloatArray uCoords, vCoords;

	// verify the arguments
	if (mUVSet)
    {
		status = uvIsValid(mUV,count);
        if (status.error()) return status;
        
        uCoords = vCoords = MFloatArray(count);
        
        for(int i=0;i<count;i++)
        {
        	int id =i*ELEMENTS_UV;
			uCoords[i] = mUV[id];
			vCoords[i] = mUV[id+1];               
        }
    }
    else
    {
    	if (mUSet)
        {
        	count = mU.length();
            uCoords = MFloatArray(count);
	        for(int i=0;i<count;i++)
				uCoords[i] = mU[i];            
        }
        else
        	uCoords = MFloatArray(1,0.5);	
        
        if (mVSet)
        {
        	count = mV.length();
			vCoords = MFloatArray(count);            
	        for(int i=0;i<count;i++)
				vCoords[i] = mV[i];            
        }
        else
			vCoords = MFloatArray(1,0.5);	        
        
		unsigned int incU,incV;

		status = twoArgCountsValid(uCoords.length(),vCoords.length(),incU,incV,count);
		if (status.error()) return status;
        
        if (incU==0)
        	uCoords = MFloatArray(count,uCoords[0]);
        
        if (incV==0)
        	vCoords = MFloatArray(count,vCoords[0]);
    }

	// now that we have the uv coordinates, sample the shading engine or shading node
    
	// get current camera

	MDagPath cameraPath;
	M3dView::active3dView().getCamera( cameraPath );
	MMatrix mat = cameraPath.inclusiveMatrix();
	MFloatMatrix cameraMat( mat.matrix );


	MFloatVectorArray colors, transps;
    
    if ( MS::kSuccess == MRenderUtil::sampleShadingNetwork( 
			m2dShaderName, 
			int(count),
			false, // shadow
			false, // reuse
			cameraMat, //cameraMat,
			NULL, //points
			&uCoords,
			&vCoords,
			NULL, //normals
			NULL, //refPoints
			NULL, //tanUs
			NULL, //tanVs
			NULL, //filterSizes
			colors,
			transps ) )
	{
		switch (mAction)
        {
        	case (MSI_CMD_ACTION_COLOR):
            {
				for ( int i = 0; i < colors.length(); i++ )
				{
					appendToResult( (double) colors[i].x );
					appendToResult( (double) colors[i].y );
					appendToResult( (double) colors[i].z );
				}
                break;
            }
            
           	case (MSI_CMD_ACTION_VALUE):
            {
				for ( int i = 0; i < colors.length(); i++ )
					appendToResult( (double) colors[i].length());
    
                break;
            }

           	case (MSI_CMD_ACTION_TRANSPARENCY):
            {
            	for ( int i = 0; i < transps.length(); i++ )
				{
					appendToResult( (double) transps[i].x );
					appendToResult( (double) transps[i].y );
					appendToResult( (double) transps[i].z );
				}
                break;
	        }
        }
	
	}
	else
	{
		displayError(MString("m2dShaderName: ")+ m2dShaderName + MString(" is not a shading engine!  Specify node.attr or shading group node." ) );
        return MS::kFailure;
	}
    	

	return status;
}


	

} // end namespace

