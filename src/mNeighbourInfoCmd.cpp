#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MStringArray.h>

#include <values.h>

#include "../include/mHelperMacros.h"
#include "../include/mNeighbourInfoCmd.h"
#include "../include/mHelperFunctions.h"

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
	mHelpFlagSet = false;    
    mLookupRangeFlagSet = false;            
    mLookupPointFlagSet = false;            
    mNeighbourPointFlagSet = false;            

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
    help += "//\t  -lr|-lookupRange        [Q]     Mandator upon Query: Specify a double array of radii around each lookup points used as the lookup range \n";
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
		status = argParseGetDblArrayArg(args, LOOKUP_RANGE_FLAG, LOOKUP_RANGE_FLAG_LONG, mLookupRange, flagSet ); 
		if (status.error()) return status;
		if (flagSet)
		{
			flagNum -=2;
			mLookupRangeFlagSet = true;
		}

		status = argParseGetDblArrayArg(args, LOOKUP_POINT_FLAG,LOOKUP_POINT_FLAG_LONG, mLookupPoint, flagSet ); 
		if (status.error()) return status;
		if (flagSet)
		{
			flagNum -=2;
			mLookupPointFlagSet = true;
		}
        
		if (!(mLookupPointFlagSet && mLookupRangeFlagSet))
        {
			USER_ERROR_CHECK(MS::kFailure,"mNeighbourInfo: if you are querying, you have to provide -lp|-lookupPoint AND -lr|-lookupRange flags as well as a neighbourInfo object!");
        }
               
        
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
void mNeighbourInfo::getNeighboursInRange(const mNeighbourInfoStruct &myNI,
											 const MVector &lookupPoint,
                                             const double lookupRange,
                                             MVectorArray &nearPoint,
                                             MDoubleArray &nearIndex,
                                             MDoubleArray &nearDistanceSqr,                                             
                                             int &nearCount) 
{
	nearPoint.clear();
    nearIndex.clear();
    nearDistanceSqr.clear();
    
    double lookupRangeSqr = lookupRange * lookupRange;
    
	triplet currentLookup = {lookupPoint.x, lookupPoint.y, lookupPoint.z};
    
  	std::vector<triplet> nearestNodes;
  	myNI.find_within_range(currentLookup, lookupRange, std::back_inserter(nearestNodes));
  
  	std::vector<triplet>::const_iterator nearestNodesIter = nearestNodes.begin();
  	for (; nearestNodesIter != nearestNodes.end(); ++nearestNodesIter)
    {
    	triplet myNode = *nearestNodesIter;
        
        MVector point(myNode.d[0],myNode.d[1],myNode.d[2]);

        // check if it is within the range distance       
        double distSqr = distanceSqr(point, lookupPoint);
        
        if (distSqr <= lookupRangeSqr)
        {
           	nearPoint.append(MVector(myNode.d[0],myNode.d[1],myNode.d[2]));
        	nearIndex.append(myNode.index);
            nearDistanceSqr.append(distSqr);
        }
    }
    
//    cerr <<"\nall in range " << lookupPoint<<" , "<<lookupRange<<"\nnearPoint "<<nearPoint<<" nearIndex "<<nearIndex;
    
    nearCount = nearIndex.length();
}


//************************************************************************//
// get the nearest point
MStatus mNeighbourInfo::doQueryNearestInRange(const mNeighbourInfoStruct &myNI,
                                              const unsigned int incLP,
                                              const unsigned int incLR, 
                                              const unsigned int count,
                                              MDoubleArray &result) 
{

	MStatus status;


	MVectorArray nearPoint;
    MDoubleArray nearIndex;
    MDoubleArray nearDistanceSqr;    
    
    // iterate through all lookup points,
    // determine the nearest in the
    int iterLP, iterLR;
    iterLP=iterLR=0;
    
    
        
	if (mQueryPosition)
    {
    	// get the positions
        
		MVectorArray vecResult(count);
        
        for(int i=0;i<count;i++)
    	{

			// get all points in range
    	    int index = iterLP*3;
        	MVector currLookupPoint(mLookupPoint[index],mLookupPoint[index+1],mLookupPoint[index+2]);
	        double currLookupRange = mLookupRange[iterLR];
        
		    int nearCount;    
        
	        getNeighboursInRange(myNI,currLookupPoint,currLookupRange,nearPoint,nearIndex,nearDistanceSqr,nearCount);
	
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
                
        	// update the indices
	        iterLP += incLP;
    	    iterLR += incLR;        
        }
        
        result = vectorArrayToDoubleArray(vecResult);
        
	}
    else
    {
    	// get the indices
	    result = MDoubleArray(count, -1.0);
          
	    for(int i=0;i<count;i++)
    	{
			// get all points in range
    	    int index = iterLP*3;
        	MVector currLookupPoint(mLookupPoint[index],mLookupPoint[index+1],mLookupPoint[index+2]);
	        double currLookupRange = mLookupRange[iterLR];
        
		    int nearCount;    
        
	        getNeighboursInRange(myNI,currLookupPoint,currLookupRange,nearPoint,nearIndex,nearDistanceSqr,nearCount);

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
    	    // update the indices
        	iterLP += incLP;
	        iterLR += incLR;        
		}
    }
    
    return status;
}

