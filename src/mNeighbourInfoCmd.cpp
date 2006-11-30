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

// Title: mNeighbourInfo Command


#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MStringArray.h>

#include <values.h>

#include "../include/mHelperMacros.h"
#include "../include/mNeighbourInfoCmd.h"
#include "../include/mHelperFunctions.h"



/*
   Function: mNeighbourInfo

This Command allows you to create, edit and query mNeighbourInfo objects which allow you to efficiently
find for a set of LOOKUP POINTS a set of NEIGHBOURing POINTS in a LOOKUP RANGE around the LOOKUP POINTS.


Parameters:

-np|-neighbourPoint     - [CE]    Specify a vector array of points used as the neighbouring points (doubleArray).
-lp|-lookupPoint        - [Q]     Mandator upon Query: Specify a vector array of points which will be the lookup points (doubleArray!).
                                (must have same 1 or same amount of values than there are lookup ranges) (double value or doubleArray)     
-lr|-lookupRadius        - [Q]    Mandator upon Query: Specify a double array of radii around each lookup points used as the lookup range 
                                (must have same 1 or same amount of values than there are lookup points) (double value or doubleArray) 
-ld|-lookupDirection     - [Q]    Optional upon Query, only valid in conjunction with -la flag: Specify a vector array of directions for each lookup points to look in
                                (must have same 1 or same amount of values than there are lookup points) (double value or doubleArray) 
-la|-lookupAngle        - [Q]    Only valid in conjunction with -ld flag: Specify an angle around the lookupDirection (build a FieldOfView)
                                (must have same 1 or same amount of values than there are lookup points) (double value or doubleArray) 

 -nir|-nearestInRange    - [Q]     Returns for each lookup point the index of the nearest neighbour point in the lookup range
                                  or -1 if none is found. (doubleArray)    
 -fir|-furthestInRange    -[Q]     Returns for each lookup point the index of the furthest neighbour point in the lookup range
                                  or -1 if none is found. (doubleArray)   
 -rir|-randomInRange     - [Q]     Returns for each lookup point the index of a random neighbour point in the lookup range
                                  or -1 if none is found. (doubleArray)       
 -air|-allInRange        - [Q]     Specify an index  of a lookup point (int) and it will return an array of indices of neighbour points
                                  in the lookup range or -1 if none is found. (doubleArray)  
 -xcs|-excludeSame        - [Q]   if the lookupPoint is part of the result, exclude it (useful if the lookupPoints and neighbourPoints are the same)
 -cir|-countInRange      - [Q]     Returns for each lookup point the number of neighbour point in the lookup range (doubleArray)
 -pos|-position         -  [Q]     Use this option when querying to return the positions of the neighbours instead of their indices
   -l|-list             -  [C]     Returns all mNeighbourInfo object names (as a stringArray)
   -d|-delete          -   [E]     Delete the specified mNeighbourInfo object.    
  -da|-deleteAll        -  [C]     Delete all mNeighbourInfo objects.    
   -h|-help            -   [C]     Displays this help.


 - [C]reate [E]dit [Q]uery flags

   Returns:

      The result is dependent on the provided flags

   Examples:
   
   // create a particle system, then
   
   float $points[] = `getAttr particle1.worldPosition`;
   
   string $myNI = `mNeighbourInfo -neighbourPoint $points`
   
   // Result:mNeighbourInfoObject0//
   
   float $lookupPoints[] = {0.0, 1, 0, 2, 0, 0};
   
   float $lookupRadius[] = {2.0};
   
   mNeighbourInfo -lookupPoint $lookupPoints -lookupRange $lookupRadius -nearestInRange $myNI;
   
   // Example Result: 5 9 (id's of respective positions in $points)//
   
   mNeighbourInfo -lookupPoint $lookupPoints -lookupRange $lookupRadius -nearestInRange -position $myNI;
   
   // Example Result: 0.5 1 0 1 0 0 (actual positions of id 5 and 9 in $points)//

   // optaining the velocities of the nearest particles to our lookupPoints points
   //
   
   float $velocities[] = `getAttr particle1.velocity`;
   
   float $nearestId[] = `mNeighbourInfo -lookupPoint $lookupPoints -lookupRange $lookupRadius -nearestInRange $myNI`;
   
   mVecGet($velocities, $nearestId);
   
   // Example Result: 1 0 0 1 1 0 (velocities of id 5 and 9 in respective to the $points array)//
   
   // it's a good idea to delete the neighbour info object after using it, so the memory is freed
   
   mNeighbourInfo -delete $myNI;

*/


