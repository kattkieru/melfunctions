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



// Title: mInstancerInfo Command
//

#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnMesh.h>
#include <maya/MRenderUtil.h>
#include <maya/MFnInstancer.h>
#include <maya/MDagPathArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MBoundingBox.h>


#include "../include/mHelperMacros.h"
#include "../include/mInstancerInfoCmd.h"
#include "../include/mHelperFunctions.h"


/*
   Function: mInstancerInfo

   This command allows you to query information of a shader at specific uv's.

   Parameters:

   -mat|-matrix     -   Get matrices for all instances (matrixArray)
    -co|-count      -   Get number of instances (int)
 -bbmin|-boundingBoxMin - Get bounding box minimum corner (vectorArray)
 -bbmax|-boundingBoxMax - Get bounding box maximum corner (vectorArray)
    -h|-help         -        Displays this help
   instancer       - [string]       Manadatory: Name of instancer node

   Returns:

      The result is dependent on the provided flag.

   Examples:
   
      

*/


namespace melfunctions
{


void* mInstancerInfo::creator()
{
	return new mInstancerInfo;
}

//************************************************************************//
// init vars
//void mInstancerInfo::initVariables()
mInstancerInfo::mInstancerInfo()
{
    mAction = MII_CMD_ACTION_NONE;
	mHelpFlagSet = false;    
}

//************************************************************************//

MStatus mInstancerInfo::help() const
{
    MString help( "mInstancerInfo, melfunctions, (c) Carsten Kolve, 2006\n" );
    help += "This allows you to query information of a shader at specific uv's\n\n";
    help += "USAGE: mInstancerInfo\n";
    help += "//\t   -co|-count     [C]  Mandatory: Get number of instances\n";
    help += "//\t  -mat|-matrix         Or: Get transformation matrices of instances\n";
    help += "//\t-bbmin|-boundingBoxMin Or: Get bounding box minimum corner\n";    
    help += "//\t-bbmax|-boundingBoxMax Or: Get bounding box maximum corner\n";        
    help += "//\t   instancer [string]  Manadatory: Name of instancer node.\n";
    MGlobal::displayInfo( help );
    return MS::kSuccess;
}

//************************************************************************//
// now this gets quite elaborate as we can't use the arg parser as it doesn't support arrays


bool mInstancerInfo::argParseIsFlagSet( const MArgList& args, 
                        const char *shortFlag, const char *longFlag, 
                        int &flagIndex)
{
    flagIndex = args.flagIndex( shortFlag, longFlag);
    return ( MArgList::kInvalidArgIndex != flagIndex );
}


MStatus mInstancerInfo::argParseGetObjectStringArg (const MArgList& args,MString& name)
{
	MStatus status = MS::kSuccess;
	int argIndex = args.length() -1;
    
	name = args.asString(argIndex,&status);
	USER_ERROR_CHECK(status,("mInstancerInfo: can't get name of shader, either specify name of shading engine or shading node output attribute!"));

	return status;
}


MStatus mInstancerInfo::argParseGetStringArg( const MArgList& args,  
                                const char *shortFlag, const char *longFlag, 
                                MString &argStr, bool &flagSet)
{
    int flagIndex;
	MStatus status = MS::kSuccess;
    if(argParseIsFlagSet(args,shortFlag, longFlag,flagIndex))
	{
		argStr = args.asString((flagIndex+1),&status);
		USER_ERROR_CHECK(status,(" mInstancerInfo: can't get string argument for argument nr"+(flagIndex+1)));
		flagSet = true;
	}
	else
		flagSet = false;

    return status;
}

//************************************************************************//


MStatus mInstancerInfo::parseArgs( const MArgList& args )
{
 
	MStatus status = MS::kSuccess;
	int flagNum = args.length(); // number of flags that still need to be processed
    int index;
    bool flagSet;

    
    // do we want the help
	if (argParseIsFlagSet(args,MII_HELP_FLAG,MII_HELP_FLAG_LONG,index) || (flagNum ==0))
	{
	     mHelpFlagSet = true;
	     return status;   
	}
    

	// do we want count?
 	if (argParseIsFlagSet(args,MII_COUNT_FLAG,MII_COUNT_FLAG_LONG,index))
	{
		mAction = MII_CMD_ACTION_COUNT;
		flagNum--;
	}
    

    // or do we want bbmin
 	if (argParseIsFlagSet(args,MII_BBMIN_FLAG,MII_BBMIN_FLAG_LONG,index))
	{
		if (mAction == MII_CMD_ACTION_NONE)
			mAction = MII_CMD_ACTION_BBMIN;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,"mInstancerInfo: you can only query for one of the instancer parameter -co|-mat|-bbmin|bbmax!");    
		}			
		
