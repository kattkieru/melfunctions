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
 
#include "Noise.h"
#include <math.h>


//************************************************************************************************
//************************************************************************************************
//                                       LOOKUP TABLES
//************************************************************************************************
//************************************************************************************************

// for perlin
//unsigned char g_hash[256]= {
unsigned char hash[512]= {
	151,160,137,91,90,15,131, 13,201,95,96,53,194,233,7,
	225,140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,
	148,247,120,234,75, 0,26,197, 62,94,252,219,203,117,
	35,11,32,57,177,33, 88,237,149,56,87,174,20,125,136,
	171,168, 68,175,74,165,71,134,139,48,27,166, 77,146,
	158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54, 65,25,63,161, 1,216,80,
	73,209,76,132,187,208,89,18,169,200,196,135,130,116,
	188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
	250,124,123, 5,202,38,147,118,126,255,82,85,212,207,
	206, 59,227, 47,16,58,17,182,189,28,42, 223,183,170,
	213,119,248,152,2,44,154,163,70,221,153,101,155,167, 
	43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,
	178,185, 112,104,218,246,97,228, 251,34,242,193,238,
	210,144,12,191,179,162,241,81,51,145,235,249,14,239,
	107, 49,192,214, 31,181,199,106,157,184, 84,204,176,
	115,121,50,45,127, 4,150,254,138,236,205,93,222,114,
	67, 29,24, 72,243,141,128,195,78, 66,215,61,156,180, 
	151,160,137,91,90,15,131, 13,201,95,96,53,194,233,7,
	225,140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,
	148,247,120,234,75, 0,26,197, 62,94,252,219,203,117,
	35,11,32,57,177,33, 88,237,149,56,87,174,20,125,136,
	171,168, 68,175,74,165,71,134,139,48,27,166, 77,146,
	158,231,83,111,229,122,60,211,133,230,220,105,92,41,
	55,46,245,40,244,102,143,54, 65,25,63,161, 1,216,80,
	73,209,76,132,187,208,89,18,169,200,196,135,130,116,
	188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
	250,124,123, 5,202,38,147,118,126,255,82,85,212,207,
	206, 59,227, 47,16,58,17,182,189,28,42, 223,183,170,
	213,119,248,152,2,44,154,163,70,221,153,101,155,167, 
	43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,
	178,185, 112,104,218,246,97,228, 251,34,242,193,238,
	210,144,12,191,179,162,241,81,51,145,235,249,14,239,
	107, 49,192,214, 31,181,199,106,157,184, 84,204,176,
	115,121,50,45,127, 4,150,254,138,236,205,93,222,114,
	67, 29,24, 72,243,141,128,195,78, 66,215,61,156,180 
 };   
 
 /* needed for voronoi */