namespace melfunctions
{

mNeighbourInfoMapType mNeighbourInfo::mNIMap;
int mNeighbourInfo::mNeighbourInfoIndex =0;

void* mNeighbourInfo::creator()
{
	return new mNeighbourInfo;
}

//************************************************************************//
// init vars
//void mNeighbourInfo::initVariables()
mNeighbourInfo::mNeighbourInfo()
{
	
	mCmdAction = CMD_ACTION_CREATE;
    mQueryAction = CMD_QUERY_NONE;
	mEditAction = CMD_EDIT_NONE;
    mCreateAction = CMD_CREATE_CREATE;

    mQueryPosition = false;
    mExcludeSame = false;    
	mHelpFlagSet = false;    
    mLookupRadiusFlagSet = false;            
    mLookupPointFlagSet = false;            
    mNeighbourPointFlagSet = false;            

    mLookupDirectionFlagSet = false;            
    mLookupAngleFlagSet = false;                
    mLookupFOV = false;                    
}

//************************************************************************//

void mNeighbourInfo::help() const
{
    MString help( "mNeighbourInfo, melfunctions, (c) Carsten Kolve, 2006\n" );
    help += "This Command allows you to create, edit and query mNeighbourInfo objects which allow you to efficiently\n";
    help += "find for a set of LOOKUP POINTS a set of NEIGHBOURing POINTS in a LOOKUP RANGE around the LOOKUP POINTS.\n\n";
    help += "USAGE: mNeighbourInfo\n";
    help += "//\t  -np|-neighbourPoint     [CE]    Specify a vector array of points used as the neighbouring points (doubleArray).\n";
    help += "//\t  -lp|-lookupPoint        [Q] 	   Mandator upon Query: Specify a vector array of points which will be the lookup points (doubleArray!).\n";
    help += "//\t                                  (must have same 1 or same amount of values than there are lookup ranges) (double value or doubleArray)\n";     
    help += "//\t  -lr|-lookupRadius       [Q]     Mandator upon Query: Specify a double array of radii around each lookup points used as the lookup range \n";
    help += "//\t                                  (must have same 1 or same amount of values than there are lookup points) (double value or doubleArray)\n"; 
    help += "//\t  -ld|-lookupDirection    [Q]     Optional, must be used with -la flag: Specify a vector array of direction from each lookup point\n";
    help += "//\t                                  (must have same 1 or same amount of values than there are lookup points) (vector value or doubleArray)\n"; 
    help += "//\t  -la|-lookupAngle        [Q]     Optional, must be used with -ld flag: Specify a double array of angles (rad) around the lookupDirection\n";
    help += "//\t                                  (must have same 1 or same amount of values than there are lookup points) (double value or doubleArray)\n"; 

    help += "//\t -nir|-nearestInRange     [Q]     Returns for each lookup point the index of the nearest neighbour point in the lookup range\n";
    help += "//\t                                  or -1 if none is found. (doubleArray)\n";    
    help += "//\t -fir|-furthestInRange    [Q]     Returns for each lookup point the index of the furthest neighbour point in the lookup range\n";
    help += "//\t                                  or -1 if none is found. (doubleArray)\n";   
    help += "//\t -rir|-randomInRange      [Q]     Returns for each lookup point the index of a random neighbour point in the lookup range\n";
    help += "//\t                                  or -1 if none is found. (doubleArray)\n";       
    help += "//\t -air|-allInRange         [Q]     Specify an index  of a lookup point (int) and it will return an array of indices of neighbour points\n";
    help += "//\t                                  in the lookup range or -1 if none is found. (doubleArray)\n";  
	help += "//\t -cir|-countInRange       [Q]     Returns for each lookup point the number of neighbour point in the lookup range (doubleArray)\n";
	help += "//\t -xcs|-excludeSame        [Q]     If the lookupPoint is part of the result, exclude it (useful if the lookupPoints and neighbourPoints are the same)\n";
	help += "//\t -pos|-position           [Q]     Use this option when querying to return the positions of the neighbours instead of their indices\n";
    help += "//\t   -l|-list               [C]     Returns all mNeighbourInfo object names (as a stringArray)\n";
    help += "//\t   -d|-delete             [E]     Delete the specified mNeighbourInfo object.\n";    
    help += "//\t  -da|-deleteAll          [C]     Delete all mNeighbourInfo objects.\n";    
    help += "//\t   -h|-help               [C]     Displays this help.\n";

    MGlobal::displayInfo( help );
}

//************************************************************************//
// now this gets quite elaborate as we can't use the arg parser as it doesn't support arrays

bool argParseIsQuery( const MArgList& args )
{
    int flagIndex = args.flagIndex( "-q","-query" );
    return ( MArgList::kInvalidArgIndex != flagIndex );
}

bool argParseIsEdit( const MArgList& args )
{
    int flagIndex = args.flagIndex( "-e","-edit" );
    return ( MArgList::kInvalidArgIndex != flagIndex );
}

bool argParseIsCreate( const MArgList& args )
{
    int flagIndex = args.flagIndex( "-c","-create" );
    return ( MArgList::kInvalidArgIndex != flagIndex );
}


bool argParseIsFlagSet( const MArgList& args, 
                        const char *shortFlag, const char *longFlag, 
                        int &flagIndex)
{
    flagIndex = args.flagIndex( shortFlag, longFlag);
    return ( MArgList::kInvalidArgIndex != flagIndex );
}


MStatus argParseGetObjectStringArg (const MArgList& args,MString& name)
{
	MStatus status = MS::kSuccess;
	int argIndex = args.length() -1;
    
	name = args.asString(argIndex,&status);
	USER_ERROR_CHECK(status,("mNeighbourInfo: can't get name of mNeighbourInfo object"));

	return status;
}

MStatus argParseGetDblArrayArg( const MArgList& args,  
                                const char *shortFlag, const char *longFlag, 
                                MDoubleArray &argDblA, bool &flagSet)
{
    int flagIndex;
	MStatus status = MS::kSuccess;
    if(argParseIsFlagSet(args,shortFlag, longFlag,flagIndex))
	{
		status = getDoubleArrayArg(args, (flagIndex+1), argDblA);
		USER_ERROR_CHECK(status,("mNeighbourInfo: can't get doubleArray argument for argument "+(flagIndex+1)));
		flagSet = true;
	}
	else
		flagSet = false;

    return status;
}

MStatus mNeighbourInfo::parseArgs( const MArgList& args )
{
 
	MStatus status = MS::kSuccess;
	int flagNum = args.length(); // number of flags that still need to be processed

	///////////////////////////////////////////////
	// are there any flags set or do we want help?
	int index;
	if (argParseIsFlagSet(args,HELP_FLAG,HELP_FLAG_LONG,index))
	{
	     mHelpFlagSet = true;
	     return status;   
	}

	//////////////////////////////////////////////
	// in which mode are we? create, query or edit

	if (argParseIsQuery(args))
	{
		flagNum--;
		mCmdAction = CMD_ACTION_QUERY;
	}

	if (argParseIsEdit(args))
	{
		if ( mCmdAction != CMD_ACTION_QUERY)
		{		
			mCmdAction = CMD_ACTION_EDIT;
			flagNum--;
		}
		else
		{
			USER_ERROR_CHECK(MS::kFailure,"mNeighbourInfo: can't use -edit (-e) and -query (-q) flag at the same time!");
		}
	}
    
	if (argParseIsCreate(args))
	{
		if ( mCmdAction != CMD_ACTION_CREATE)
		{		
			USER_ERROR_CHECK(MS::kFailure,"mNeighbourInfo: can't use -edit (-e) or -query (-q) flag at the same time than -create (-c) flag!");
		}
        else
        {
			flagNum--;        
        }
	}
    

    ////////////////////////////////////////////
	// now get arguments according to query
	bool flagSet; 


	////////////////////////////////////////////
    // parse based on mode
	if ( mCmdAction == CMD_ACTION_CREATE)
	{
    	int index =-1;
        
    	// check for special actions
		if (argParseIsFlagSet(args,DELETE_ALL_FLAG,DELETE_ALL_FLAG_LONG,index)) 
			mCreateAction = CMD_CREATE_DELETE_ALL;       
		else if (argParseIsFlagSet(args,LIST_FLAG ,LIST_FLAG_LONG ,index))
			mCreateAction = CMD_CREATE_LIST;
            		
        if (index>-1)
        	flagNum--;
        else
        {
                
	        // must be created with a list of neighbour points
			status = argParseGetDblArrayArg(args, NEIGHBOUR_POINT_FLAG,NEIGHBOUR_POINT_FLAG_LONG, mNeighbourPoint, flagSet );
			if (status.error()) 
        		return status;
            
			if (flagSet)
			{
				flagNum -=2;
				mNeighbourPointFlagSet = true;
			}        
        	else
	        {
    	    	status = MS::kFailure;
	        	USER_ERROR_CHECK(status,"mNeighbourInfo: when creating a neighbourInfoObject you have to provide a set of neighbour points using the -np|-neighbourPoint flag!");
    	    }        
      	}
        
    }
    else if ( mCmdAction == CMD_ACTION_EDIT)
	{

        // delete the ni object
        int flagIndex;
        if (argParseIsFlagSet(args, DELETE_FLAG,DELETE_FLAG_LONG,flagIndex))
        {
	        mEditAction = CMD_EDIT_DELETE;
            flagNum--;
        }
        else
        {
			// edit the neighbour points
			status = argParseGetDblArrayArg(args, NEIGHBOUR_POINT_FLAG,NEIGHBOUR_POINT_FLAG_LONG, mNeighbourPoint, flagSet );
			if (status.error()) return status;
            
			if (flagSet)
			{
				flagNum -=2;
				mNeighbourPointFlagSet = true;
			}
        }
        
        
        // need a neighbour info object
        status = argParseGetObjectStringArg(args,mNIObject);
		if (status.error()) 
			return status;
		else 
			flagNum--;
        
    }
    else // QUERY
    {
    	// can only query if the lookup info is provided
		status = argParseGetDblArrayArg(args, LOOKUP_RADIUS_FLAG, LOOKUP_RADIUS_FLAG_LONG, mLookupRadius, flagSet ); 
		if (status.error()) return status;
		if (flagSet)
		{
			flagNum -=2;
			mLookupRadiusFlagSet = true;
		}

		status = argParseGetDblArrayArg(args, LOOKUP_POINT_FLAG,LOOKUP_POINT_FLAG_LONG, mLookupPoint, flagSet ); 
		if (status.error()) return status;
		if (flagSet)
		{
			flagNum -=2;
			mLookupPointFlagSet = true;
		}
        
		if (!(mLookupPointFlagSet && mLookupRadiusFlagSet))
        {
			USER_ERROR_CHECK(MS::kFailure,"mNeighbourInfo: if you are querying, you have to provide -lp|-lookupPoint AND -lr|-lookupRange flags as well as a neighbourInfo object!");
        }
               
        // now check for the optional lookup angle parameters
		status = argParseGetDblArrayArg(args, LOOKUP_ANGLE_FLAG,LOOKUP_ANGLE_FLAG_LONG, mLookupAngle, flagSet ); 
		if (status.error()) return status;
		if (flagSet)
		{
			flagNum -=2;
			mLookupAngleFlagSet = true;
		}
        
		status = argParseGetDblArrayArg(args, LOOKUP_DIRECTION_FLAG,LOOKUP_DIRECTION_FLAG_LONG, mLookupDirection, flagSet ); 
		if (status.error()) return status;
		if (flagSet)
		{
			flagNum -=2;
			mLookupDirectionFlagSet = true;
		}
        
        // angle flag set but not direction or vis-e-versa?
		if ( (mLookupAngleFlagSet && (!mLookupDirectionFlagSet)) || ((!mLookupAngleFlagSet) && (mLookupDirectionFlagSet)) )
        {
			USER_ERROR_CHECK(MS::kFailure,"mNeighbourInfo: if you are querying and using the -ld|-lookupDirection and -la|-lookupAngle flags, you have to provide either both flags or none!");
        }
        else
        	if (mLookupAngleFlagSet && mLookupDirectionFlagSet)
	        	mLookupFOV = true;
        
        
        
		// only one of the available query flags should be set
		int index = -1;
		if (argParseIsFlagSet(args,NEAREST_IN_RANGE_FLAG,NEAREST_IN_RANGE_FLAG_LONG,index)) 
			mQueryAction = CMD_QUERY_NEAREST_IN_RANGE;
		else if (argParseIsFlagSet(args,FURTHEST_IN_RANGE_FLAG ,FURTHEST_IN_RANGE_FLAG_LONG ,index))
			mQueryAction = CMD_QUERY_FURTHEST_IN_RANGE;
		else if (argParseIsFlagSet(args,RANDOM_IN_RANGE_FLAG,RANDOM_IN_RANGE_FLAG_LONG,index))
			mQueryAction = CMD_QUERY_RANDOM_IN_RANGE_FLAG;
		else if (argParseIsFlagSet(args,ALL_IN_RANGE_FLAG,ALL_IN_RANGE_FLAG_LONG,index))
			mQueryAction = CMD_QUERY_ALL_IN_RANGE_FLAG;
		else if (argParseIsFlagSet(args,COUNT_IN_RANGE_FLAG,COUNT_IN_RANGE_FLAG_LONG,index))
			mQueryAction = CMD_QUERY_COUNT_IN_RANGE_FLAG;
		else
		{
			USER_ERROR_CHECK(MS::kFailure,"mNeighbourInfo: no matching query lookup flag provided!");
		}        
        
        if (index)
        	flagNum--;
              
        // is the position flag set
		if (argParseIsFlagSet(args,POSITION_FLAG,POSITION_FLAG_LONG,index)) 
		{
			flagNum --;
			mQueryPosition = true;
		}

        // is the exclude same flag set
		if (argParseIsFlagSet(args,EXCLUDE_SAME_FLAG,EXCLUDE_SAME_FLAG_LONG,index)) 
		{
			flagNum --;
			mExcludeSame = true;
		}
        
        // need a neighbour info object
        status = argParseGetObjectStringArg(args,mNIObject);
		if (status.error()) 
			return status;
		else 
			flagNum--;
    }


    
    if (flagNum != 0)
    {
   		USER_ERROR_CHECK(MS::kFailure,"mNeighbourInfo: wrong number of arguments!");
    }

    

    return status;
}


//************************************************************************//
// get NI object from the map
MStatus	mNeighbourInfo::getNeighbourInfoObject(const MString name, mNeighbourInfoStruct &myNIObject)
{
	MStatus status = MS::kSuccess;
	mNeighbourInfoIterType iter;
    
    std::string myName = name.asChar();
	iter = mNIMap.find(myName);
    if(iter == mNIMap.end())
    {
		status = MS::kFailure;

		MString error = "mNeighbourInfo: specified mNeighbourInfoObject '";
        error += mNIObject +"' does not exist!";
        USER_ERROR_CHECK(status,error);    
    }
	else
    {
		myNIObject = iter->second;
    }
    
    return status;
}

//************************************************************************//
// generate a new unique name
std::string	mNeighbourInfo::generateNeighbourInfoObjectName()
{
    
    MString newName = "mNeighbourInfoObject";
    newName += mNeighbourInfoIndex;
    mNeighbourInfoIndex++;

    return (std::string(newName.asChar()));
}

//************************************************************************//
// get a list of all neighbour info objects

MStatus mNeighbourInfo::listAllNeighbourInfoObjects( )
{
	MStatus status = MS::kSuccess;

    MStringArray mNIList; mNIList.clear();
    
    mNeighbourInfoIterType iter;
  	
    for(iter = mNIMap.begin(); iter != mNIMap.end(); iter++)
        mNIList.append(MString( (*iter).first.c_str() ));
    
	setResult(mNIList);
    
	return status;
}

//************************************************************************//
// delete current neighbour info objects

MStatus mNeighbourInfo::deleteNeighbourInfoObject( )
{
	MStatus status = MS::kSuccess;    

	mNeighbourInfoIterType iter;
    
    std::string myName = mNIObject.asChar();
    
	iter = mNIMap.find(myName);
    if(iter == mNIMap.end())
    {
		status = MS::kFailure;

		MString error = "mNeighbourInfo: specified mNeighbourInfoObject '";
        error += mNIObject +"' does not exist!";
        USER_ERROR_CHECK(status,error);    
    }
	else
    {
    	mNeighbourInfoStruct myNI =  iter->second;
        myNI.clear();
		mNIMap.erase(iter);
    }
    	
    
	return status;
}

//************************************************************************//
// delete all neighbour info objects

MStatus mNeighbourInfo::deleteAllNeighbourInfoObjects( )
{
	MStatus status = MS::kSuccess;    

	while( !mNIMap.empty() ) 
    {
	 	mNeighbourInfoStruct myNI = mNIMap.begin()->second;
        myNI.clear();
        mNIMap.erase( mNIMap.begin() );
    }

	mNeighbourInfoIndex =0;
	return status;
}

//************************************************************************//

MStatus mNeighbourInfo::updateNeighbourInfoObject(mNeighbourInfoStruct &myNI)
{
	MStatus status;

	if(mNeighbourPointFlagSet)
    {

    	// check if we have a valid array
        unsigned int size;
		status = vecIsValid(mNeighbourPoint,size);
		if (status.error()) return status;
        
        // clear the contents of the NI

        myNI.clear();
		// build the kd tree from the provided points                
        for (int i=0; i<mNeighbourPoint.length(); i+=3)
        {
        	triplet c= {mNeighbourPoint[i], mNeighbourPoint[i+1], mNeighbourPoint[i+2]};
            c.index = i/3.0;
        	myNI.insert(c);
        }
        
        myNI.optimise();

	}
}

          
//************************************************************************//
MStatus mNeighbourInfo::doCreate( )
{
	MStatus status;
    

	switch (mCreateAction)
	{
    	case CMD_CREATE_CREATE:
        {
		    // create a new NI object
		    mNeighbourInfoStruct myNI; 
    
		    // store the object in the map
			std::string name = generateNeighbourInfoObjectName();
    
		    // fill the object with the cmd line values
		    status = updateNeighbourInfoObject(myNI);
			if (status.error())	return status;        

			// store it in the map	
			mNIMap[name] = myNI; 
		    setResult(MString(name.c_str()));
            
            break;
        }
        
		case CMD_CREATE_DELETE_ALL: 
        {
	        status = deleteAllNeighbourInfoObjects(); 
            break;
        }
        
        case CMD_CREATE_LIST: 
        {
        	status = listAllNeighbourInfoObjects(); 
            break;
        }
    }

    
	return status;
}
//************************************************************************//
MStatus mNeighbourInfo::doEdit( )
{
	MStatus status;

    

	switch (mEditAction)
	{
    	case CMD_EDIT_DELETE: 
        {
        	deleteNeighbourInfoObject(); 
            break;
        }
        
        default:
        {
			// get the ni object
			mNeighbourInfoStruct myNI;
			status = getNeighbourInfoObject(mNIObject,myNI);
		    if (status.error())	return status;    
    
	        // fill the object with the cmd line values
		    status = updateNeighbourInfoObject(myNI);
            mNIMap[(std::string(mNIObject.asChar()))] = myNI; 
        }
    }
        
	return status;
}


//************************************************************************//
// get all the neighbours in the kd tree for specified lookup info
// filter for field of view
void mNeighbourInfo::getNeighboursInFOV(const mNeighbourInfoStruct &myNI,
											 const MVector &lookupPoint,
                                             const double lookupRadius,
											 const MVector &lookupDirection,                                             
                                             double lookupAngle,
                                             MVectorArray &nearPoint,
                                             MDoubleArray &nearIndex,
                                             MDoubleArray &nearDistanceSqr,                                             
                                             int &nearCount) 
{
	nearPoint.clear();
    nearIndex.clear();
    nearDistanceSqr.clear();
    
    double lookupRadiusSqr = lookupRadius * lookupRadius;  
	
    // half the lookup angle, as it referes to the full fov, not the angle around the
    // lookup dir
    lookupAngle /=2.0;

	// find in kdtree using the lookup radius
	triplet currentLookup = {lookupPoint.x, lookupPoint.y, lookupPoint.z};
  	std::vector<triplet> nearestNodes;
  	myNI.find_within_range(currentLookup, lookupRadius, std::back_inserter(nearestNodes));
  
  	// iterate over the result and make sure it is
    // a) within the radius and
    // b) within the angle
  	std::vector<triplet>::const_iterator nearestNodesIter = nearestNodes.begin();
    
    // do we want to check for fov,
 	for (; nearestNodesIter != nearestNodes.end(); ++nearestNodesIter)
   	{
    	triplet myNode = *nearestNodesIter;
        
   	    MVector point(myNode.d[0],myNode.d[1],myNode.d[2]);

        if (mExcludeSame)
        	if (point == lookupPoint)
            	continue;

        // check if it is within the radius distance       
   	    double distSqr = distanceSqr(point, lookupPoint);
        
        if (distSqr <= lookupRadiusSqr)
   	    {
       		// yes, check if it is within the angle, direction
            MVector diff = point - lookupPoint;
   	        double angle = diff.angle(lookupDirection);
            
       	    if (angle <= lookupAngle)
           	{
	           	nearPoint.append(MVector(myNode.d[0],myNode.d[1],myNode.d[2]));
   		    	nearIndex.append(myNode.index);
       		    nearDistanceSqr.append(distSqr);
           	}
        }
   }
    
    nearCount = nearIndex.length();
}

//************************************************************************//
// get all the neighbours in the kd tree for specified lookup info
// filter for radius around
void mNeighbourInfo::getNeighboursInRadius(const mNeighbourInfoStruct &myNI,
											 const MVector &lookupPoint,
                                             const double lookupRadius,
                                             MVectorArray &nearPoint,
                                             MDoubleArray &nearIndex,
                                             MDoubleArray &nearDistanceSqr,                                             
                                             int &nearCount) 
{
	nearPoint.clear();
    nearIndex.clear();
    nearDistanceSqr.clear();
    
    double lookupRadiusSqr = lookupRadius * lookupRadius;  

	// find in kdtree using the lookup radius
	triplet currentLookup = {lookupPoint.x, lookupPoint.y, lookupPoint.z};
  	std::vector<triplet> nearestNodes;
  	myNI.find_within_range(currentLookup, lookupRadius, std::back_inserter(nearestNodes));
  
  	// iterate over the result and make sure it is
    // a) within the radius and
    // b) within the angle
  	std::vector<triplet>::const_iterator nearestNodesIter = nearestNodes.begin();
    
	// only check for radius
	for (; nearestNodesIter != nearestNodes.end(); ++nearestNodesIter)
    {
	   	triplet myNode = *nearestNodesIter;
       
        MVector point(myNode.d[0],myNode.d[1],myNode.d[2]);
        
        if (mExcludeSame)
        	if (point == lookupPoint)
            	continue;

	       // check if it is within the radius distance       
        double distSqr = distanceSqr(point, lookupPoint);
        
	    if (distSqr <= lookupRadiusSqr)
        {
	       	nearPoint.append(MVector(myNode.d[0],myNode.d[1],myNode.d[2]));
   		   	nearIndex.append(myNode.index);
       	    nearDistanceSqr.append(distSqr);
	    }
    }
            
    nearCount = nearIndex.length();
}

//************************************************************************//
// get all the neighbours dependent on setting
void mNeighbourInfo::getNeighbours(	const mNeighbourInfoStruct &myNI,
	       					const unsigned int incLP, const unsigned int incLR, 
							const unsigned int incLD, const unsigned int incLA,         
                            int &iterLP, int &iterLR,int &iterLD,int &iterLA,
                            MVectorArray &nearPoint, MDoubleArray &nearIndex,
                            MDoubleArray &nearDistanceSqr, int &nearCount)                           
{
	 // get all points in range
     int indexLP = iterLP*3;
     MVector currLookupPoint(mLookupPoint[indexLP],mLookupPoint[indexLP+1],mLookupPoint[indexLP+2]);
     double currLookupRadius = mLookupRadius[iterLR];

     if (mLookupFOV)
     {
         int indexLD = iterLD*3;
         MVector currLookupDir(mLookupDirection[indexLD],mLookupDirection[indexLD+1],mLookupDirection[indexLD+2]);
         double currLookupAngle = mLookupAngle[iterLA];        

         getNeighboursInFOV(myNI,currLookupPoint,currLookupRadius,currLookupDir,currLookupAngle,nearPoint,nearIndex,nearDistanceSqr,nearCount);
     
         // update indices
         iterLD += incLD;
         iterLA += incLA;                      

     }      
     else
         getNeighboursInRadius(myNI,currLookupPoint,currLookupRadius,nearPoint,nearIndex,nearDistanceSqr,nearCount);

	 // update the indices
     iterLP += incLP;
	 iterLR += incLR;  
}


//************************************************************************//
// get the nearest point
MStatus mNeighbourInfo::doQueryNearestInRange(const mNeighbourInfoStruct &myNI,
                                              const unsigned int incLP,
                                              const unsigned int incLR, 
											const unsigned int incLD, const unsigned int incLA,                                               
                                              const unsigned int count,
                                              MDoubleArray &result) 
{

	MStatus status;


	MVectorArray nearPoint;
    MDoubleArray nearIndex;
    MDoubleArray nearDistanceSqr;    
    
    // iterate through all lookup points,
    // determine the nearest in the
    int iterLP, iterLR,iterLA, iterLD;
    iterLP=iterLR=iterLD=iterLA=0;
    
    
        
	if (mQueryPosition)
    {
    	// get the positions
        
		MVectorArray vecResult(count);
        
        for(int i=0;i<count;i++)
    	{

		    int nearCount;              
    	   	getNeighbours(myNI,	incLP,  incLR, incLD,  incLA,         
	                      iterLP, iterLR,iterLD, iterLA,
                          nearPoint, nearIndex, nearDistanceSqr, nearCount)  ; 
                            
	
			if (nearCount == 1)
        	{
        		vecResult[i] = nearPoint[0];
	        }
    	    else
        	{
				int nearestPointId = -1;
	    	    double dist = DBL_MAX;
        
		        // now iterate through the result and find the furthest one
				for (int j=0;j<nearCount;j++)
        		{
		            if (nearDistanceSqr[j] < dist)
    		        {
        		    	dist = nearDistanceSqr[j];
            		    nearestPointId = j;
	            	}
	    	    } 
        
    	    	if (nearestPointId > -1)
	    	    	vecResult[i] = nearPoint[nearestPointId];
	        }
                      
        }
        
        result = vectorArrayToDoubleArray(vecResult);
        
	}
    else
    {
    	// get the indices
	    result = MDoubleArray(count, -1.0);
          
	    for(int i=0;i<count;i++)
    	{
		    int nearCount;              
    	   	getNeighbours(myNI,	incLP,  incLR, incLD,  incLA,         
	                           iterLP, iterLR,iterLD, iterLA,
                            nearPoint, nearIndex, nearDistanceSqr, nearCount)  ; 
                            
			if (nearCount == 1)
    	    {
        		result[i] = nearIndex[0];
	        }
    	    else
        	{
        
				int nearestPointId = -1;
    		    double dist = DBL_MAX;
        
        		// now iterate through the result and find the nearest one
				for (int j=0;j<nearCount;j++)
	    	    {

	    	        if (nearDistanceSqr[j] < dist)
    	    	    {
        	    		dist = nearDistanceSqr[j];
            	    	nearestPointId = j;
		            }
    		    } 
        
	    	    if (nearestPointId>-1)
		    	    result[i] = nearIndex[nearestPointId];
        
	        }
		}
    }
    
    return status;
}

//************************************************************************//
// get the nearest point
MStatus mNeighbourInfo::doQueryFurthestInRange(const mNeighbourInfoStruct &myNI,
                                              const unsigned int incLP,
                                              const unsigned int incLR,
											const unsigned int incLD, const unsigned int incLA,                                                
                                              const unsigned int count,
                                              MDoubleArray &result) 
{
	MStatus status;


	MVectorArray nearPoint;
    MDoubleArray nearIndex;
    MDoubleArray nearDistanceSqr;    
 
    // iterate through all lookup points,
    // determine the nearest in the
    int iterLP, iterLR,iterLD,iterLA;
    iterLP=iterLR=iterLD=iterLA=0;
    
	if (mQueryPosition)
    {
    	// get the positions
        //
        
		MVectorArray vecResult(count);
        
        for(int i=0;i<count;i++)
    	{

		    int nearCount;              
    	   	getNeighbours(myNI,	incLP,  incLR, incLD,  incLA,         
	                      iterLP, iterLR,iterLD, iterLA,
                          nearPoint, nearIndex, nearDistanceSqr, nearCount)  ; 
                          
                          	
			if (nearCount == 1)
        	{
        		vecResult[i] = nearPoint[0];
	        }
    	    else
        	{
				int nearestPointId = -1;
	    	    double dist = DBL_MIN;
        
		        // now iterate through the result and find the furthest one
				for (int j=0;j<nearCount;j++)
        		{

		            if (nearDistanceSqr[j] > dist)
    		        {
        		    	dist = nearDistanceSqr[j];
            		    nearestPointId = j;
	            	}
	    	    } 
        
    	    	if (nearestPointId > -1)
	    	    	vecResult[i] = nearPoint[nearestPointId];
	        }
                
     
        }
        
        result = vectorArrayToDoubleArray(vecResult);
        
	}
    else
    {
		// get the indices
	    result = MDoubleArray(count, -1.0);
          
	    for(int i=0;i<count;i++)
    	{

		    int nearCount;              
    	   	getNeighbours(myNI,	incLP,  incLR, incLD,  incLA,         
	                      iterLP, iterLR,iterLD, iterLA,
                          nearPoint, nearIndex, nearDistanceSqr, nearCount)  ; 
                          	
			if (nearCount == 1)
        	{
        		result[i] = nearIndex[0];
	        }
    	    else
        	{
				int nearestPointId = -1;
	    	    double dist = DBL_MIN;
        
		        // now iterate through the result and find the furthest one
				for (int j=0;j<nearCount;j++)
        		{
		            if (nearDistanceSqr[j] > dist)
    		        {
        		    	dist = nearDistanceSqr[j];
            		    nearestPointId = j;
	            	}
	    	    } 
        
    	    	if (nearestPointId > -1)
	    	    	result[i] = nearIndex[nearestPointId];
	        }
                
        }
        
    }
    
    return status;
}

//************************************************************************//
// get a random point in the range
MStatus mNeighbourInfo::doQueryRandomInRange(const mNeighbourInfoStruct &myNI,
                                              const unsigned int incLP, const unsigned int incLR, 
											const unsigned int incLD, const unsigned int incLA,                                               
                                              const unsigned int count, MDoubleArray &result) 
{
	MStatus status;

	MVectorArray nearPoint;
    MDoubleArray nearIndex;
    MDoubleArray nearDistanceSqr;    
        
    // iterate through all lookup points,
    // determine the nearest in the
    int iterLP, iterLR, iterLD, iterLA;
    iterLP=iterLR=iterLD=iterLA=0;
    
    result = MDoubleArray(count, -1.0);
          
	// init the random number generator with a seed
    srand(666);
    
	if (mQueryPosition)
    {
    	// get the positions
        //
        
		MVectorArray vecResult(count);
        
    	// only go for indices
	    for(int i=0;i<count;i++)
	    {
		    int nearCount;              
    	   	getNeighbours(myNI,	incLP,  incLR, incLD,  incLA,         
	                           iterLP, iterLR,iterLD, iterLA,
                            nearPoint, nearIndex, nearDistanceSqr, nearCount)  ; 
        
			if (nearCount == 1)
    			vecResult[i] = nearPoint[0];
	        else
	        {
        
		    	if (nearCount >0)
    		    {
	  				int randomIndex = rand()%nearCount;
           			vecResult[i] = nearPoint[randomIndex];
		        }
        
    	   }
	    }
        
        result = vectorArrayToDoubleArray(vecResult);
        
    }
    else
    {
    	// only go for indices
	    for(int i=0;i<count;i++)
	    {

		    int nearCount;              
    	   	getNeighbours(myNI,	incLP,  incLR, incLD,  incLA,         
	                           iterLP, iterLR,iterLD, iterLA,
                            nearPoint, nearIndex, nearDistanceSqr, nearCount)  ; 
                
			if (nearCount == 1)
        		result[i] = nearIndex[0];
    	    else
	        {
        
		    	if (nearCount >0)
    		    {
	  				int randomIndex = rand()%nearCount;
           			result[i] = nearIndex[randomIndex];
		        }
        
    	   }
	    } // indices
        
    } // else
   
    return status;
}



//************************************************************************//
// get all in range
MStatus mNeighbourInfo::doQueryAllInRange(const mNeighbourInfoStruct &myNI,
                                              const unsigned int incLP, const unsigned int incLR, 
											const unsigned int incLD, const unsigned int incLA,                                               
                                              const unsigned int count, MDoubleArray &result) 
{
	MStatus status;

	// any of the parameter arrays bigger than 1 element? > ERROR
    if ((incLP > 0) || (incLR >0) || (incLD >0) || (incLA >0))
    {
    	status = MS::kFailure;

		MString error = "mNeighbourInfo: when querying for -air|-allInRange you can only provide a single element for -lp|-lookupPoint, -lr|-lookupRange, -ld|-lookupDirection and -la|-lookupAngle!";
        USER_ERROR_CHECK(status,error);    
    }
    
	MVectorArray nearPoint;
    MDoubleArray nearIndex,nearDistanceSqr;
              
	// get all points in range
    MVector currLookupPoint(mLookupPoint[0],mLookupPoint[1],mLookupPoint[2]);
    double currLookupRange = mLookupRadius[0];
        
    int nearCount;  
  
	if (mLookupFOV)
    {
    	MVector currLookupDir(mLookupDirection[0],mLookupDirection[1],mLookupDirection[2]);
	    double currLookupAngle = mLookupAngle[0];        
        
        getNeighboursInFOV(myNI,currLookupPoint,currLookupRange,currLookupDir,currLookupAngle,nearPoint,nearIndex,nearDistanceSqr,nearCount);
    }      
    else
	    getNeighboursInRadius(myNI,currLookupPoint,currLookupRange,nearPoint,nearIndex,nearDistanceSqr,nearCount);

	if (mQueryPosition)
        result = vectorArrayToDoubleArray(nearPoint);
    else
		result.copy(nearIndex);	
    
    return status;
}

//************************************************************************//
// get number of points in range
MStatus mNeighbourInfo::doQueryCountInRange(const mNeighbourInfoStruct &myNI,
                                              const unsigned int incLP, const unsigned int incLR, 
												const unsigned int incLD, const unsigned int incLA,                                               
                                              const unsigned int count, MDoubleArray &result) 
{
	MStatus status;

	MVectorArray nearPoint;
    MDoubleArray nearIndex;
    MDoubleArray nearDistanceSqr; 
        
    // iterate through all lookup points,
    // determine the nearest in the
    int iterLP, iterLR,iterLD, iterLA;
    iterLP=iterLR=iterLD=iterLA =0;
    
    result = MDoubleArray(count, 0);
          
	if (mQueryPosition)
    {
    	status = MS::kFailure;    	
		MString error = "mNeighbourInfo: can't use -pos|-position flag when querying for -cir|-countInRange!'";
        USER_ERROR_CHECK(status,error);     
    }

	for(int i=0;i<count;i++)
	{
		// get all points in range
	    int nearCount;    
                
       	getNeighbours(myNI,	incLP,  incLR, incLD,  incLA,         
                            iterLP, iterLR,iterLD, iterLA,
                            nearPoint, nearIndex, nearDistanceSqr, nearCount)  ; 
                             
 		result[i] = nearCount;
	
	}
        
        
 
        
   
    return status;
}

//************************************************************************//
MStatus mNeighbourInfo::doQuery( )
{
	MStatus status;

	// get the ni object
	mNeighbourInfoStruct myNI;
	status = getNeighbourInfoObject(mNIObject,myNI);
    if (status.error()) return status;    

	// check the size of the lookup info
	unsigned int sizeLP, sizeLR, sizeLA, sizeLD, incLP, incLR, incLA, incLD, sizeResult;	
    status = vecIsValid(mLookupPoint,sizeLP);
	if (status.error()) return status;
    
    sizeLR=mLookupRadius.length();

	// do we only query for radius?
	if (!mLookupFOV)
	    status = twoArgCountsValid(sizeLR,sizeLP,incLR,incLP, sizeResult);
    else
    {
    	// check additional flag arguments
	    status = vecIsValid(mLookupDirection,sizeLD);
		if (status.error()) return status;
        
		sizeLA = mLookupAngle.length();   
        
        status = fourArgCountsValid(sizeLR,sizeLP,sizeLD,sizeLA,incLR,incLP,incLD,incLA, sizeResult);
    }
    
    if (status.error()) return status;                              


	// do the kd tree lookup   
	// query the object
	MDoubleArray result;

	switch (mQueryAction)
	{
		case CMD_QUERY_NEAREST_IN_RANGE: 		status = doQueryNearestInRange(	myNI,incLP,incLR,incLD,incLA,sizeResult,result); break;
		case CMD_QUERY_FURTHEST_IN_RANGE:		status = doQueryFurthestInRange(myNI,incLP,incLR,incLD,incLA,sizeResult,result); break;
		case CMD_QUERY_RANDOM_IN_RANGE_FLAG:	status = doQueryRandomInRange(	myNI,incLP,incLR,incLD,incLA,sizeResult,result); break;
		case CMD_QUERY_ALL_IN_RANGE_FLAG:		status = doQueryAllInRange(		myNI,incLP,incLR,incLD,incLA,sizeResult,result); break;
		case CMD_QUERY_COUNT_IN_RANGE_FLAG:		status = doQueryCountInRange(	myNI,incLP,incLR,incLD,incLA,sizeResult,result); break;        
	}

	if (status.error()) 
    	return status;
    else
    {
		setResult(result);
		return status;
    }
}

//************************************************************************//

MStatus mNeighbourInfo::doIt( const MArgList& args )
{
    
    
    MStatus status = MS::kSuccess;
    status = parseArgs(args);
    if(!status)
    	return status;
     
	if (mHelpFlagSet)
    {
        help();
        return MS::kSuccess;
    }

	switch (mCmdAction)
	{
		case CMD_ACTION_CREATE: status = doCreate(); break;
		case CMD_ACTION_EDIT: status = doEdit(); break;
		case CMD_ACTION_QUERY: status = doQuery(); break;
		default:
		{
			status = MS::kFailure;
			USER_ERROR_CHECK(status,"mNeighbourInfo: error parsing arguments!");
		}
	}

	return status;
}


} // end namespace