//************************************************************************//
// get the nearest point
MStatus mNeighbourInfo::doQueryFurthestInRange(const mNeighbourInfoStruct &myNI,
                                              const unsigned int incLP,
                                              const unsigned int incLR, 
                                              const unsigned int count,
                                              MDoubleArray &result) 
{
	MStatus status;


	MVectorArray nearPoint;
    MDoubleArray nearIndex;
    MDoubleArray nearDistanceSqr;    
 
    // iterate through all lookup points,
    // determine the nearest in the
    int iterLP, iterLR;
    iterLP=iterLR=0;
    
	if (mQueryPosition)
    {
    	// get the positions
        //
        
		MVectorArray vecResult(count);
        
        for(int i=0;i<count;i++)
    	{

			// get all points in range
    	    int index = iterLP*3;
        	MVector currLookupPoint(mLookupPoint[index],mLookupPoint[index+1],mLookupPoint[index+2]);
	        double currLookupRange = mLookupRange[iterLR];
        
		    int nearCount;    
        
	        getNeighboursInRange(myNI,currLookupPoint,currLookupRange,nearPoint,nearIndex,nearDistanceSqr,nearCount);
	
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
                
        	// update the indices
	        iterLP += incLP;
    	    iterLR += incLR;        
        }
        
        result = vectorArrayToDoubleArray(vecResult);
        
	}
    else
    {
		// get the indices
	    result = MDoubleArray(count, -1.0);
          
	    for(int i=0;i<count;i++)
    	{

			// get all points in range
    	    int index = iterLP*3;
        	MVector currLookupPoint(mLookupPoint[index],mLookupPoint[index+1],mLookupPoint[index+2]);
	        double currLookupRange = mLookupRange[iterLR];
        
		    int nearCount;    
        
	        getNeighboursInRange(myNI,currLookupPoint,currLookupRange,nearPoint,nearIndex,nearDistanceSqr,nearCount);
	
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
                
        	// update the indices
	        iterLP += incLP;
    	    iterLR += incLR;        
        }
        
    }
    
    return status;
}

//************************************************************************//
// get a random point in the range
MStatus mNeighbourInfo::doQueryRandomInRange(const mNeighbourInfoStruct &myNI,
                                              const unsigned int incLP, const unsigned int incLR, 
                                              const unsigned int count, MDoubleArray &result) 
{
	MStatus status;

	MVectorArray nearPoint;
    MDoubleArray nearIndex;
    MDoubleArray nearDistanceSqr;    
        
    // iterate through all lookup points,
    // determine the nearest in the
    int iterLP, iterLR;
    iterLP=iterLR=0;
    
    result = MDoubleArray(count, -1.0);
          
	// init the random number generator with a seed
    // TODO: interface this seed
    srand(666);
    
	if (mQueryPosition)
    {
    	// get the positions
        //
        
		MVectorArray vecResult(count);
        
    	// only go for indices
	    for(int i=0;i<count;i++)
	    {

			// get all points in range
	    	int index = iterLP*3;
    	    MVector currLookupPoint(mLookupPoint[index],mLookupPoint[index+1],mLookupPoint[index+2]);
        	double currLookupRange = mLookupRange[iterLR];
        
		    int nearCount;    
        
    	    getNeighboursInRange(myNI,currLookupPoint,currLookupRange,nearPoint,nearIndex,nearDistanceSqr,nearCount);

			if (nearCount == 1)
    	    {
        		vecResult[i] = nearPoint[0];
	        }
    	    else
	        {
        
		    	if (nearCount >0)
    		    {
	  				int randomIndex = rand()%nearCount;
           			vecResult[i] = nearPoint[randomIndex];
		        }
        
    	   }
	
           // update the indices
    	   iterLP += incLP;
	   	   iterLR += incLR;        
	    }
        
        result = vectorArrayToDoubleArray(vecResult);
        
    }
    else
    {
    	// only go for indices
	    for(int i=0;i<count;i++)
	    {

			// get all points in range
	    	int index = iterLP*3;
    	    MVector currLookupPoint(mLookupPoint[index],mLookupPoint[index+1],mLookupPoint[index+2]);
        	double currLookupRange = mLookupRange[iterLR];
        
		    int nearCount;    
        
    	    getNeighboursInRange(myNI,currLookupPoint,currLookupRange,nearPoint,nearIndex,nearDistanceSqr,nearCount);

			if (nearCount == 1)
    	    {
        		result[i] = nearIndex[0];
	        }
    	    else
	        {
        
		    	if (nearCount >0)
    		    {
	  				int randomIndex = rand()%nearCount;
           			result[i] = nearIndex[randomIndex];
		        }
        
    	   }
	
           // update the indices
    	   iterLP += incLP;
	   	   iterLR += incLR;        
	    }
        
    }
    
   
    return status;
}

