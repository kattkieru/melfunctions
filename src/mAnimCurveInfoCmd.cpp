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



// Title: mAnimCurveInfo Command
//

#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnAnimCurve.h>


#include "../include/mHelperMacros.h"
#include "../include/mAnimCurveInfoCmd.h"
#include "../include/mHelperFunctions.h"


/*
   Function: mAnimCurveInfo

   This command allows you to sample an animation curve at specified values.

   Parameters:

   value     -  [float[]]   Mandatory: float array of sampling values
   animCurve       - [string]       Manadatory: Name of animationCurve to sample. Alternativly define an attribute driven by an animationCurve

   Returns:

      The result is a float array of sampled values

   Examples:
   
   	  Animate the translateX of a locator
      
   	  mAnimCurveInfo {0,1,2,3,4} locator1.translateX;
      
      mAnimCurveInfo {0,1,2,3,4} locator1_translateX;

*/

#define mel int mAnimCurveInfo(float $doubleArrayA[], string $nodeAttr);
#undef mel


namespace melfunctions
{



CREATOR(mAnimCurveInfo)

MStatus mAnimCurveInfo::doIt( const MArgList& args )
{
    // get the arguments
	MStatus stat = getDoubleArrayArg(args,0,mDblArg);    
	ERROR_FAIL(stat);

	MString nodeAttr;
	stat = getStringArg(args,1,mAnimCurveName);
    ERROR_FAIL(stat);
    
          
	return redoIt();
}


MStatus mAnimCurveInfo::redoIt() 
{ 
    // get the selection list
	MSelectionList sList;
	MStatus stat = getSelectionListFromString(commandString(),mAnimCurveName,sList);
    ERROR_FAIL(stat);
    
    // now get the plug/object
    MObject animCurveObj;
    MPlug animCurvePlug;
        
    
	// now that we've got the fn, sample it
    MDoubleArray result(mDblArg.length());

    
    // try to get the plug
    stat =  sList.getPlug(0,animCurvePlug);

   	MFnAnimCurve animCurveFn(animCurvePlug,&stat);
    if (!stat.error())
    {
		for (int i=0;i<mDblArg.length();i++)
		{
    		result[i] = animCurveFn.evaluate(mDblArg[i],&stat);
//		    USER_ERROR_CHECK(stat,"mAnimCurveInfo:can't sample fn !");
		}
	}
    else
    {
    	// try to get the obj
    	stat = sList.getDependNode(0,animCurveObj);
        if (stat.error())
        {
        	USER_ERROR_CHECK(MStatus::kFailure,"mAnimCurveInfo: specified nodeAttr/node does not exist!");
        }
        else
        {
        	MFnAnimCurve animCurveFn(animCurveObj,&stat);
            USER_ERROR_CHECK(stat,"mAnimCurveInfo:can't sample specified object, does it have an animCurve attached?");
		
            for (int i=0;i<mDblArg.length();i++)
		    {
    			result[i] = animCurveFn.evaluate(mDblArg[i],&stat);
//		        USER_ERROR_CHECK(stat,"mAnimCurveInfo:can't sample fn !");
		    }
        }    	
    }
    
    setResult(result);

    return MS::kSuccess; 
} 

	

} // end namespace