#define HASHPNT(x,y,z) hashpntf+3*hash[ (hash[ (hash[(z) & 255]+(y)) & 255]+(x)) & 255]
static float hashpntf[768] = {
0.536902, 0.020915, 0.501445, 0.216316, 0.517036, 0.822466, 0.965315,
0.377313, 0.678764, 0.744545, 0.097731, 0.396357, 0.247202, 0.520897,
0.613396, 0.542124, 0.146813, 0.255489, 0.810868, 0.638641, 0.980742,
0.292316, 0.357948, 0.114382, 0.861377, 0.629634, 0.722530, 0.714103,
0.048549, 0.075668, 0.564920, 0.162026, 0.054466, 0.411738, 0.156897,
0.887657, 0.599368, 0.074249, 0.170277, 0.225799, 0.393154, 0.301348,
0.057434, 0.293849, 0.442745, 0.150002, 0.398732, 0.184582, 0.915200,
0.630984, 0.974040, 0.117228, 0.795520, 0.763238, 0.158982, 0.616211,
0.250825, 0.906539, 0.316874, 0.676205, 0.234720, 0.667673, 0.792225,
0.273671, 0.119363, 0.199131, 0.856716, 0.828554, 0.900718, 0.705960,
0.635923, 0.989433, 0.027261, 0.283507, 0.113426, 0.388115, 0.900176,
0.637741, 0.438802, 0.715490, 0.043692, 0.202640, 0.378325, 0.450325,
0.471832, 0.147803, 0.906899, 0.524178, 0.784981, 0.051483, 0.893369,
0.596895, 0.275635, 0.391483, 0.844673, 0.103061, 0.257322, 0.708390,
0.504091, 0.199517, 0.660339, 0.376071, 0.038880, 0.531293, 0.216116,
0.138672, 0.907737, 0.807994, 0.659582, 0.915264, 0.449075, 0.627128,
0.480173, 0.380942, 0.018843, 0.211808, 0.569701, 0.082294, 0.689488, 
0.573060, 0.593859, 0.216080, 0.373159, 0.108117, 0.595539, 0.021768, 
0.380297, 0.948125, 0.377833, 0.319699, 0.315249, 0.972805, 0.792270, 
0.445396, 0.845323, 0.372186, 0.096147, 0.689405, 0.423958, 0.055675, 
0.117940, 0.328456, 0.605808, 0.631768, 0.372170, 0.213723, 0.032700, 
0.447257, 0.440661, 0.728488, 0.299853, 0.148599, 0.649212, 0.498381,
0.049921, 0.496112, 0.607142, 0.562595, 0.990246, 0.739659, 0.108633, 
0.978156, 0.209814, 0.258436, 0.876021, 0.309260, 0.600673, 0.713597, 
0.576967, 0.641402, 0.853930, 0.029173, 0.418111, 0.581593, 0.008394, 
0.589904, 0.661574, 0.979326, 0.275724, 0.111109, 0.440472, 0.120839, 
0.521602, 0.648308, 0.284575, 0.204501, 0.153286, 0.822444, 0.300786, 
0.303906, 0.364717, 0.209038, 0.916831, 0.900245, 0.600685, 0.890002, 
0.581660, 0.431154, 0.705569, 0.551250, 0.417075, 0.403749, 0.696652, 
0.292652, 0.911372, 0.690922, 0.323718, 0.036773, 0.258976, 0.274265, 
0.225076, 0.628965, 0.351644, 0.065158, 0.080340, 0.467271, 0.130643,
0.385914, 0.919315, 0.253821, 0.966163, 0.017439, 0.392610, 0.478792, 
0.978185, 0.072691, 0.982009, 0.097987, 0.731533, 0.401233, 0.107570, 
0.349587, 0.479122, 0.700598, 0.481751, 0.788429, 0.706864, 0.120086, 
0.562691, 0.981797, 0.001223, 0.192120, 0.451543, 0.173092, 0.108960,
0.549594, 0.587892, 0.657534, 0.396365, 0.125153, 0.666420, 0.385823, 
0.890916, 0.436729, 0.128114, 0.369598, 0.759096, 0.044677, 0.904752, 
0.088052, 0.621148, 0.005047, 0.452331, 0.162032, 0.494238, 0.523349, 
0.741829, 0.698450, 0.452316, 0.563487, 0.819776, 0.492160, 0.004210, 
0.647158, 0.551475, 0.362995, 0.177937, 0.814722, 0.727729, 0.867126, 
0.997157, 0.108149, 0.085726, 0.796024, 0.665075, 0.362462, 0.323124,
0.043718, 0.042357, 0.315030, 0.328954, 0.870845, 0.683186, 0.467922, 
0.514894, 0.809971, 0.631979, 0.176571, 0.366320, 0.850621, 0.505555, 
0.749551, 0.750830, 0.401714, 0.481216, 0.438393, 0.508832, 0.867971, 
0.654581, 0.058204, 0.566454, 0.084124, 0.548539, 0.902690, 0.779571, 
0.562058, 0.048082, 0.863109, 0.079290, 0.713559, 0.783496, 0.265266, 
0.672089, 0.786939, 0.143048, 0.086196, 0.876129, 0.408708, 0.229312, 
0.629995, 0.206665, 0.207308, 0.710079, 0.341704, 0.264921, 0.028748, 
0.629222, 0.470173, 0.726228, 0.125243, 0.328249, 0.794187, 0.741340, 
0.489895, 0.189396, 0.724654, 0.092841, 0.039809, 0.860126, 0.247701, 
0.655331, 0.964121, 0.672536, 0.044522, 0.690567, 0.837238, 0.631520, 
0.953734, 0.352484, 0.289026, 0.034152, 0.852575, 0.098454, 0.795529, 
0.452181, 0.826159, 0.186993, 0.820725, 0.440328, 0.922137, 0.704592,
0.915437, 0.738183, 0.733461, 0.193798, 0.929213, 0.161390, 0.318547,
0.888751, 0.430968, 0.740837, 0.193544, 0.872253, 0.563074, 0.274598, 
0.347805, 0.666176, 0.449831, 0.800991, 0.588727, 0.052296, 0.714761, 
0.420620, 0.570325, 0.057550, 0.210888, 0.407312, 0.662848, 0.924382, 
0.895958, 0.775198, 0.688605, 0.025721, 0.301913, 0.791408, 0.500602, 
0.831984, 0.828509, 0.642093, 0.494174, 0.525880, 0.446365, 0.440063, 
0.763114, 0.630358, 0.223943, 0.333806, 0.906033, 0.498306, 0.241278,
0.427640, 0.772683, 0.198082, 0.225379, 0.503894, 0.436599, 0.016503, 
0.803725, 0.189878, 0.291095, 0.499114, 0.151573, 0.079031, 0.904618, 
0.708535, 0.273900, 0.067419, 0.317124, 0.936499, 0.716511, 0.543845, 
0.939909, 0.826574, 0.715090, 0.154864, 0.750150, 0.845808, 0.648108, 
0.556564, 0.644757, 0.140873, 0.799167, 0.632989, 0.444245, 0.471978, 
0.435910, 0.359793, 0.216241, 0.007633, 0.337236, 0.857863, 0.380247, 
0.092517, 0.799973, 0.919000, 0.296798, 0.096989, 0.854831, 0.165369, 
0.568475, 0.216855, 0.020457, 0.835511, 0.538039, 0.999742, 0.620226, 
0.244053, 0.060399, 0.323007, 0.294874, 0.988899, 0.384919, 0.735655, 
0.773428, 0.549776, 0.292882, 0.660611, 0.593507, 0.621118, 0.175269, 
0.682119, 0.794493, 0.868197, 0.632150, 0.807823, 0.509656, 0.482035, 
0.001780, 0.259126, 0.358002, 0.280263, 0.192985, 0.290367, 0.208111, 
0.917633, 0.114422, 0.925491, 0.981110, 0.255570, 0.974862, 0.016629,
0.552599, 0.575741, 0.612978, 0.615965, 0.803615, 0.772334, 0.089745, 
0.838812, 0.634542, 0.113709, 0.755832, 0.577589, 0.667489, 0.529834,
0.325660, 0.817597, 0.316557, 0.335093, 0.737363, 0.260951, 0.737073, 
0.049540, 0.735541, 0.988891, 0.299116, 0.147695, 0.417271, 0.940811, 
0.524160, 0.857968, 0.176403, 0.244835, 0.485759, 0.033353, 0.280319, 
0.750688, 0.755809, 0.924208, 0.095956, 0.962504, 0.275584, 0.173715,
0.942716, 0.706721, 0.078464, 0.576716, 0.804667, 0.559249, 0.900611, 
0.646904, 0.432111, 0.927885, 0.383277, 0.269973, 0.114244, 0.574867, 
0.150703, 0.241855, 0.272871, 0.199950, 0.079719, 0.868566, 0.962833, 
0.789122, 0.320025, 0.905554, 0.234876, 0.991356, 0.061913, 0.732911, 
0.785960, 0.874074, 0.069035, 0.658632, 0.309901, 0.023676, 0.791603, 
0.764661, 0.661278, 0.319583, 0.829650, 0.117091, 0.903124, 0.982098, 
0.161631, 0.193576, 0.670428, 0.857390, 0.003760, 0.572578, 0.222162, 
0.114551, 0.420118, 0.530404, 0.470682, 0.525527, 0.764281, 0.040596, 
0.443275, 0.501124, 0.816161, 0.417467, 0.332172, 0.447565, 0.614591, 
0.559246, 0.805295, 0.226342, 0.155065, 0.714630, 0.160925, 0.760001, 
0.453456, 0.093869, 0.406092, 0.264801, 0.720370, 0.743388, 0.373269, 
0.403098, 0.911923, 0.897249, 0.147038, 0.753037, 0.516093, 0.739257, 
0.175018, 0.045768, 0.735857, 0.801330, 0.927708, 0.240977, 0.591870,
0.921831, 0.540733, 0.149100, 0.423152, 0.806876, 0.397081, 0.061100, 
0.811630, 0.044899, 0.460915, 0.961202, 0.822098, 0.971524, 0.867608, 
0.773604, 0.226616, 0.686286, 0.926972, 0.411613, 0.267873, 0.081937, 
0.226124, 0.295664, 0.374594, 0.533240, 0.237876, 0.669629, 0.599083, 
0.513081, 0.878719, 0.201577, 0.721296, 0.495038, 0.079760, 0.965959,
0.233090, 0.052496, 0.714748, 0.887844, 0.308724, 0.972885, 0.723337,
0.453089, 0.914474, 0.704063, 0.823198, 0.834769, 0.906561, 0.919600,
0.100601, 0.307564, 0.901977, 0.468879, 0.265376, 0.885188, 0.683875,
0.868623, 0.081032, 0.466835, 0.199087, 0.663437, 0.812241, 0.311337,
0.821361, 0.356628, 0.898054, 0.160781, 0.222539, 0.714889, 0.490287,
0.984915, 0.951755, 0.964097, 0.641795, 0.815472, 0.852732, 0.862074,
0.051108, 0.440139, 0.323207, 0.517171, 0.562984, 0.115295, 0.743103,
0.977914, 0.337596, 0.440694, 0.535879, 0.959427, 0.351427, 0.704361,
0.010826, 0.131162, 0.577080, 0.349572, 0.774892, 0.425796, 0.072697,
0.500001, 0.267322, 0.909654, 0.206176, 0.223987, 0.937698, 0.323423,
0.117501, 0.490308, 0.474372, 0.689943, 0.168671, 0.719417, 0.188928,
0.330464, 0.265273, 0.446271, 0.171933, 0.176133, 0.474616, 0.140182,
0.114246, 0.905043, 0.713870, 0.555261, 0.951333
};

