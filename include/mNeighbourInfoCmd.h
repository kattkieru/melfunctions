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


#ifndef _mNeighbourInfoCmd_h_
#define _mNeighbourInfoCmd_h_

//#define KDTREE_DEFINE_OSTREAM_OPERATORS
#include <kdtree++/kdtree.hpp>

#include <maya/MVectorArray.h> 
#include <maya/MDoubleArray.h> 
#include <map> 
#include <string> 
#include <iostream> 


namespace melfunctions
{

static const char* COMMAND_NAME = "mNeighbourInfo";

static const char* HELP_FLAG = "h";
static const char* HELP_FLAG_LONG = "help";

// things that can be edited and queried
static const char* LOOKUP_RADIUS_FLAG = "lr";
static const char* LOOKUP_RADIUS_FLAG_LONG = "lookupRadius";

static const char* LOOKUP_ANGLE_FLAG = "la";
static const char* LOOKUP_ANGLE_FLAG_LONG = "lookupAngle";

static const char* LOOKUP_DIRECTION_FLAG = "ld";
static const char* LOOKUP_DIRECTION_FLAG_LONG = "lookupDirection";

static const char* LOOKUP_POINT_FLAG = "lp";
static const char* LOOKUP_POINT_FLAG_LONG = "lookupPoint";

static const char* NEIGHBOUR_POINT_FLAG = "np";
static const char* NEIGHBOUR_POINT_FLAG_LONG = "neighbourPoint";


// delete the named info object
static const char* DELETE_FLAG = "d";
static const char* DELETE_FLAG_LONG = "delete";

// delete the named info object
static const char* DELETE_ALL_FLAG = "da";
static const char* DELETE_ALL_FLAG_LONG = "deleteAll";


// get a list of all created objects
static const char* LIST_FLAG = "l";
static const char* LIST_FLAG_LONG = "list";

// nearest in range querys
static const char* POSITION_FLAG = "pos";
static const char* POSITION_FLAG_LONG = "position";

static const char*  NEAREST_IN_RANGE_FLAG = "nir";
static const char*  NEAREST_IN_RANGE_FLAG_LONG = "nearestInRange";

static const char*  FURTHEST_IN_RANGE_FLAG = "fir";
static const char*  FURTHEST_IN_RANGE_FLAG_LONG = "furthestInRange";

static const char*  RANDOM_IN_RANGE_FLAG = "rir";
static const char*  RANDOM_IN_RANGE_FLAG_LONG = "randomInRange";

static const char*  COUNT_IN_RANGE_FLAG = "cir";
static const char*  COUNT_IN_RANGE_FLAG_LONG = "countInRange";

static const char*  ALL_IN_RANGE_FLAG = "air";
static const char*  ALL_IN_RANGE_FLAG_LONG = "allInRange";

static const char*  EXCLUDE_SAME_FLAG = "xcs";
static const char*  EXCLUDE_SAME_FLAG_LONG = "excludeSame";


#define CMD_ACTION_CREATE 0
#define CMD_ACTION_EDIT 1
#define CMD_ACTION_QUERY 2

#define CMD_CREATE_CREATE 0
#define CMD_CREATE_DELETE_ALL 1
#define CMD_CREATE_LIST 2

#define CMD_EDIT_NONE 0
#define CMD_EDIT_DELETE 1

#define CMD_QUERY_NONE 0
#define CMD_QUERY_NEAREST_IN_RANGE 1
#define CMD_QUERY_FURTHEST_IN_RANGE 2
#define CMD_QUERY_RANDOM_IN_RANGE_FLAG 3
#define CMD_QUERY_ALL_IN_RANGE_FLAG 4
#define CMD_QUERY_COUNT_IN_RANGE_FLAG 5

struct triplet 
{
  typedef double value_type;

  inline value_type operator[](size_t const N) const 
  { 
  	return d[N]; 
  }
  
  value_type d[3];
  double index;
};

inline bool operator==(triplet const& A, triplet const& B) 
{
  return A.d[0] == B.d[0] && A.d[1] == B.d[1] && A.d[2] == B.d[2];
}


//std::ostream& operator<<(std::ostream& out, triplet const& T)
//{
//  return out << '(' << T.d[0] << ',' << T.d[1] << ',' << T.d[2] << ')';
//}



typedef KDTree::KDTree<3, triplet> mNeighbourInfoStruct;

/*
struct mNeighbourInfoStruct
{
//	MDoubleArray neighbourPoint; // treated as a vector array
	tree_type kdtree;    
    
};
*/

typedef std::map < std::string, mNeighbourInfoStruct> mNeighbourInfoMapType;
typedef	std::map < std::string, mNeighbourInfoStruct>::iterator mNeighbourInfoIterType;

class mNeighbourInfo : public MPxCommand 
{
		public:
						mNeighbourInfo();
                        ~mNeighbourInfo(){};
			