//************************************************************************//
// get all in range
MStatus mNeighbourInfo::doQueryAllInRange(const mNeighbourInfoStruct &myNI,
                                              const unsigned int incLP, const unsigned int incLR, 
                                              const unsigned int count, MDoubleArray &result) 
{
	MStatus status;

	// are the parameter arrays bigger than 1 element? > ERROR
    if ((incLP > 0) && (incLR >0))
    {
    	status = MS::kFailure;

		MString error = "mNeighbourInfo: when querying for -air|-allInRange you can only provide a single element for -lp|-lookupPoint and -lr|-lookupRange!'";
        USER_ERROR_CHECK(status,error);    
    }
    
	MVectorArray nearPoint;
    MDoubleArray nearIndex,nearDistanceSqr;
              
	// get all points in range
    MVector currLookupPoint(mLookupPoint[0],mLookupPoint[1],mLookupPoint[2]);
    double currLookupRange = mLookupRange[0];
        
    int nearCount;    
    getNeighboursInRange(myNI,currLookupPoint,currLookupRange,nearPoint,nearIndex,nearDistanceSqr,nearCount);

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
                                              const unsigned int count, MDoubleArray &result) 
{
	MStatus status;

	MVectorArray nearPoint;
    MDoubleArray nearIndex;
     MDoubleArray nearDistanceSqr; 
        
    // iterate through all lookup points,
    // determine the nearest in the
    int iterLP, iterLR;
    iterLP=iterLR=0;
    
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
		int index = iterLP*3;
        MVector currLookupPoint(mLookupPoint[index],mLookupPoint[index+1],mLookupPoint[index+2]);
    	double currLookupRange = mLookupRange[iterLR];
    
	    int nearCount;    
    
        getNeighboursInRange(myNI,currLookupPoint,currLookupRange,nearPoint,nearIndex,nearDistanceSqr,nearCount);

 		result[i] = nearCount;
	
       // update the indices
       iterLP += incLP;
	   iterLR += incLR;        
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
	unsigned int sizeLP, sizeLR, incLP, incLR, sizeResult;	
    status = vecIsValid(mLookupPoint,sizeLP);
	if (status.error()) return status;
    
    sizeLR=mLookupRange.length();
        
    status = twoArgCountsValid(sizeLR,sizeLP,incLR,incLP, sizeResult);
    if (status.error()) return status;                              

	// do the kd tree lookup   
	// query the object
	MDoubleArray result;

	switch (mQueryAction)
	{
		case CMD_QUERY_NEAREST_IN_RANGE: 		status = doQueryNearestInRange(myNI,incLP,incLR,sizeResult,result); break;
		case CMD_QUERY_FURTHEST_IN_RANGE:		status = doQueryFurthestInRange(myNI,incLP,incLR,sizeResult,result); break;
		case CMD_QUERY_RANDOM_IN_RANGE_FLAG:	status = doQueryRandomInRange(myNI,incLP,incLR,sizeResult,result); break;
		case CMD_QUERY_ALL_IN_RANGE_FLAG:		status = doQueryAllInRange(myNI,incLP,incLR,sizeResult,result); break;
		case CMD_QUERY_COUNT_IN_RANGE_FLAG:		status = doQueryCountInRange(myNI,incLP,incLR,sizeResult,result); break;        
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