// for blender noise
float hashvectf[768]= {
0.33783,0.715698,-0.611206,-0.944031,-0.326599,-0.045624,-0.101074,-0.416443,-0.903503,0.799286,0.49411,-0.341949,-0.854645,0.518036,0.033936,0.42514,-0.437866,-0.792114,-0.358948,0.597046,0.717377,-0.985413,0.144714,0.089294,-0.601776,-0.33728,-0.723907,-0.449921,0.594513,0.666382,0.208313,-0.10791,
0.972076,0.575317,0.060425,0.815643,0.293365,-0.875702,-0.383453,0.293762,0.465759,0.834686,-0.846008,-0.233398,-0.47934,-0.115814,0.143036,-0.98291,0.204681,-0.949036,-0.239532,0.946716,-0.263947,0.184326,-0.235596,0.573822,0.784332,0.203705,-0.372253,-0.905487,0.756989,-0.651031,0.055298,0.497803,
0.814697,-0.297363,-0.16214,0.063995,-0.98468,-0.329254,0.834381,0.441925,0.703827,-0.527039,-0.476227,0.956421,0.266113,0.119781,0.480133,0.482849,0.7323,-0.18631,0.961212,-0.203125,-0.748474,-0.656921,-0.090393,-0.085052,-0.165253,0.982544,-0.76947,0.628174,-0.115234,0.383148,0.537659,0.751068,
0.616486,-0.668488,-0.415924,-0.259979,-0.630005,0.73175,0.570953,-0.087952,0.816223,-0.458008,0.023254,0.888611,-0.196167,0.976563,-0.088287,-0.263885,-0.69812,-0.665527,0.437134,-0.892273,-0.112793,-0.621674,-0.230438,0.748566,0.232422,0.900574,-0.367249,0.22229,-0.796143,0.562744,-0.665497,-0.73764,
0.11377,0.670135,0.704803,0.232605,0.895599,0.429749,-0.114655,-0.11557,-0.474243,0.872742,0.621826,0.604004,-0.498444,-0.832214,0.012756,0.55426,-0.702484,0.705994,-0.089661,-0.692017,0.649292,0.315399,-0.175995,-0.977997,0.111877,0.096954,-0.04953,0.994019,0.635284,-0.606689,-0.477783,-0.261261,
-0.607422,-0.750153,0.983276,0.165436,0.075958,-0.29837,0.404083,-0.864655,-0.638672,0.507721,0.578156,0.388214,0.412079,0.824249,0.556183,-0.208832,0.804352,0.778442,0.562012,0.27951,-0.616577,0.781921,-0.091522,0.196289,0.051056,0.979187,-0.121216,0.207153,-0.970734,-0.173401,-0.384735,0.906555,
0.161499,-0.723236,-0.671387,0.178497,-0.006226,-0.983887,-0.126038,0.15799,0.97934,0.830475,-0.024811,0.556458,-0.510132,-0.76944,0.384247,0.81424,0.200104,-0.544891,-0.112549,-0.393311,-0.912445,0.56189,0.152222,-0.813049,0.198914,-0.254517,-0.946381,-0.41217,0.690979,-0.593811,-0.407257,0.324524,
0.853668,-0.690186,0.366119,-0.624115,-0.428345,0.844147,-0.322296,-0.21228,-0.297546,-0.930756,-0.273071,0.516113,0.811798,0.928314,0.371643,0.007233,0.785828,-0.479218,-0.390778,-0.704895,0.058929,0.706818,0.173248,0.203583,0.963562,0.422211,-0.904297,-0.062469,-0.363312,-0.182465,0.913605,0.254028,
-0.552307,-0.793945,-0.28891,-0.765747,-0.574554,0.058319,0.291382,0.954803,0.946136,-0.303925,0.111267,-0.078156,0.443695,-0.892731,0.182098,0.89389,0.409515,-0.680298,-0.213318,0.701141,0.062469,0.848389,-0.525635,-0.72879,-0.641846,0.238342,-0.88089,0.427673,0.202637,-0.532501,-0.21405,0.818878,
0.948975,-0.305084,0.07962,0.925446,0.374664,0.055817,0.820923,0.565491,0.079102,0.25882,0.099792,-0.960724,-0.294617,0.910522,0.289978,0.137115,0.320038,-0.937408,-0.908386,0.345276,-0.235718,-0.936218,0.138763,0.322754,0.366577,0.925934,-0.090637,0.309296,-0.686829,-0.657684,0.66983,0.024445,
0.742065,-0.917999,-0.059113,-0.392059,0.365509,0.462158,-0.807922,0.083374,0.996399,-0.014801,0.593842,0.253143,-0.763672,0.974976,-0.165466,0.148285,0.918976,0.137299,0.369537,0.294952,0.694977,0.655731,0.943085,0.152618,-0.295319,0.58783,-0.598236,0.544495,0.203796,0.678223,0.705994,-0.478821,
-0.661011,0.577667,0.719055,-0.1698,-0.673828,-0.132172,-0.965332,0.225006,-0.981873,-0.14502,0.121979,0.763458,0.579742,0.284546,-0.893188,0.079681,0.442474,-0.795776,-0.523804,0.303802,0.734955,0.67804,-0.007446,0.15506,0.986267,-0.056183,0.258026,0.571503,-0.778931,-0.681549,-0.702087,-0.206116,
-0.96286,-0.177185,0.203613,-0.470978,-0.515106,0.716095,-0.740326,0.57135,0.354095,-0.56012,-0.824982,-0.074982,-0.507874,0.753204,0.417969,-0.503113,0.038147,0.863342,0.594025,0.673553,-0.439758,-0.119873,-0.005524,-0.992737,0.098267,-0.213776,0.971893,-0.615631,0.643951,0.454163,0.896851,-0.441071,
0.032166,-0.555023,0.750763,-0.358093,0.398773,0.304688,0.864929,-0.722961,0.303589,0.620544,-0.63559,-0.621948,-0.457306,-0.293243,0.072327,0.953278,-0.491638,0.661041,-0.566772,-0.304199,-0.572083,-0.761688,0.908081,-0.398956,0.127014,-0.523621,-0.549683,-0.650848,-0.932922,-0.19986,0.299408,0.099426,
0.140869,0.984985,-0.020325,-0.999756,-0.002319,0.952667,0.280853,-0.11615,-0.971893,0.082581,0.220337,0.65921,0.705292,-0.260651,0.733063,-0.175537,0.657043,-0.555206,0.429504,-0.712189,0.400421,-0.89859,0.179352,0.750885,-0.19696,0.630341,0.785675,-0.569336,0.241821,-0.058899,-0.464111,0.883789,
0.129608,-0.94519,0.299622,-0.357819,0.907654,0.219238,-0.842133,-0.439117,-0.312927,-0.313477,0.84433,0.434479,-0.241211,0.053253,0.968994,0.063873,0.823273,0.563965,0.476288,0.862152,-0.172516,0.620941,-0.298126,0.724915,0.25238,-0.749359,-0.612122,-0.577545,0.386566,0.718994,-0.406342,-0.737976,
0.538696,0.04718,0.556305,0.82959,-0.802856,0.587463,0.101166,-0.707733,-0.705963,0.026428,0.374908,0.68457,0.625092,0.472137,0.208405,-0.856506,-0.703064,-0.581085,-0.409821,-0.417206,-0.736328,0.532623,-0.447876,-0.20285,-0.870728,0.086945,-0.990417,0.107086,0.183685,0.018341,-0.982788,0.560638,
-0.428864,0.708282,0.296722,-0.952576,-0.0672,0.135773,0.990265,0.030243,-0.068787,0.654724,0.752686,0.762604,-0.551758,0.337585,-0.819611,-0.407684,0.402466,-0.727844,-0.55072,-0.408539,-0.855774,-0.480011,0.19281,0.693176,-0.079285,0.716339,0.226013,0.650116,-0.725433,0.246704,0.953369,-0.173553,
-0.970398,-0.239227,-0.03244,0.136383,-0.394318,0.908752,0.813232,0.558167,0.164368,0.40451,0.549042,-0.731323,-0.380249,-0.566711,0.730865,0.022156,0.932739,0.359741,0.00824,0.996552,-0.082306,0.956635,-0.065338,-0.283722,-0.743561,0.008209,0.668579,-0.859589,-0.509674,0.035767,-0.852234,0.363678,
-0.375977,-0.201965,-0.970795,-0.12915,0.313477,0.947327,0.06546,-0.254028,-0.528259,0.81015,0.628052,0.601105,0.49411,-0.494385,0.868378,0.037933,0.275635,-0.086426,0.957336,-0.197937,0.468903,-0.860748,0.895599,0.399384,0.195801,0.560791,0.825012,-0.069214,0.304199,-0.849487,0.43103,0.096375,
0.93576,0.339111,-0.051422,0.408966,-0.911072,0.330444,0.942841,-0.042389,-0.452362,-0.786407,0.420563,0.134308,-0.933472,-0.332489,0.80191,-0.566711,-0.188934,-0.987946,-0.105988,0.112518,-0.24408,0.892242,-0.379791,-0.920502,0.229095,-0.316376,0.7789,0.325958,0.535706,-0.912872,0.185211,-0.36377,
-0.184784,0.565369,-0.803833,-0.018463,0.119537,0.992615,-0.259247,-0.935608,0.239532,-0.82373,-0.449127,-0.345947,-0.433105,0.659515,0.614349,-0.822754,0.378845,-0.423676,0.687195,-0.674835,-0.26889,-0.246582,-0.800842,0.545715,-0.729187,-0.207794,0.651978,0.653534,-0.610443,-0.447388,0.492584,-0.023346,
0.869934,0.609039,0.009094,-0.79306,0.962494,-0.271088,-0.00885,0.2659,-0.004913,0.963959,0.651245,0.553619,-0.518951,0.280548,-0.84314,0.458618,-0.175293,-0.983215,0.049805,0.035339,-0.979919,0.196045,-0.982941,0.164307,-0.082245,0.233734,-0.97226,-0.005005,-0.747253,-0.611328,0.260437,0.645599,
0.592773,0.481384,0.117706,-0.949524,-0.29068,-0.535004,-0.791901,-0.294312,-0.627167,-0.214447,0.748718,-0.047974,-0.813477,-0.57959,-0.175537,0.477264,-0.860992,0.738556,-0.414246,-0.53183,0.562561,-0.704071,0.433289,-0.754944,0.64801,-0.100586,0.114716,0.044525,-0.992371,0.966003,0.244873,-0.082764,
};


      
//************************************************************************************************
//************************************************************************************************
//                                      PUBLIC FUNCTIONS
//************************************************************************************************
//************************************************************************************************