			MStatus		doIt( const MArgList& );
			MStatus		doCreate();
			MStatus		doEdit();
			MStatus		doQuery();

    		static		void* creator();
            
            MStatus     parseArgs( const MArgList& args );
            void        help() const;
            
        private:            
        
			std::string generateNeighbourInfoObjectName();
			MStatus		updateNeighbourInfoObject(mNeighbourInfoStruct &myNIObject);            
			MStatus		getNeighbourInfoObject(const MString name, mNeighbourInfoStruct &myNIObject);
			MStatus		fillNeighbourInfoObject(mNeighbourInfoStruct &myNIObject, MDoubleArray points);            
            
			void 		getNeighboursInFOV (const mNeighbourInfoStruct &myNI,
											 const MVector &lookupPoint,
                                             const double lookupRadius,
   											 const MVector &lookupDirection,                                                                                         
                                             const double lookupAngle,
                                             MVectorArray &nearPoint,
                                             MDoubleArray &nearIndex,
                                             MDoubleArray &nearDistance,
                                             int &count) ;

			void 		getNeighboursInRadius (const mNeighbourInfoStruct &myNI,
											 const MVector &lookupPoint,
                                             const double lookupRadius,
                                             MVectorArray &nearPoint,
                                             MDoubleArray &nearIndex,
                                             MDoubleArray &nearDistance,
                                             int &count) ;

			void	getNeighbours(	const mNeighbourInfoStruct &myNI,
	       					const unsigned int incLP, const unsigned int incLR, 
							const unsigned int incLD, const unsigned int incLA,         
                            int &iterLP, int &iterLR,int &iterLD,int &iterLA,
                            MVectorArray &nearPoint, MDoubleArray &nearIndex,
                            MDoubleArray &nearDistanceSqr, int &nearCount)  ;       
                                                                         
                                                         
            MStatus		doQueryNearestInRange(const mNeighbourInfoStruct &myNI, const unsigned int incLP, 
            								  const unsigned int incLR, const unsigned int incLD, const unsigned int incLA, const unsigned int count,MDoubleArray &result);
            MStatus		doQueryFurthestInRange(const mNeighbourInfoStruct &myNI, const unsigned int incLP, 
            								  const unsigned int incLR, const unsigned int incLD, const unsigned int incLA, const unsigned int count,MDoubleArray &result);
            MStatus		doQueryRandomInRange(const mNeighbourInfoStruct &myNI, const unsigned int incLP, 
            								  const unsigned int incLR, const unsigned int incLD, const unsigned int incLA, const unsigned int count,MDoubleArray &result);
            MStatus		doQueryAllInRange(const mNeighbourInfoStruct &myNI, const unsigned int incLP, 
            								  const unsigned int incLR, const unsigned int incLD, const unsigned int incLA, const unsigned int count,MDoubleArray &result);
            MStatus		doQueryCountInRange(const mNeighbourInfoStruct &myNI, const unsigned int incLP, 
            								  const unsigned int incLR, const unsigned int incLD, const unsigned int incLA, const unsigned int count,MDoubleArray &result);


            MStatus		deleteNeighbourInfoObject();
		    MStatus     deleteAllNeighbourInfoObjects();
            MStatus		listAllNeighbourInfoObjects();
                        
            short           mCmdAction;
			short			mQueryAction;
			short			mEditAction; 
			short			mCreateAction;                       

            bool			mQueryPosition;
            bool			mExcludeSame;            

            bool            mHelpFlagSet;
            MString			mNIObject;

            bool            mLookupRadiusFlagSet;            
            MDoubleArray    mLookupRadius;
            
            bool            mLookupPointFlagSet;
            MDoubleArray    mLookupPoint;            
            
            bool            mLookupFOV;
            bool            mLookupAngleFlagSet;
            MDoubleArray    mLookupAngle;            

            bool            mLookupDirectionFlagSet;
            MDoubleArray    mLookupDirection;            

            bool            mNeighbourPointFlagSet;
            MDoubleArray    mNeighbourPoint;     

			// map storing all the neighbour info objects
			static	mNeighbourInfoMapType mNIMap;
            static  int mNeighbourInfoIndex;
};


}//end namespace


#endif

/*
float $np[] = {0,0,0, 0,0,1, 0,0,2 ,0,0,3};
float $lr[] = {1};
float $lp[] = {0,0,0};

string $my =`mNeighbourInfo -np $np`;
mNeighbourInfo 



*/
