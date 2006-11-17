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


// Title: Usage
//
// About:
// This is a quick introduction on how to use melfunctions. I am assuming you are familiar with mel scripting and basic cg related math. 
//
// Double Arrays:
// Nearly all math melfunctions operate on arrays. An array can either be created manually  
//
// float $dArray[] = {1.0, 2.0, 3.0, 5.0}; - .
//
// or by using the melfunction creator functions
//
// float $dArray[] = mDblCreate(3,5); >> Result: 5 5 5 - . 
//
// Another way to get to an array is from an array plug using mel
//
// float $dArray[] = `getAttr particleShape1.mass` - .
//
// Now that you have those arrays you can use them as arguments for functions.
//
// float $dArrayA[] = {0.0, 1.0, 2.0, 3.2}; - .
// float $dArrayB[] = {5.0, 6.0, -2.0, 0.2}; - .
// mDblAdd($dArrayA, $dArrayB); >> 5.0 7.0 0.0 3.4 - .
// 
// All array arguments to a melfunction have to be either the same size or be single elements. If you are providing just a single value 
// this value will be used in the computation with all other array elements, if you are providing equal sized arrays the operations will
// be done elementwise. The result of the operation will always be the size of the biggest array.
//
// mDblAdd(1.0, 2.0); >> 3 - .
// mDblAdd({1.0, 5.0, 8.0, -4.0}, 2.0); >> 3.0 7.0 10.0 -2.0 - .
// mDblAdd(1.0, {5.0, 8.0}); >> 6.0 9.0 - .
// mDblAdd({1.0}, {5.0, 8.0}); >> 6.0 9.0 - .
// mDblAdd({1.0, 2.0}, {5.0, 8.0}); >> 6.0 9.0 - .
// mDblAdd({1.0, 2.0, 3.0}, {5.0, 8.0}); >> ERROR - .
//
// The above limitations are true for all of the mathematical operations, however special wrangling functions allow different sized
// array inputs. Use the "append" function to append an element / array to an already existing array.
//
// mDblAppend ({0.0, 11.0}, {5.0, 6.0, 10.0}); >>  0.0 11.0 5.0 6.0 10.0 - .
//
// Use the "get" function to extract elements from an array, the second element are int indices of the respective elements of the array to get.
// Note you can use a third argument which will be used as a substition in case you specifiy an invalid index. It has to be the same size than
// the index array (or a single element)
//
// mDblGet ({1.0, 2.0, 3.0, 4.0}, 0); >> 1.0 - .
// mDblGet ({1.0, 2.0, 3.0, 4.0}, {0,3}); >> 1.0 4.0 - .
// mDblGet ({1.0, 2.0, 3.0, 4.0}, -1); >> ERROR, INVALID INDEX - .
// mDblGet ({1.0, 2.0, 3.0, 4.0}, -1, 10.0); >> 10.0 - .
// mDblGet ({1.0, 2.0, 3.0, 4.0}, {0,1,9}, 10.0); >> 1.0 2.0 10.0 - .
// mDblGet ({1.0, 2.0, 3.0, 4.0}, {0,1,9}, {10.0, 11.0, 12.0} ); >> 1.0 2.0 12.0 - .
// mDblGet ({1.0, 2.0, 3.0, 4.0}, {-1,1,9}, {10.0, 11.0, 12.0} ); >> 10.0 2.0 12.0 - .
//
// Use the "set" functions to set specific array elements. Note that the elements to set have to be the same size (or single elements) than 
// the index array.
//
// mDblSet ({1.0, 2.0, 3.0, 4.0}, 0, 10.0); >> 10.0  2.0 3.0 4.0 - .
// mDblSet ({1.0, 2.0, 3.0, 4.0}, 0, 10.0); >> 10.0  2.0 3.0 4.0 - .
// mDblSet ({1.0, 2.0, 3.0, 4.0}, {0 ,2}, 10.0); >> 10.0  2.0 10.0 4.0 - .
// mDblSet ({1.0, 2.0, 3.0, 4.0}, {0 ,2}, {10.0, 15.0}); >> 10.0  2.0 15.0 4.0 - .
// mDblSet ({1.0, 2.0, 3.0, 4.0}, {1,2,3}, {10.0, 15.0}); >> ERROR - .
// mDblSet ({1.0, 2.0, 3.0, 4.0}, 4 , {10.0, 15.0}); >> ERROR, INDEX OUT OF BOUNDS - .
//
// Vector and Matrix Arrays:
// Mel is fairly limited when it comes to data types- especially when it comes to more complex ones
// like arrays of vectors or matrices. When you request the data of a vector array from a plug
// (eg. by using the "getAttr" command) what you will get back is a double array made of the components
// of the individula vector elements. To work with this, you traditionally have to decode the double
// array back into a vector array.
//
// float $myPos[] = `getAttr particleShape1.worldPosition`; - .
// vector $element5 = <<$myPos[5*3+0],$myPos[5*3+1],$myPos[5*3+2]>>; - .
//
// So a vectorArray is realy just a doubleArray that can be devided by the number of vector components: 3.
// Vector arrays in melfunctions follow this pattern, they don't have a special vectorArray type but are just doubleArrays, too.
// So as double and vector arrays have the same type, distinguishing them is up to the user. This can for example be done by naming
// the variables according to their type.
//
// float $vPosition[] = {1.0, 0.0, 2.0, 0.0, 1.0, 0.0}; - a two element vector array
// float $dMass[] = {1.0, 0.0, 2.0, 0.0, 1.0, 0.0}; - a six element double array
//
// All the functions described above work the same for vector and matrix arrays.
//
// mVecCreate(); >> 0 0 0 - .
// mVecCreate(3); >> 0 0 0 0 0 0 0 0 0 - .
// mVecCreate(3, {1,2,3}); >> 1 2 3 1 2 3 1 2 3 - .
//
// Mathematical functions.
//
// mVecAdd({0,1,2}, {10,11,12}); >> 10 12 14 - .
// mVecAdd({0,1,2}, {10,11,12}); >> 10 12 14 - .
// mVecAdd({0,1,2,3,4,5}, {10,11,12}); >> 10 12 14 13 15 17 - .
// mVecAdd({0,1,2,3,4,5}, {10,11,12,13 14,15}); >> 10 12 14 16 18 20 - .
// mVecAdd({0,1,2,3,4,5,6,7,8}, {10,11,12,13 14,15}); >> ERROR, SIZE MISMATCH - .
//
// Wrangling.
//
// mVecAppend({6,7,8}, {2,3}); >> ERROR, 2ND ARGUMENT NOT A VECTOR ARRAY - .
// mVecAppend({6,7,8}, {2,3,4,1,2,3}); >> 6 7 8 2 3 4 1 2 3 - .
//
// mVecGet({0,1,2,3,4,5,6,7,8},1); >> 3 4 5 - .
// mVecGet({0,1,2,3,4,5,6,7,8},{1,0}); >> 3 4 5 0 1 2 - .
//
// mVecSet({0,1,2,3,4,5,6,7,8},1,{20,21,22}); >> 0 1 2 20 21 22 6 7 8 - .
//
// Note that the mel data type vector and matrix are not supported when returning values (though they are fine as arguments to functions).
//
// This should give you a brief overview of the concepts behind melfunctions, for more infos consult the individual commands documentation.