//************************************************************************************************
// constructor
Noise::Noise()
{
	// start with the default permutation table
//	for( unsigned int i = 0; i < 256; ++i )
//		hash[i] = hash[256+i] = g_hash[i];
}
/*
Noise::Noise( const unsigned int seed )
{
	const unsigned int modseed = (1402024253 * seed + 586950981) >> 24;		// 0 ÷ 255

	for( unsigned int i = 0; i < 256; ++i )
		hash[i] = hash[256+i] = g_hash[(modseed + i) & 255];
}
*/

//************************************************************************************************
// destructor
Noise::~Noise()
{}

//*************************************************************************************************
// helper function to obtain the function pointer to the noise basis function

float (Noise::*getNoiseFunctionPtrS(const int noiseBasis))(float, float, float)
{
    switch (noiseBasis) 
    {
		case NOISE_VORONOI_F1       : return &Noise::voronoiF1S;
		case NOISE_VORONOI_F2       : return &Noise::voronoiF2S;   
		case NOISE_VORONOI_F3       : return &Noise::voronoiF3S;   
		case NOISE_VORONOI_F4       : return &Noise::voronoiF4S; 
		case NOISE_VORONOI_F2F1     : return &Noise::voronoiF1F2S; 
		case NOISE_VORONOI_CRACKLE  : return &Noise::voronoiCrackleS;
		case NOISE_CELL             : return &Noise::cellNoiseS;
		case NOISE_BLENDER          : return &Noise::blenderNoiseS;   
   		case NOISE_IMPROVED_PERLIN  : 
		default                     : return &Noise::improvedPerlin3dS;
	}
}



//*************************************************************************************************
// generic noise function, call noise based on basis function
float Noise::noise3dS(float x, float y, float z, int noiseBasis)
{
    float (Noise::*noisefunc)(float, float, float) ;
    noisefunc = getNoiseFunctionPtrS(noiseBasis);
    return (*this.*noisefunc)(x, y, z);
}

/* General Vector noise */
void Noise::noiseVector( float x, float y, float z, int nb, float v[3] )
{   
    float (Noise::*noisefunc)(float, float, float) ;
    noisefunc = getNoiseFunctionPtrS(nb);

	/* Simply evaluate noise at 3 different positions */
	v[0] = (float)(2.0 * (*this.*noisefunc)(x + 9.321f, y - 1.531f,  z - 7.951f) - 1.0);
	v[1] = (float)(2.0 * (*this.*noisefunc)(x,          y,           z        ) - 1.0);
	v[2] = (float)(2.0 * (*this.*noisefunc)(x + 6.327f, y + 0.1671f, z - 2.672f) - 1.0);
}

//************************************************************************************************
// turbulence

float Noise::turbulence3dS(float x, float y, float z, int oct, bool hard, int noiseBasis)
{
    // todo: maybe make frequency scale and amp scale visible to the outside
    
	float (Noise::*noisefunc)(float, float, float) ;
	float sum, t, amp=1, fscale =1;
	int i;
	
    noisefunc = getNoiseFunctionPtrS(noiseBasis);

	sum = 0;
	for (i=0;i<=oct;i++, amp*=0.5, fscale*=2) 
    {
		t = (*this.*noisefunc)(fscale*x, fscale*y, fscale*z);
		if (hard) t = fabs(2.0*t-1.0);
		sum += t * amp;
	}
	
	sum *= ((float)(1<<oct)/(float)((1<<(oct+1))-1));

	return sum;

}


/* Turbulence Vector */

void Noise::turbulenceVector( float x, float y, float z, int oct, bool hard, int noiseBasis, float v[3] )
{
	float t[3];
	int i;
	float amp = 1.f;
	float ampScale = 0.5f;    
    float freqscale = 1.f;
    
	noiseVector( x, y, z, noiseBasis, v );
	if( hard ) 
    {
		v[0] = (float)fabs( v[0] );
		v[1] = (float)fabs( v[1] );
		v[2] = (float)fabs( v[2] );
	}
    
	for( i = 1; i < oct; i++ ) 
    {
		amp *= ampScale;
        freqscale *= 2;
		x *= freqscale;
		y *= freqscale;
		z *= freqscale;
		noiseVector( x, y, z, noiseBasis, t );
        
		if( hard ) 
        {
			t[0] = (float)fabs( t[0] );
			t[1] = (float)fabs( t[1] );
			t[2] = (float)fabs( t[2] );
		}
		v[0] += amp * t[0];
		v[1] += amp * t[1];
		v[2] += amp * t[2];
	}
}




//************************************************************************************************
// improved perlin noise function, after ken perlin 2002
//


// lerp and fade as defines - not very nice, but the preprocessor will make sure the code's faster
#define lerp(t, a, b) ((a)+(t)*((b)-(a)))
#define fade(t) ((t)*(t)*(t)*((t)*((t)*6-15)+10))


//************************************************************************************************
float Noise::gradient( const int hash, const float x, const float y, const float z, const float w )
{
	int h = hash & 31;	// h from 0 to 31

	/*
	float a = 0;
	float b = 0;
	float c = 0;

	switch (h >> 3) 
	{
	case 0:		// for 'h' from 0 to 7
		a = x;
		b = y;
		c = z;
		break;
	case 1:		// for 'h' from 8 to 15
		a = w;
		b = x;
		c = y;
		break;
	case 2:		// for 'h' from 16 to 23
		a = z;
		b = w;
		c = x;
		break;
	case 3:		// for 'h' from 24 to 31
		a = y;
		b = z;
		c = w;
		break;
	}

	return ((h&4)==0 ? -a:a) + ((h&2)==0 ? -b:b) + ((h&1)==0 ? -c:c);
	*/

	// hopefully, this should be compiled into one big jump-table
	switch( h )
	{
	case 0:
		return -x - y - z;
	case 1:
		return -x - y + z;
	case 2:
		return -x + y - z;
	case 3:
		return -x + y + z;
	case 4:
		return x - y - z;
	case 5:
		return x - y + z;
	case 6:
		return x + y - z;
	case 7:
		return x + y + z;

	case 8:
		return -w - x - y;
	case 9:
		return -w - x + y;
	case 10:
		return -w + x - y;
	case 11:
		return -w + x + y;
	case 12:
		return w - x - y;
	case 13:
		return w - x + y;
	case 14:
		return w + x - y;
	case 15:
		return w + x + y;

	case 16:
		return -z - w - x;
	case 17:
		return -z - w + x;
	case 18:
		return -z + w - x;
	case 19:
		return -z + w + x;
	case 20:
		return z - w - x;
	case 21:
		return z - w + x;
	case 22:
		return z + w - x;
	case 23:
		return z + w + x;

	case 24:
		return -y - z - w;
	case 25:
		return -y - z + w;
	case 26:
		return -y + z - w;
	case 27:
		return -y + z + w;
	case 28:
		return y - z - w;
	case 29:
		return y - z + w;
	case 30: 
		return y + z - w;
	default:				// case 31:
		return y + z + w;
	}
}

