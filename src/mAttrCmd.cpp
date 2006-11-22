// Title: Attribute Commands
//
// About:
// A collection of commands for you to conviniently manage arrays on attributes.
//


#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnAttribute.h>
#include <maya/MDoubleArray.h>
#include <maya/MArgList.h>


#include "../include/mHelperFunctions.h"
#include "../include/mHelperMacros.h"
#include "../include/mAttrCmd.h"

namespace melfunctions
{


//************************************************************************************************//
/*
   Function: mDblSetAttr
   Set double array on attribute

   Parameters:
		doubleArrayA - double array 
        nodeAttr - node.attribute
        
   Returns:

*/
#define mel int mDblSetAttr(float $doubleArrayA[], string $nodeAttr);
#undef mel


CREATOR(mDblSetAttr)

MStatus mDblSetAttr::doIt( const MArgList& args )
{
    // get the arguments
	MStatus stat = getDoubleArrayArg(args,0,mDblArg);    
	ERROR_FAIL(stat);

	MString nodeAttr;
	stat = getStringArg(args,1,mNodeAttr);
    ERROR_FAIL(stat);

    
    // get the plug to the nodeAttr
    MPlug plug;
  	stat = getPlugFromString(commandString(),mNodeAttr, plug);
	ERROR_FAIL(stat);      

    
	// is the plug a double array?
//    MObject data;
//    stat = plug.getValue(data);
//   	if (!data.hasFn( MFn::kDoubleArrayData ))
//    {
//		USER_ERROR_CHECK(MStatus::kFailure,"mDblSetAttr: specified attribute is not of type doubleArray!");
//    }

    MObject attr;
    attr = plug.attribute(&stat);
    MFnAttribute attrFn(attr);
  	if (!attrFn.accepts( MFnData::kDoubleArray ))
    {
		USER_ERROR_CHECK(MStatus::kFailure,"mDblSetAttr: specified attribute is not of type doubleArray!");
    }

	return redoIt();
}

MStatus mDblSetAttr::redoIt() 
{ 
	MStatus stat;

	// get the current data on the plug, no need for testing any more - we know its all good from the doit
    MPlug plug;
  	stat = getPlugFromString(commandString(),mNodeAttr, plug);

	// store plug data for the undo
    MObject data;// = plug.attribute();
	stat = plug.getValue(data);	
    MFnDoubleArrayData daFn(data);
    stat = daFn.copyTo(mDblOld);

	// set the new value    
	MFnDoubleArrayData myFn;
    MObject myObj = myFn.create(mDblArg);
    stat = plug.setValue(myObj);
    USER_ERROR_CHECK(stat,"mDblSetAttr: error setting doubleArray plug!");    
        
    return MS::kSuccess; 
} 

MStatus mDblSetAttr::undoIt() 
{ 
    MPlug plug;
  	MStatus stat = getPlugFromString(commandString(),mNodeAttr, plug);

    MObject data;
	plug.getValue(data);    
    MFnDoubleArrayData daFn(data);
	daFn.set(mDblOld);
   
    plug.setValue(data);
	
    return MS::kSuccess; 
} 

bool mDblSetAttr::isUndoable() const 
{ 
    return true; 
} 

//************************************************************************************************//
/*
   Function: mVecSetAttr
   Set vector array on attribute

   Parameters:
		doubleArrayA - double array or vectors
        nodeAttr - node.attribute
        
   Returns:

*/
#define mel int mVecSetAttr(float $doubleArrayA[], string $nodeAttr);
#undef mel


CREATOR(mVecSetAttr)

MStatus mVecSetAttr::doIt( const MArgList& args )
{
    // get the arguments
    MDoubleArray dblA;
	MStatus stat = getDoubleArrayArg(args,0,dblA);    
	ERROR_FAIL(stat);
    
    // is it a vector array
    unsigned int count;
    stat = vecIsValid(dblA, count);
    ERROR_FAIL(stat)
    mVecArg = doubleArrayToVectorArray(dblA);

	MString nodeAttr;
	stat = getStringArg(args,1,mNodeAttr);
    ERROR_FAIL(stat);
    
    // get the plug to the nodeAttr
    MPlug plug;
  	stat = getPlugFromString(commandString(),mNodeAttr, plug);
	ERROR_FAIL(stat);      
    
	// is the plug a vector array?
    MObject attr;
    attr = plug.attribute(&stat);
    MFnAttribute attrFn(attr);
  	if (!attrFn.accepts( MFnData::kVectorArray ))
    {
		USER_ERROR_CHECK(MStatus::kFailure,"mVecSetAttr: specified attribute is not of type vectorArray!");
    }


	return redoIt();
}

MStatus mVecSetAttr::redoIt() 
{ 
	MStatus stat;

	// get the current data on the plug, no need for testing any more - we know its all good from the doit
    MPlug plug;
  	stat = getPlugFromString(commandString(),mNodeAttr, plug);

	// store plug data for the undo
    MObject data;
	stat = plug.getValue(data);	
    MFnVectorArrayData daFn(data);
    stat = daFn.copyTo(mVecOld);

	// set the new value    
	MFnVectorArrayData myFn;
    MObject myObj = myFn.create(mVecArg);
    stat = plug.setValue(myObj);
    USER_ERROR_CHECK(stat,"mDblSetAttr: error setting doubleArray plug!");    
    
        
    return MS::kSuccess; 
} 

MStatus mVecSetAttr::undoIt() 
{ 
    MPlug plug;
  	MStatus stat = getPlugFromString(commandString(),mNodeAttr, plug);

    MObject data;
	plug.getValue(data);    
    MFnVectorArrayData daFn(data);
	daFn.set(mVecOld);
   
    plug.setValue(data);
	
    return MS::kSuccess; 
} 

bool mVecSetAttr::isUndoable() const 
{ 
    return true; 
} 


}// namespace

