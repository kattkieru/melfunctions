// Title: About
//
// melfunctions:
// mal-, mael- #aehem# melfunctions is a plugin for Alias Maya that gives you access to many new mel command functions
// most of which are only very hard to replicate purely in MEL. A lot of the commands are actually just wrapper
// around functionality that is already in the Maya API. melfunctions gives the user access to this
// hidden power formerly only available to C++ developers. Hopefully it will make it easier and faster to create more advanced 
// software using Maya's embedded scripting language.
//
//
// Installation:
// Installation is easy, just unzip the package and place the file "melfunctions.mll" (.so on linux) in your
// Maya plugin directory. Then load it using the plugin manager. Alternativly use the "loadPlugin("/pathToMelfunctions/melfunctions.mll")" mel command.
//
// Loading the plugin:
// After the plugin is succesfully loaded you can start using it in your scripts just like any other MEL command.
// There is however one thing to be aware of. *IMPORTANT*: MEL files that were sourced before the plugin was loaded
// won't know about the commands available through the plugin. So if you are using the commands in your scripts
// make sure the plugin is loaded prior to sourcing them. This can be achieved by:
// - setting the load settings in the plugin window to "autoload"
// - resourcing the scripts after loading the plugin
// - using the "eval" directive in your scripts when using the plugin commands (which will execute them in another new scope which knows about the commands)
// - have a small control script which a) loads the plugin and then b) sources your actual script prior to executing it
//
// Version History:
// - *0.7*, 07/12/06, added 2 turbulence functions to the noise set, conversion from signed to unsigned noise values, boolean logic functions and component wise addition of vectors and doubles, reorganized the docs
// - *0.6*, 30/11/06, added 5 new noise functions and a way of creating a vectorArray from its components
// - *0.5*, 28/11/06, added mAnimCurveInfo, added FieldOfView lookup support to mNeighbourInfo
// - *0.4*, 22/11/06, random functions, sampling of 2d shader, information about instancer, get and set array attributes etc.
// - *0.3*, 10/11/06, added basic uv array support, various bugfixes, more dbl and vec functions, nearest neighbour lookup and mesh vertex informations
// - *0.2*, 02/09/06, changed matrix functions to work with arrays, added many double and vector array commands,
// due to various changes backwardscompatibility is not guaranteed 
// - *0.1*, 16/02/06, initial release with a basic set of 15 matrix functions 
//
// Source Code:
// The subversion directory to the source code is located here: <http://code.google.com/p/melfunctions/> , feel free to check out the code
// look at how it works and make contributions to it! If you recompiled a version for a new platform, please send it to me so I can include 
// it in the package of compiled versions.
//
// Disclaimer:
// melfunctions, Copyright (C) 2006 Carsten Kolve, <http://www.kolve.com>, carsten@kolve.com
// Licensed under the GNU General Public License
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// You should have received a copy of the GNU General Public License along with this program; if not, 
// visit <http://www.gnu.org/licenses/gpl.txt> or write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
// MA 02111-1307 USA.
//
// If you are finding this software useful please consider donating some money to a charity of your choice.
// If you are not sure which charity to donate to, I recommend the *Doctors Without Borders / Medicines Sans Frontieres* <http://www.msf.org/>. 
// Here is a webpage with a list of their national offices <http://www.msf.org/msfinternational/donations/>. It's best to donate to your national office as they can provide you with tax receipts.
// As to the question how much to donate, well that is entirely up to you - please do however take into account the amount of money it would have cost you to develop a similar solution inhouse, 
// how much money you save by using it, all donations are tax deductible and you are supporting a good cause!  
// Please let me know if you do make a donation- I'd like to know if something like this actually works.




