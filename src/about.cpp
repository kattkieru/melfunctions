// Title: About
//
// melfunctions:
// mal-, mael- #aehem# melfunctions is a plugin for Alias Maya that gives you access to many new mel command functions
// most of which are only very hard to replicate purely in MEL. A lot of the commands are actually just wrapper
// around functionality that is already in the Maya API. melfunctions gives the user access to this
// hidden power formerly only available to C++ developers. Hopefully it will make it easier and faster to create more advanced 
// software using Maya's embedded scripting language.
//
// If you want to compile melfunctions it for your favourite OS give me a shout and I'm sure we can work something out.
//
// Installation:
// Installation is easy, just unzip the package and place the file "melfunctions.mll" (.so on linux) in your
// Maya plugin directory. Then load it using the plugin manager. Alternativly use the "loadPlugin("/pathToMelfunctions/melfunctions.mll")" mel command.
//
// Loading the plugin:
// After the plugin is succesfully loaded you can start using it in your scripts just like any other MEL command.
// There is howeever one thing to be aware of. *IMPORTANT*: MEL files that were sourced before the plugin was loaded
// won't know about the commands available through the plugin. So if you are using the commands in your scripts
// make sure the plugin is loaded prior to sourcing them. This can be achieved by:
// - setting the load settings in the plugin window to "autoload"
// - resourcing the scripts after loading the plugin
// - using the "eval" directive in your scripts when using the plugin commands (which will execute them in another new scope which knows about the commands)
// - have a small control script which a) loads the plugin and then b) sources your actual script prior to executing it
//
// Version History:
// - *0.2*, 02/09/06, changed matrix functions to work with arrays, added many double and vector array commands,
// due to various changes backwardscompatibility is not guaranteed 
// - *0.1*, 16/02/06, initial release with a basic set of 15 matrix functions 
//
// Disclaimer:
// melfunctions, Copyright (C) 2006 Carsten Kolve, <http://www.kolve.com>, carsten@kolve.com
// The standard disclaimer applies, this software comes without any warranties, there is not even a guarantee it works. Use it at your own risk!
// 
// Please read carefully as this is important: melfunctions is NOT free software. melfunctions is *charityware* <http://en.wikipedia.org/wiki/Charityware>, this mean that
// in case you want to use this software commercially you are asked to donate an amount of money to a charity. If you are using
// the software for private or educational purposes then there is no obligation to donate any money, but it certainly would be appreciated!
// 
// If you are not sure which charity to donate to, I recommend the *Doctors Without Borders / Medicines Sans Frontieres* <http://www.msf.org/>. 
// Here is a webpage with a list of their national offices <http://www.msf.org/msfinternational/donations/>. It's best to donate to your national office as they can provide you with tax receipts.
// As to the question how much to donate, well that is entirely up to you - please do however take into account the amount of money it would have cost you to develop a similar solution inhouse, 
// how much money you save by using it, all donations are tax deductible and you are supporting a good cause!  
// Please let me know if you do make a donation- I'd like to know if something like this actually works.