		flagNum--;
	}

    // or do we want bbmax
 	if (argParseIsFlagSet(args,MII_BBMAX_FLAG,MII_BBMAX_FLAG_LONG,index))
	{
		if (mAction == MII_CMD_ACTION_NONE)
			mAction = MII_CMD_ACTION_BBMAX;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,"mInstancerInfo: you can only query for one of the instancer parameter -co|-mat|-bbmin|bbmax!");    
		}			
		
		flagNum--;
	}

    // or do we want matrix
 	if (argParseIsFlagSet(args,MII_MATRIX_FLAG,MII_MATRIX_FLAG_LONG,index))
	{
		if (mAction == MII_CMD_ACTION_NONE)
			mAction = MII_CMD_ACTION_MATRIX;
		else
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,"mInstancerInfo: you can only query for one of the instancer parameter -co|-mat|-bbmin|bbmax!");    
		}			
		
		flagNum--;
	}
    
    // any action selected
 	if (mAction == MII_CMD_ACTION_NONE)   
 	{
		status = MS::kFailure;
		USER_ERROR_CHECK(status,"mInstancerInfo: you must query for one of the instancer parameter -co|-mat|-bbmin|bbmax!");    
	}			

   
	// get the instancer
	status = argParseGetObjectStringArg(args,mInstancerName);
	if (status.error()) 
		return status;
	else 
	{
		flagNum--;
		status = getDagPathFromString(commandString(),mInstancerName,mInstancerDP);// get the dagpath to the object
		ERROR_FAIL(status);		
	}


	if (flagNum != 0)
	{
		USER_ERROR_CHECK(MS::kFailure,"mInstancerInfo: wrong number of arguments! Maybe you used unknown or incompatible flags?");
	}

	return status;
}

//************************************************************************//

MStatus mInstancerInfo::doIt( const MArgList& args )
{
    MStatus status = parseArgs(args);
    if(!status)
    	return status;
    else
    	return redoIt();
}


MStatus mInstancerInfo::getBoundingInfo(MFnInstancer &instancerFn, int count, short bbType, MDoubleArray &bbCorner)
{
	MStatus status;
//    MMatrix instancerWorldMatrix = mInstancerDP.inclusiveMatrix();
	
    bbCorner = MDoubleArray(count*3,0);
    		
	// iterate through all particles
	for(int p = 0; p < count; p++ )
	{
		MMatrix particleMatrix;
		MDagPathArray particlePaths;
		
        int numInstances = instancerFn.instancesForParticle( p, particlePaths, particleMatrix );
		
        if (!numInstances)
        {
        }
        else
        {
			MBoundingBox bbox;
            
            // build bounding box for each instance
	        int numInstances = instancerFn.instancesForParticle( p, particlePaths, particleMatrix );                        
            
            bbox.clear();
            
    		for (int i=0;i<numInstances;i++)	                
            {
         		const MDagPath& instancedPath = particlePaths[i];
	            MFnDagNode dagFn(instancedPath);
                
                MBoundingBox bboxLocal = dagFn.boundingBox();
    			MMatrix instancedPathMatrix = instancedPath.inclusiveMatrix();	
				MMatrix finalMatrixForPath = instancedPathMatrix * particleMatrix;

				bboxLocal.transformUsing(finalMatrixForPath);
            
				bbox.expand( bboxLocal );
                
//				cerr << "\n\t "<< instancedPath.fullPathName();                                                 
//	            cerr << "\n\t "<< i <<"   " << bboxLocal.min() << " - "<< bboxLocal.max();                                                    
            }
			
            MPoint vec;
            if (bbType == MII_BB_MIN)
            	vec = bbox.min();
            else
				vec	= bbox.max();
            
            bbCorner[p*3] = vec.x;
            bbCorner[p*3+1] = vec.y;
            bbCorner[p*3+2] = vec.z;                        
//            cerr << "\np "<< p <<"   " << bbox.min() << " - "<< bbox.max();                        
            
        }

	} // p
 	return MS::kSuccess;
}

//************************************************************************//
// do the actual computation
MStatus mInstancerInfo::redoIt()
{
    MStatus status = MS::kSuccess;

	if (mHelpFlagSet)
		return help();

	// specified node is instancer
	if (mInstancerDP.node().hasFn(MFn::kInstancer))
	{
		// create the fn
		MFnInstancer instancerFn(mInstancerDP, &status);
		USER_ERROR_CHECK(status,"mInstancerInfo: error getting instancerFn!");        
       	
        int count = instancerFn.particleCount();
        
        switch (mAction)
        {
        	case (MII_CMD_ACTION_COUNT):
            {
	            setResult(count);
                break;
            }
    
        	case (MII_CMD_ACTION_MATRIX):
	        {
//    			MMatrix instancerWorldMatrix = mInstancerDP.inclusiveMatrix();

				MDoubleArray result = createEmptyMatArray(count);
                
				// iterate through all particles
				for(int p = 0; p < count; p++ )
				{
	
					MMatrix particleMatrix;
					MDagPathArray particlePaths;
                    
					int numInstances = instancerFn.instancesForParticle( p, particlePaths, particleMatrix );
					
                    if (!numInstances)
                    	setMatToArray(result,p,MMatrix::identity);
                    else
                    {
                    	// get only the first matrix
                    	const MDagPath& instancedPath = particlePaths[0];
						MMatrix instancedPathMatrix = instancedPath.inclusiveMatrix();	
        				MMatrix finalMatrixForPath = instancedPathMatrix * particleMatrix;
                        setMatToArray(result,p,finalMatrixForPath);
                    }
                }//p
                
                setResult(result);
                break;
          	}
            
        	case (MII_CMD_ACTION_BBMIN):
	        {
    			MDoubleArray result;
				status = getBoundingInfo(instancerFn, count, MII_BB_MIN, result);
                setResult(result);
                break;
            }
            
        	case (MII_CMD_ACTION_BBMAX):
	        {
    			MDoubleArray result;
				status = getBoundingInfo(instancerFn, count, MII_BB_MAX, result);
                setResult(result);
                break;
            }

    	} // switch  
        
        
    }
    else
 	{
		USER_ERROR_CHECK(MS::kFailure,"mInstancerInfo: specified object is not an instancer!");
	}

	return status;
}
	

} // end namespace

