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
 
 // function library of various noise calls
 
#ifndef __Noise_H__
#define __Noise_H__

// noise basis functions
#define NOISE_IMPROVED_PERLIN	1
#define NOISE_VORONOI_F1		2
#define NOISE_VORONOI_F2		3
#define NOISE_VORONOI_F3		4
#define NOISE_VORONOI_F4		5
#define NOISE_VORONOI_F2F1	    6
#define NOISE_VORONOI_CRACKLE	7
#define NOISE_CELL  		    8
#define NOISE_BLENDER			9

// voronoi distance metrics
#define VORONOI_DIST_REAL       0
#define VORONOI_DIST_SQUARED    1
#define VORONOI_DIST_MANHATTAN  2
#define VORONOI_DIST_CHEBYCHEV  3
#define VORONOI_DIST_MINKOVSKY  4
#define VORONOI_DIST_MINKOVSKYH 5
#define VORONOI_DIST_MINKOVSKY4 6


class Noise
{
    public:
    	Noise();
//    	Noise( const unsigned int seed );
    	~Noise();

	    void defaultPermutationTable();
    	void reshufflePermutationTable( const unsigned int seed );

        // general noise call for signed noise (just a convinience wrapper)
           float noise3dS(float x, float y, float z, int noiseBasis);
           void noiseVector(float x, float y, float z, int noiseBasis,float v[3] );
        
        // general turbulence
           float turbulence3dS(float x, float y, float z, int oct, bool hard, int noiseBasis);
           void turbulenceVector(float x, float y, float z, int oct, bool hard, int noiseBasis,float v[3] );
           
              
        // unsigned (0 to 1)
        
            // basis noise functions
            //
            
            // improved perlin
            float improvedPerlin1dU(float x);
            float improvedPerlin2dU(float x, float y);
            float improvedPerlin3dU(float x, float y, float z);                        
            float improvedPerlin4dU(float x, float y, float z, float w);

            // voronoi / worley
            float voronoiF1U(float x, float y, float z);
            float voronoiF2U(float x, float y, float z);
            float voronoiF3U(float x, float y, float z);
            float voronoiF4U(float x, float y, float z);
            float voronoiF1F2U(float x, float y, float z);
            float voronoiCrackleU(float x, float y, float z);
        
            // cell noise
            float cellNoiseU(float x, float y, float z);
        
            // blender noise
            float blenderNoiseU(float x, float y, float z);
        
            // fractal noise
            // mussgrave noise functions
            float musgraveFBmU(float x, float y, float z, float H, float lacunarity, float octaves, int noiseBasis);
            float musgraveMultiFractalU(float x, float y, float z, float H, float lacunarity, float octaves, int noiseBasis);
            float musgraveVLNoiseU(float x, float y, float z, float distortion, int noiseBasis1, int noiseBasis2);
            float musgraveHeteroTerrainU(float x, float y, float z, float H, float lacunarity, float octaves, float offset, int noiseBasis);
            float musgraveHybridMultiFractalU(float x, float y, float z, float H, float lacunarity, float octaves, float offset, float gain, int noiseBasis);
            float musgraveRidgedMultiFractalU(float x, float y, float z, float H, float lacunarity, float octaves, float offset, float gain, int noiseBasis);               


        // the same functions, just returning a signed value between (-1 and 1)

            // improved perlin
            float improvedPerlin1dS(float x);
            float improvedPerlin2dS(float x, float y);
            float improvedPerlin3dS(float x, float y, float z);                        
            float improvedPerlin4dS(float x, float y, float z, float w);

            // voronoi / worley
            float voronoiF1S(float x, float y, float z);
            float voronoiF2S(float x, float y, float z);
            float voronoiF3S(float x, float y, float z);
            float voronoiF4S(float x, float y, float z);
            float voronoiF1F2S(float x, float y, float z);
            float voronoiCrackleS(float x, float y, float z);
        
            // cell noise
            float cellNoiseS(float x, float y, float z);
        
            // blender noise
            float blenderNoiseS(float x, float y, float z);

            // mussgrave noise functions
            float musgraveFBmS(float x, float y, float z, float H, float lacunarity, float octaves, int noiseBasis);
            float musgraveMultiFractalS(float x, float y, float z, float H, float lacunarity, float octaves, int noiseBasis);
            float musgraveVLNoiseS(float x, float y, float z, float distortion, int noiseBasis1, int noiseBasis2);
            float musgraveHeteroTerrainS(float x, float y, float z, float H, float lacunarity, float octaves, float offset, int noiseBasis);
            float musgraveHybridMultiFractalS(float x, float y, float z, float H, float lacunarity, float octaves, float offset, float gain, int noiseBasis);
            float musgraveRidgedMultiFractalS(float x, float y, float z, float H, float lacunarity, float octaves, float offset, float gain, int noiseBasis);               
              
        
        // tools
        float unsignedToSignedNoiseValue(float x);
        float signedToUnsignedNoiseValue(float x);
                        

    private:

        // improved perlin
        int floor( const float x ) { return ((int)(x) - ((x) < 0 && (x) != (int)(x)));  }
	    float gradient( const int hash, const float x );
    	float gradient( const int hash, const float x, const float y );
    	float gradient( const int hash, const float x, const float y, const float z );
    	float gradient( const int hash, const float x, const float y, const float z, const float w );
            
//     	unsigned char hash[512];					// permutation table

        // worley / voronoi
        void  voronoi(float x, float y, float z, float* da, float* pa, float me, int dtype);
};

#endif // Noise