float Noise::gradient( const int hash, const float x, const float y, const float z ) 
{
	// choose the index of one of 16 gradient vectors ( 0 ÷ 15 )
	const int h = hash & 15;

	/* original Perlin's code, much shorter from my BIG-GIANT switch statement

	float u = (h < 8) ? x : y; 
	float v = (h < 4) ? y : (h==12 || h==14) ? x : z;
	return (( h & 1 ) == 0 ? u : -u) + (( h & 2 ) == 0 ? v : -v);
	*/

	switch( h )
	{
	case 0:
		return x + y;		// DP3( (x,y,z), (1,1,0) )
	case 1:
		return -x + y;		// DP3( (x,y,z), (-1,1,0) )
	case 2:
		return x - y;		// DP3( (x,y,z), (1,-1,0) )
	case 3:
		return -x - y;		// DP3( (x,y,z), (-1,-1,0) )
	case 4:
		return x + z;		// DP3( (x,y,z), (1,0,1) )
	case 5:
		return -x + z;
	case 6:
		return x - z;
	case 7:
		return -x - z;
	case 8:
		return y + z;
	case 9:
		return -y + z;
	case 10:
		return y - z;
	case 11:
		return -y - z;

	// padding from 12 to 16 cases, for explanation
	// see Perlin's document on Improved Perlin Noise implementation
	case 12:
		return y + x;
	case 13:
		return -y + z;
	case 14:
		return y - x;
	default:				// case 15:
		return -y - z;		// DP3( (x,y,z), (0,-1,-1) )
	}
}

float Noise::gradient( const int hash, const float x, const float y )
{

	switch( hash & 7 )
	{
	case 0:
		return x + y;
	case 1:
		return x - y;
	case 2:
		return -x + y;
	case 3:
		return -x - y;
	case 4:
		return x;
	case 5:
		return -x;
	case 6:
		return y;
	default:
		return -y;
	}
}

float Noise::gradient( const int hash, const float x )
{
	return (hash & 1) ? x : -x;
}


//************************************************************************************************
// 1d noise constructs a noisy line

float Noise::improvedPerlin1dS( float x)
{
	int X = floor( x );
	x -= X;
	X &= 255;

	int X1 = X + 1;	

	return lerp( fade( x ), gradient( hash[ X  ], x     ),
							gradient( hash[ X1 ], x - 1 ));
}

float Noise::improvedPerlin1dU(float x)
{
    return signedToUnsignedNoiseValue(improvedPerlin1dS(x));
}


//************************************************************************************************
// 2d noise constructs 2 noise lines and then lerps between them through 'b' (modified 'y') 
// parameter, making a noise quad
//
float Noise::improvedPerlin2dS( float x, float y)
{
	int X = floor( x );
	int Y = floor( y );

	// fractional parts ( from 0.0 to 1.0 )
	x -= X;
	y -= Y;

	const float a = fade( x );
	const float b = fade( y );

	X &= 255;
	Y &= 255;

	int X1 = X + 1;	
	int Y1 = Y + 1;	

	int AA = hash[ X  ] + Y;		// left		bottom
	int BA = hash[ X1 ] + Y;		// right	bottom
	int AB = hash[ X  ] + Y1;	// left		top
	int BB = hash[ X1 ] + Y1;	// right	top

	const float x_1 = x - 1.0f;
	const float y_1 = y - 1.0f;

	return lerp( b, lerp( a, gradient( hash[ AA ], x  , y   ),
							 gradient( hash[ BA ], x_1, y   )),
					lerp( a, gradient( hash[ AB ], x  , y_1 ),
							 gradient( hash[ BB ], x_1, y_1 )));
}

float Noise::improvedPerlin2dU(float x, float y)
{
    return signedToUnsignedNoiseValue(improvedPerlin2dS(x,y));
}


//************************************************************************************************
// 2d noise constructs a noise quad
// 3d noise constructs 2 noise quads and then lerps between them through 'c' (modified 'z') 
// parameter, making a noisy cube
//
//
float Noise::improvedPerlin3dS(float x, float y, float z)
{
	int X = floor( x );
	int Y = floor( y );
	int Z = floor( z );

	// fractional parts ( from 0.0 to 1.0 )
	x -= X;
	y -= Y;
	z -= Z;

	const float a = fade( x );
	const float b = fade( y );
	const float c = fade( z );

	X &= 255;
	Y &= 255;
	Z &= 255;

	int X1 = X + 1;	
	int Y1 = Y + 1;	
	int Z1 = Z + 1;	

	//--------------------------
	// indices for 2 noise quads
	//--------------------------

	int AA = hash[ X  ] + Y;		// left		bottom
	int BA = hash[ X1 ] + Y;		// right	bottom
	int AB = hash[ X  ] + Y1;	// left		top
	int BB = hash[ X1 ] + Y1;	// right	top

	// front-side quad indices
	int AAA = hash[ AA ] + Z;
	int BAA = hash[ BA ] + Z;
	int ABA = hash[ AB ] + Z;
	int BBA = hash[ BB ] + Z;

	// back-side quad indices
	int AAB = hash[ AA ] + Z1;
	int BAB = hash[ BA ] + Z1;
	int ABB = hash[ AB ] + Z1;
	int BBB = hash[ BB ] + Z1;

	//-----------------------
	// lerp between the quads
	//-----------------------

	const float x_1 = x - 1.0f;
	const float y_1 = y - 1.0f;
	const float z_1 = z - 1.0f;

	return lerp( c, lerp( b, lerp( a, gradient( hash[ AAA ], x  , y  , z   ),
								      gradient( hash[ BAA ], x_1, y  , z   )),
						     lerp( a, gradient( hash[ ABA ], x  , y_1, z   ),
								      gradient( hash[ BBA ], x_1, y_1, z   ))),
				    lerp( b, lerp( a, gradient( hash[ AAB ], x  , y  , z_1 ),
								      gradient( hash[ BAB ], x_1, y  , z_1 )),
						     lerp( a, gradient( hash[ ABB ], x  , y_1, z_1 ),
								      gradient( hash[ BBB ], x_1, y_1, z_1 ))));
}

float Noise::improvedPerlin3dU(float x, float y, float z)                      
{
    return signedToUnsignedNoiseValue(improvedPerlin3dS(x,y,z));
}


