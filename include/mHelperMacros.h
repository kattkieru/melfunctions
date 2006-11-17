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


#ifndef __mHelperMacros_h__
#define __mHelperMacros_h__

#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MArgList.h>


#define SYS_ERROR_CHECK(stat,msg)		\
	if ( MS::kSuccess != stat ) \
	{	\
		cerr << "\nERROR: "<<__FILE__<<", "<<__LINE__<<", "<<msg;	\
		MGlobal::displayError(msg); \
		return MS::kFailure;		\
	}

#define USER_ERROR_CHECK(stat,msg)		\
	if ( MS::kSuccess != stat ) \
	{	\
		MGlobal::displayError(msg); \
		return MS::kFailure;		\
	}

#endif

/*#define ERROR_ARG(stat,arg)		\
	if ( MS::kSuccess != stat ) \
	{	\
		MString err = "execution of "+MPxCommand::commandString()+" failed,  argument "#arg" invalid!";\
		MGlobal::displayError(err); \
		return MS::kFailure;		\
	}

#define ERROR_FAIL(stat)		\
	if ( MS::kSuccess != stat ) \
	{	\
		MString err = "execution of "+MPxCommand::commandString()+" failed!";\
		MGlobal::displayError(err); \
		return MS::kFailure;		\
	}
*/

#define ERROR_ARG(stat,arg)		\
	if ( MS::kSuccess != stat ) \
	{	\
		MString err = "execution failed,  argument "#arg" invalid!";\
		MGlobal::displayError(err); \
		return MS::kFailure;		\
	}

#define ERROR_FAIL(stat)		\
	if ( MS::kSuccess != stat ) \
	{	\
		MString err = "execution failed!";\
		MGlobal::displayError(err); \
		return MS::kFailure;		\
	}

#define ERROR_ARG_FAIL(stat)		\
	if ( MS::kSuccess != stat ) \
	{	\
		MString err = "error parsing arguments!";\
		MGlobal::displayError(err); \
		return MS::kFailure;		\
	}

    
#define ERROR_MSG(msg)		\
	MGlobal::displayError(msg); \
	return MS::kFailure;		



#define DECLARE_COMMAND(command)\
	class command : public MPxCommand \
	{\
		public:\
						command();\
			MStatus		doIt( const MArgList& );\
			static		void* creator();\
	};

#define CREATOR(command)\
	command::command(){setCommandString(#command);}\
	void* command::creator() { return new command();}


// small helper macros to register and deregister commands
#define REGISTER_COMMAND(namespace,cmd)\
	status = plugin.registerCommand( #cmd, namespace::cmd::creator );\
	SYS_ERROR_CHECK(status, "registering command "#cmd" failed!")

#define DEREGISTER_COMMAND(cmd)\
	status = plugin.deregisterCommand( #cmd);\
	SYS_ERROR_CHECK(status, "deregistering command "#cmd" failed!")
    
