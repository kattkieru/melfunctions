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

   This command allows you to query information of a shader at specific uv's.

   Parameters:

   -uv|-UV     -  [float[]]   Mandatory: Specify list of UV's to be used in the query\n";
    -u|-U      -  [float[]]   Or: Specify list of U's to be used in the query, don't use in conjunction with -uv flag\n";
    -v|-V      -  [float[]]   Or: Specify list of V's to be used in the query, don't use in conjunction with -uv flag\n";
  -col|-color  -              Default: Return color of shader at specified UV's.\n";
  -colr|-colorRed  -          Or: Return red component of color specified UV's.\n";
  -colg|-colorGreen  -        Or: Return green component of color specified UV's.\n";
  -colg|-colorBlue  -         Or: Return blue component of color specified UV's.\n";      
  -val|-value   -             Or: Return value (mean of color components) of shader at specified UV's.\n";
-trans|-transparency -        Or: Return transparency of shader at specified UV's.\n";    
    -h|-help         -        Displays this help.\n\n";
   shader       - [string]       Manadatory: Name of shading node.\n";

   Returns:

      The result is dependent on the provided flag.

   Examples:
   
   	  m2dShaderInfo -uv {0.5,0,1,0} -col ramp1.outColor;
      
      

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
    help += "//\t  -col|-color              [C]  Mandatory: Return color of shader at specified UVs.\n";
    help += "//\t -colr|-colorRed           [C]  Or: Return red component of color at specified UVs.\n";
    help += "//\t -colg|-colorGreen         [C]  Or: Return green component of color at specified UVs.\n";
    help += "//\t -colb|-colorBlue          [C]  Or: Return blue component of color at specified UVs.\n";
    help += "//\t  -val|-value              [C]  Or: Return value of shader at specified UVs.\n";
    help += "//\t-trans|-transparency       [C]  or: Return transparency of shader at specified UVs.\n";    
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
    

    // or do we want color RED
 	if (argParseIsFlagSet(args,MSI_COLOR_RED_FLAG,MSI_COLOR_RED_FLAG_LONG,index))
	{
		if (mAction == MSI_CMD_ACTION_NONE)
			mAction = MSI_CMD_ACTION_COLOR_RED;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,"m2dShaderInfo: you can only query for either -col|-color (RGB) or -val|-value or -trans|-transparency !");    
		}			
		
		flagNum--;
	}


    // or do we want color GREEN
 	if (argParseIsFlagSet(args,MSI_COLOR_GREEN_FLAG,MSI_COLOR_GREEN_FLAG_LONG,index))
	{
		if (mAction == MSI_CMD_ACTION_NONE)
			mAction = MSI_CMD_ACTION_COLOR_GREEN;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,"m2dShaderInfo: you can only query for either -col|-color (RGB) or -val|-value or -trans|-transparency !");    
		}			
		
		flagNum--;
	}


    // or do we want color BLUE
 	if (argParseIsFlagSet(args,MSI_COLOR_BLUE_FLAG,MSI_COLOR_BLUE_FLAG_LONG,index))
	{
		if (mAction == MSI_CMD_ACTION_NONE)
			mAction = MSI_CMD_ACTION_COLOR_BLUE;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,"m2dShaderInfo: you can only query for either -col|-color (RGB) or -val|-value or -trans|-transparency !");    
		}			
		
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
			USER_ERROR_CHECK(status,"m2dShaderInfo: you can only query for either -col|-color (RGB) or -val|-value or -trans|-transparency !");    
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
			USER_ERROR_CHECK(status,"m2dShaderInfo: you can only query for either -col|-color (RGB) or -val|-value or -trans|-transparency !");    
		}			
		
		flagNum--;
	}


	if(mAction == MSI_CMD_ACTION_NONE)
    {
		status = MS::kFailure;
		USER_ERROR_CHECK(status,"m2dShaderInfo: you must specify what to query for, either -col|-color (RGB), -val|-value or -trans|-transparency !");    
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

        	case (MSI_CMD_ACTION_COLOR_RED):
            {
				for ( int i = 0; i < colors.length(); i++ )
					appendToResult( (double) colors[i].x );
                break;
            }

        	case (MSI_CMD_ACTION_COLOR_GREEN):
            {
				for ( int i = 0; i < colors.length(); i++ )
					appendToResult( (double) colors[i].y );
                break;
            }

        	case (MSI_CMD_ACTION_COLOR_BLUE):
            {
				for ( int i = 0; i < colors.length(); i++ )
					appendToResult( (double) colors[i].z );
                break;
            }
            
           	case (MSI_CMD_ACTION_VALUE):
            {
				for ( int i = 0; i < colors.length(); i++ )
					appendToResult( (double) ((colors[i].x + colors[i].y + colors[i].z)/3.0));
    
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