//************************************************************************************************
// 3d noise constructs a noise volume ( cube )
// 4d noise constructs 2 noisy cubes and then lerps between them through 'd' (modified 'w') 
// parameter, making the effect of animated volume
//
float Noise::improvedPerlin4dS(	float x, float y, float z, float w)
{
	int X = floor( x );
	int Y = floor( y );
	int Z = floor( z );
	int W = floor( w );

	x -= X;
	y -= Y;
	z -= Z;
	w -= W;

	const float a = fade( x );
	const float b = fade( y );
	const float c = fade( z );
	const float d = fade( w );

	X &= 255;
	Y &= 255;
	Z &= 255;
	W &= 255;

	int X1 = X + 1;
	int Y1 = Y + 1;
	int Z1 = Z + 1;
	int W1 = W + 1;

	//----------------------------
	// indices for 2 noise volumes
	//----------------------------

	int AA = hash[ X  ] + Y;		// left		bottom
	int BA = hash[ X1 ] + Y;		// right	bottom

	int AB = hash[ X  ] + Y1;	// left		top
	int BB = hash[ X1 ] + Y1;	// right	top

	int AAA = hash[ AA ] + Z;	// left		bottom	front
	int BAA = hash[ BA ] + Z;	// right	bottom	front
	int ABA = hash[ AB ] + Z;	// left		top		front
	int BBA = hash[ BB ] + Z;	// right	top		front

	int AAB = hash[ AA ] + Z1;	// left		bottom	back
	int BAB = hash[ BA ] + Z1;	// right	bottom	back
	int ABB = hash[ AB ] + Z1;	// left		top		back
	int BBB = hash[ BB ] + Z1;	// right	top		back

	int AAAA = hash[ AAA ] + W;	// left		bottom	front	volume1
	int BAAA = hash[ BAA ] + W;	// right	bottom	front	volume1
	int ABAA = hash[ ABA ] + W;	// left		top		front	volume1
	int BBAA = hash[ BBA ] + W;	// right	top		front	volume1
	int AABA = hash[ AAB ] + W;	// left		bottom	back	volume1
	int BABA = hash[ BAB ] + W;	// right	bottom	back	volume1
	int ABBA = hash[ ABB ] + W;	// left		top		back	volume1
	int BBBA = hash[ BBB ] + W;	// right	top		back	volume1

	int AAAB = hash[ AAA ] + W1;	// left		bottom	front	volume2
	int BAAB = hash[ BAA ] + W1;	// right	bottom	front	volume2
	int ABAB = hash[ ABA ] + W1;	// left		top		front	volume2
	int BBAB = hash[ BBA ] + W1;	// right	top		front	volume2
	int AABB = hash[ AAB ] + W1;	// left		bottom	back	volume2
	int BABB = hash[ BAB ] + W1;	// right	bottom	back	volume2
	int ABBB = hash[ ABB ] + W1;	// left		top		back	volume2
	int BBBB = hash[ BBB ] + W1;	// right	top		back	volume2

	//-------------------------
	// lerp between the volumes
	//-------------------------

	const float x_1 = x - 1.0f;
	const float y_1 = y - 1.0f;
	const float z_1 = z - 1.0f;
	const float w_1 = w - 1.0f;

	return lerp( d, lerp( c, lerp( b, lerp( a,  gradient( hash[ AAAA ], x  , y  , z  , w   ), 
												gradient( hash[ BAAA ], x_1, y  , z  , w   )),
									  lerp( a,	gradient( hash[ ABAA ], x  , y_1, z  , w   ), 
												gradient( hash[ BBAA ], x_1, y_1, z  , w   ))),
							 lerp( b, lerp( a,	gradient( hash[ AABA ], x  , y  , z_1, w   ), 
												gradient( hash[ BABA ], x_1, y  , z_1, w   )),
									  lerp( a,	gradient( hash[ ABBA ], x  , y_1, z_1, w   ),
												gradient( hash[ BBBA ], x_1, y_1, z_1, w   )))),
					lerp( c, lerp( b, lerp( a,	gradient( hash[ AAAB ], x  , y  , z  , w_1 ), 
												gradient( hash[ BAAB ], x_1, y  , z  , w_1 )),
									  lerp( a,	gradient( hash[ ABAB ], x  , y_1, z  , w_1 ), 
												gradient( hash[ BBAB ], x_1, y_1, z  , w_1 ))),
							 lerp( b, lerp( a,	gradient( hash[ AABB ], x  , y  , z_1, w_1 ), 
												gradient( hash[ BABB ], x_1, y  , z_1, w_1 )),
									  lerp( a,	gradient( hash[ ABBB ], x  , y_1, z_1, w_1 ),
												gradient( hash[ BBBB ], x_1, y_1, z_1, w_1 )))));
}



float Noise::improvedPerlin4dU(float x, float y, float z, float w)
{
    return signedToUnsignedNoiseValue(improvedPerlin4dS(x,y,z,w));
}


//************************************************************************************************
// voronoi / worley noise
//

// distance metrics for voronoi, e parameter only used in Minkovsky

// distance squared
static float voronoiDistanceSquared(float x, float y, float z, float e) 
{ 
    return (x*x + y*y + z*z); 
}

// real distance
static float voronoiDistanceReal(float x, float y, float z, float e) 
{ 
    return sqrt(x*x + y*y + z*z); 
}

// manhattan/taxicab/cityblock distance
static float voronoiDistanceManhattan(float x, float y, float z, float e) 
{ 
    return (fabs(x) + fabs(y) + fabs(z)); 
}

// Chebychev 
static float voronoiDistanceChebychev(float x, float y, float z, float e)
{
	float t;
	x = fabs(x);
	y = fabs(y);
	z = fabs(z);
	t = (x>y)?x:y;
	return ((z>t)?z:t);
}

// minkovsky preset exponent
static float voronoiDistanceMinkovskyH(float x, float y, float z, float e)
{
	float d = sqrt(fabs(x)) + sqrt(fabs(y)) + sqrt(fabs(z));
	return (d*d);
}

// minkovsky preset exponent 4
static float voronoiDistanceMinkovsky4(float x, float y, float z, float e)
{
	x *= x;
	y *= y;
	z *= z;
	return sqrt(sqrt(x*x + y*y + z*z));
}

// Minkovsky, general case, slow, maybe too slow to be useful 
static float voronoiDistanceMinkovsky(float x, float y, float z, float e)
{
    return pow(pow(fabs(x), e) + pow(fabs(y), e) + pow(fabs(z), e), 1.0/e);
}


//  Not 'pure' Worley, but the results are virtually the same.
//	Returns distances in da and point coords in pa */
void Noise::voronoi(float x, float y, float z, float* da, float* pa, float me, int dtype)
{
	int xx, yy, zz, xi, yi, zi;
	float xd, yd, zd, d, *p;

	float (*distfunc)(float, float, float, float);
	switch (dtype) 
    {
		case VORONOI_DIST_SQUARED   : distfunc = voronoiDistanceSquared; break;
		case VORONOI_DIST_MANHATTAN : distfunc = voronoiDistanceManhattan; break;
		case VORONOI_DIST_CHEBYCHEV : distfunc = voronoiDistanceChebychev; break;
		case VORONOI_DIST_MINKOVSKYH:	distfunc = voronoiDistanceMinkovskyH; break;
		case VORONOI_DIST_MINKOVSKY4: distfunc = voronoiDistanceMinkovsky4; break;
		case VORONOI_DIST_MINKOVSKY : distfunc = voronoiDistanceMinkovsky; break;
		case VORONOI_DIST_REAL:
        default: distfunc = voronoiDistanceReal;
	}

	xi = (int)(floor(x));
	yi = (int)(floor(y));
	zi = (int)(floor(z));
	da[0] = da[1] = da[2] = da[3] = 1e10f;
	for (xx=xi-1;xx<=xi+1;xx++) 
    {
		for (yy=yi-1;yy<=yi+1;yy++) 
        {
			for (zz=zi-1;zz<=zi+1;zz++) 
            {
				p = HASHPNT(xx, yy, zz);
				xd = x - (p[0] + xx);
				yd = y - (p[1] + yy);
				zd = z - (p[2] + zz);
				d = distfunc(xd, yd, zd, me);
				if (d<da[0]) 
                {
					da[3]=da[2];  da[2]=da[1];  da[1]=da[0];  da[0]=d;
					pa[9]=pa[6];  pa[10]=pa[7];  pa[11]=pa[8];
					pa[6]=pa[3];  pa[7]=pa[4];  pa[8]=pa[5];
					pa[3]=pa[0];  pa[4]=pa[1];  pa[5]=pa[2];
					pa[0]=p[0]+xx;  pa[1]=p[1]+yy;  pa[2]=p[2]+zz;
				}
				else if (d<da[1]) 
                {
					da[3]=da[2];  da[2]=da[1];  da[1]=d;
					pa[9]=pa[6];  pa[10]=pa[7];  pa[11]=pa[8];
					pa[6]=pa[3];  pa[7]=pa[4];  pa[8]=pa[5];
					pa[3]=p[0]+xx;  pa[4]=p[1]+yy;  pa[5]=p[2]+zz;
				}
				else if (d<da[2]) 
                {
					da[3]=da[2];  da[2]=d;
					pa[9]=pa[6];  pa[10]=pa[7];  pa[11]=pa[8];
					pa[6]=p[0]+xx;  pa[7]=p[1]+yy;  pa[8]=p[2]+zz;
				}
				else if (d<da[3]) 
                {
					da[3]=d;
					pa[9]=p[0]+xx;  pa[10]=p[1]+yy;  pa[11]=p[2]+zz;
				}
			}
		}
	}
}


/**************/
// wrapper to retrieve different aspect of worley noise
float Noise::voronoiF1U(float x, float y, float z)
{
	float da[4], pa[12];
	voronoi(x, y, z, da, pa, 1,VORONOI_DIST_REAL);
	return da[0];
}

float Noise::voronoiF1S(float x, float y, float z)
{
    return unsignedToSignedNoiseValue(voronoiF1U(x,y,z));
}

/**************/

float Noise::voronoiF2U(float x, float y, float z)
{
	float da[4], pa[12];
	voronoi(x, y, z, da, pa, 1,VORONOI_DIST_REAL);
	return da[1];
}

float Noise::voronoiF2S(float x, float y, float z)
{
    return unsignedToSignedNoiseValue(voronoiF2U(x,y,z));
}

/**************/

float Noise::voronoiF3U(float x, float y, float z)
{
	float da[4], pa[12];
	voronoi(x, y, z, da, pa, 1, VORONOI_DIST_REAL);
	return da[2];
}

float Noise::voronoiF3S(float x, float y, float z)
{
    return unsignedToSignedNoiseValue(voronoiF3U(x,y,z));
}

/**************/
float Noise::voronoiF4U(float x, float y, float z)
{
	float da[4], pa[12];
	voronoi(x, y, z, da, pa, 1, VORONOI_DIST_REAL);
	return da[3];
}

float Noise::voronoiF4S(float x, float y, float z)
{
    return unsignedToSignedNoiseValue(voronoiF4U(x,y,z));
}


/**************/
float Noise::voronoiF1F2U(float x, float y, float z)
{
	float da[4], pa[12];
	voronoi(x, y, z, da, pa, 1, VORONOI_DIST_REAL);
	return (da[1]-da[0]);
}

float Noise::voronoiF1F2S(float x, float y, float z)
{
    return unsignedToSignedNoiseValue(voronoiF1F2U(x,y,z));
}

/**************/
/* Crackle type pattern, just a scale/clamp of F2-F1 */
float Noise::voronoiCrackleU(float x, float y, float z)
{
	float t = 10 * voronoiF1F2U(x, y, z);
	if (t>1.f) return 1.f;
	return t;
}

float Noise::voronoiCrackleS(float x, float y, float z)
{
    return unsignedToSignedNoiseValue(voronoiCrackleU(x,y,z));
}


//************************************************************************************************
// cell noise
float Noise::cellNoiseU(float x, float y, float z)
{
  int xi = (int)(floor(x));
  int yi = (int)(floor(y));
  int zi = (int)(floor(z));
  unsigned int n = xi + yi*1301 + zi*314159;
  n ^= (n<<13);
  return ((float)(n*(n*n*15731 + 789221) + 1376312589) / 4294967296.0);
}

float Noise::cellNoiseS(float x, float y, float z)
{
    return unsignedToSignedNoiseValue(cellNoiseU(x,y,z));
}


//************************************************************************************************
// blender noise

float Noise::blenderNoiseU(float x, float y, float z)
{
	register float cn1, cn2, cn3, cn4, cn5, cn6, i, *h;
	float ox, oy, oz, jx, jy, jz;
	float n= 0.5;
	int ix, iy, iz, b00, b01, b10, b11, b20, b21;

	ox= (x- (ix= (int)floor(x)) );
	oy= (y- (iy= (int)floor(y)) );
	oz= (z- (iz= (int)floor(z)) );

	jx= ox-1;
	jy= oy-1;
	jz= oz-1;

	cn1=ox*ox; cn2=oy*oy; cn3=oz*oz;
	cn4=jx*jx; cn5=jy*jy; cn6=jz*jz;

	cn1= 1.0-3.0*cn1+2.0*cn1*ox;
	cn2= 1.0-3.0*cn2+2.0*cn2*oy;
	cn3= 1.0-3.0*cn3+2.0*cn3*oz;
	cn4= 1.0-3.0*cn4-2.0*cn4*jx;
	cn5= 1.0-3.0*cn5-2.0*cn5*jy;
	cn6= 1.0-3.0*cn6-2.0*cn6*jz;

	b00= hash[ hash[ix & 255]+(iy & 255)];
	b10= hash[ hash[(ix+1) & 255]+(iy & 255)];
	b01= hash[ hash[ix & 255]+((iy+1) & 255)];
	b11= hash[ hash[(ix+1) & 255]+((iy+1) & 255)];

	b20=iz & 255; b21= (iz+1) & 255;

		/* 0 */
	i= (cn1*cn2*cn3);
		h=hashvectf+ 3*hash[b20+b00];
		n+= i*(h[0]*ox+h[1]*oy+h[2]*oz);
		/* 1 */
	i= (cn1*cn2*cn6);
		h=hashvectf+ 3*hash[b21+b00];
		n+= i*(h[0]*ox+h[1]*oy+h[2]*jz);
		/* 2 */
	i= (cn1*cn5*cn3);
		h=hashvectf+ 3*hash[b20+b01];
		n+= i*(h[0]*ox+h[1]*jy+h[2]*oz);
		/* 3 */
	i= (cn1*cn5*cn6);
		h=hashvectf+ 3*hash[b21+b01];
		n+= i*(h[0]*ox+h[1]*jy+h[2]*jz);
		/* 4 */
	i= cn4*cn2*cn3;
		h=hashvectf+ 3*hash[b20+b10];
		n+= i*(h[0]*jx+h[1]*oy+h[2]*oz);
		/* 5 */
	i= cn4*cn2*cn6;
		h=hashvectf+ 3*hash[b21+b10];
		n+= i*(h[0]*jx+h[1]*oy+h[2]*jz);
		/* 6 */
	i= cn4*cn5*cn3;
		h=hashvectf+ 3*hash[b20+b11];
		n+=  i*(h[0]*jx+h[1]*jy+h[2]*oz);
		/* 7 */
	i= (cn4*cn5*cn6);
		h=hashvectf+ 3*hash[b21+b11];
		n+= i*(h[0]*jx+h[1]*jy+h[2]*jz);

	if(n<0.0) n=0.0; else if(n>1.0) n=1.0;
	return n;
}


float Noise::blenderNoiseS(float x, float y, float z)
{
    return unsignedToSignedNoiseValue(blenderNoiseU(x,y,z));
}            
            




//************************************************************************************************
//************************************************************************************************
// musgrave noise functions

 /* Parameters:
 *    ``H''  is the fractal increment parameter
 *    ``lacunarity''  is the gap between successive frequencies
 *    ``octaves''  is the number of frequencies in the fBm
 */
float Noise::musgraveFBmS(float x, float y, float z, float H, float lacunarity, float octaves, int noiseBasis)
{
	float	rmd, value=0.0, pwr=1.0, pwHL=pow(lacunarity, -H);
	int	i;


	float (Noise::*noisefunc)(float, float, float) ;
    noisefunc = getNoiseFunctionPtrS(noiseBasis);
    
	
	for (i=0; i<(int)octaves; i++) 
    {
		value += (*this.*noisefunc)(x, y, z) * pwr;
		pwr *= pwHL;
		x *= lacunarity;
		y *= lacunarity;
		z *= lacunarity;
	}

	rmd = octaves - floor(octaves);
	if (rmd!=0.f) value += rmd * (*this.*noisefunc)(x, y, z) * pwr;

	return value;

} /* fBm() */


/******************************************************/

/*
 * Procedural multifractal evaluated at "point";
 * returns value stored in "value".
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Parameters:
 *    ``H''  determines the highest fractal dimension
 *    ``lacunarity''  is gap between successive frequencies
 *    ``octaves''  is the number of frequencies in the fBm
 *    ``offset''  is the zero offset, which determines multifractality (NOT USED??)
 */
 /* this one is in fact rather confusing,
 	* there seem to be errors in the original source code (in all three versions of proc.text&mod),
	* I modified it to something that made sense to me, so it might be wrong... */

float Noise::musgraveMultiFractalS(float x, float y, float z, float H, float lacunarity, float octaves, int noiseBasis)
{
	float	rmd, value=1.0, pwr=1.0, pwHL=pow(lacunarity, -H);
	int i;
	
	float (Noise::*noisefunc)(float, float, float) ;
    noisefunc = getNoiseFunctionPtrS(noiseBasis);
 
	for (i=0; i<(int)octaves; i++) {
		value *= (pwr * (*this.*noisefunc)(x, y, z) + 1.0);
		pwr *= pwHL;
		x *= lacunarity;
		y *= lacunarity;
		z *= lacunarity;
	}
	rmd = octaves - floor(octaves);
	if (rmd!=0.0) value *= (rmd * (*this.*noisefunc)(x, y, z) * pwr + 1.0);

	return value;

} /* multifractal() */

/******************************************************/

/* "Variable Lacunarity Noise"
 * A distorted variety of Perlin noise.
 *
 * Copyright 1994 F. Kenton Musgrave*/
float Noise::musgraveVLNoiseS(float x, float y, float z, float distortion, int noiseBasis1, int noiseBasis2)
{
	float rv[3];
	float (Noise::*noisefunc1)(float, float, float);
	float (Noise::*noisefunc2)(float, float, float);
    
    noisefunc1 = getNoiseFunctionPtrS(noiseBasis1);
    noisefunc2 = getNoiseFunctionPtrS(noiseBasis2);    

	/* get a random vector and scale the randomization */
	rv[0] = (*this.*noisefunc1)(x+13.5, y+13.5, z+13.5) * distortion;
	rv[1] = (*this.*noisefunc1)(x, y, z) * distortion;
	rv[2] = (*this.*noisefunc1)(x-13.5, y-13.5, z-13.5) * distortion;
	return (*this.*noisefunc2)(x+rv[0], y+rv[1], z+rv[2]);	/* distorted-domain noise */
}

/******************************************************/

/*
 * Heterogeneous procedural terrain function: stats by altitude method.
 * Evaluated at "point"; returns value stored in "value".
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Parameters:
 *       ``H''  determines the fractal dimension of the roughest areas
 *       ``lacunarity''  is the gap between successive frequencies
 *       ``octaves''  is the number of frequencies in the fBm
 *       ``offset''  raises the terrain from `sea level'
 */
float Noise::musgraveHeteroTerrainS(float x, float y, float z, float H, float lacunarity, float octaves, float offset, int noiseBasis)
{
	float	value, increment, rmd;
	int i;
	float pwHL = pow(lacunarity, -H);
	float pwr = pwHL;	/* starts with i=1 instead of 0 */


	float (Noise::*noisefunc)(float, float, float) ;
    noisefunc = getNoiseFunctionPtrS(noiseBasis);

    
	/* first unscaled octave of function; later octaves are scaled */
	value = offset + (*this.*noisefunc)(x, y, z);
	x *= lacunarity;
	y *= lacunarity;
	z *= lacunarity;

	for (i=1; i<(int)octaves; i++) 
    {
		increment = ((*this.*noisefunc)(x, y, z) + offset) * pwr * value;
		value += increment;
		pwr *= pwHL;
		x *= lacunarity;
		y *= lacunarity;
		z *= lacunarity;
	}

	rmd = octaves - floor(octaves);
	if (rmd!=0.0) 
    {
		increment = ((*this.*noisefunc)(x, y, z) + offset) * pwr * value;
		value += rmd * increment;
	}
	return value;
}


/******************************************************/

/* Hybrid additive/multiplicative multifractal terrain model.
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Some good parameter values to start with:
 *
 *      H:           0.25
 *      offset:      0.7
 */
float Noise::musgraveHybridMultiFractalS(float x, float y, float z, float H, float lacunarity, float octaves, float offset, float gain, int noiseBasis)
{
	float result, signal, weight, rmd;
	int i;
	float pwHL = pow(lacunarity, -H);
	float pwr = pwHL;	/* starts with i=1 instead of 0 */

    float (Noise::*noisefunc)(float, float, float) ;
    noisefunc = getNoiseFunctionPtrS(noiseBasis);

	result = (*this.*noisefunc)(x, y, z) + offset;
	weight = gain * result;
	x *= lacunarity;
	y *= lacunarity;
	z *= lacunarity;

	for (i=1; (weight>0.001) && (i<(int)octaves); i++) 
    {
		if (weight>1.0)  weight=1.0;
		signal = ((*this.*noisefunc)(x, y, z) + offset) * pwr;
		pwr *= pwHL;
		result += weight * signal;
		weight *= gain * signal;
		x *= lacunarity;
		y *= lacunarity;
		z *= lacunarity;
	}

	rmd = octaves - floor(octaves);
	if (rmd!=0.f) result += rmd * (((*this.*noisefunc)(x, y, z) + offset) * pwr);

	return result;

} /* HybridMultifractal() */


/******************************************************/

/* Ridged multifractal terrain model.
 *
 * Copyright 1994 F. Kenton Musgrave
 *
 * Some good parameter values to start with:
 *
 *      H:           1.0
 *      offset:      1.0
 *      gain:        2.0
 */
float Noise::musgraveRidgedMultiFractalS(float x, float y, float z, float H, float lacunarity, float octaves, float offset, float gain, int noiseBasis)
{
	float result, signal, weight;
	int	i;
	float pwHL = pow(lacunarity, -H);
	float pwr = pwHL;	/* starts with i=1 instead of 0 */
	
	float (Noise::*noisefunc)(float, float, float) ;
    noisefunc = getNoiseFunctionPtrS(noiseBasis);

	signal = offset - fabs((*this.*noisefunc)(x, y, z));
	signal *= signal;
	result = signal;
	weight = 1.f;

	for( i=1; i<(int)octaves; i++ ) {
		x *= lacunarity;
		y *= lacunarity;
		z *= lacunarity;
		weight = signal * gain;
		if (weight>1.0) weight=1.0; else if (weight<0.0) weight=0.0;
		signal = offset - fabs((*this.*noisefunc)(x, y, z));
		signal *= signal;
		signal *= weight;
		result += signal * pwr;
		pwr *= pwHL;
	}

	return result;
} /* RidgedMultifractal() */


/******************************************************/
// musgrave functions for unsigned values

float Noise::musgraveFBmU(float x, float y, float z, float H, float lacunarity, float octaves, int noiseBasis)
{
    return signedToUnsignedNoiseValue( musgraveFBmS( x,  y,  z,  H,  lacunarity,  octaves,  noiseBasis) );
}

float Noise::musgraveMultiFractalU(float x, float y, float z, float H, float lacunarity, float octaves, int noiseBasis)
{
    return signedToUnsignedNoiseValue( musgraveMultiFractalS( x,  y,  z,  H,  lacunarity,  octaves,  noiseBasis));
}

float Noise::musgraveVLNoiseU(float x, float y, float z, float distortion, int noiseBasis1, int noiseBasis2)
{
    return signedToUnsignedNoiseValue(musgraveVLNoiseS( x,  y,  z,  distortion,  noiseBasis1,  noiseBasis2) ); 
}

float Noise::musgraveHeteroTerrainU(float x, float y, float z, float H, float lacunarity, float octaves, float offset, int noiseBasis)
{
    return signedToUnsignedNoiseValue(musgraveHeteroTerrainS( x,  y,  z,  H,  lacunarity,  octaves,  offset,  noiseBasis) );  
}

float Noise::musgraveHybridMultiFractalU(float x, float y, float z, float H, float lacunarity, float octaves, float offset, float gain, int noiseBasis)
{
    return signedToUnsignedNoiseValue(  musgraveHybridMultiFractalS( x,  y,  z,  H,  lacunarity,  octaves,  offset,  gain,  noiseBasis));
}

float Noise::musgraveRidgedMultiFractalU(float x, float y, float z, float H, float lacunarity, float octaves, float offset, float gain, int noiseBasis)              
{
    return signedToUnsignedNoiseValue(musgraveRidgedMultiFractalS( x,  y,  z,  H,  lacunarity,  octaves,  offset,  gain,  noiseBasis) );
}


//*******************************************************************************************
// helper functions

float Noise::unsignedToSignedNoiseValue(float x)
{
    return (2.0 * x -1.0);
}

float Noise::signedToUnsignedNoiseValue(float x)
{
    return (0.5 * x + 0.5);
}
