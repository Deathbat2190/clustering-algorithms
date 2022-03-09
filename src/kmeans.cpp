#include "utils.h"
#include <stdio.h>
#include <random>
#include <intrin.h>

struct KMeans_State
{
    int clusterCount;
    int randomState;
    int maxIterations;
    int sampleSize;
    float32 *centers;
};

PYTHON_EXPORT
void *InitKMeans( int clusterCount, int randomState, int maxIterations )
{
    KMeans_State *state = ( KMeans_State * ) malloc( sizeof( KMeans_State ) );
    state->clusterCount = clusterCount;
    state->randomState = randomState;
    state->maxIterations = maxIterations;
    return ( void * ) state;
}

inline float32 Distance( float32 *a, float32 *b, int sampleSize )
{
    float32 result = 0;

    // int registerCount = sampleSize / 8 + ( sampleSize % 8 == 0 ? 0 : 1 );
    for ( int i = 0; i < sampleSize; ++i )
    {
        // __m256 valueA = _mm256_load_ps( a );
        // __m256 valueB = _mm256_load_ps( b );
        // __m256 subtract = _mm256_sub_ps(valueA, valueB);
        result += ( a[ i ] - b[ i ] ) * ( a[ i ] - b[ i ] );
    }
    return result;
}

inline int PredictSample( KMeans_State *state, float32 *sample, int sampleSize )
{
    float32 minimumDistance = FLT_MAX;
    int result = -1;
    float32 *currentCenter = state->centers;

    for ( int i = 0; i < state->clusterCount; ++i )
    {
        float32 distance = Distance( currentCenter, sample, sampleSize );
        if ( distance < minimumDistance )
        {
            minimumDistance = distance;
            result = i;
        }
        currentCenter += sampleSize + 1;
    }

    Assert( result >= 0 );
    return result;
}

PYTHON_EXPORT
void Fit( KMeans_State *state, float32 *data, int sampleCount, int sampleSize )
{
    state->sampleSize = sampleSize;
    int centerSize = sampleSize + 1;
    state->centers = ( float32 * ) malloc( state->clusterCount * centerSize * sizeof( float32 ) );

    int *sampleGroups = ( int * ) malloc( sampleCount * sizeof( int ) );
    std::mt19937 mt( 0 );
    std::uniform_int_distribution< int > dist( 0, state->clusterCount - 1 );
    for ( int i = 0; i < sampleCount; ++i )
    {
        sampleGroups[ i ] = dist( mt );
    }

    int currentIteration = 0;
    while ( currentIteration++ < state->maxIterations )
    {
        float32 *currentSample = data;
        float32 *currentCenter;
        memset( state->centers, 0, state->clusterCount * centerSize * sizeof( float32 ) );
        for ( int i = 0; i < sampleCount; ++i )
        {
            currentCenter = &state->centers[ centerSize * sampleGroups[ i ] ];
            // int registerCount = sampleSize / 8 + ( sampleSize % 8 == 0 ? 0 : 1 );
            for ( int j = 0; j < sampleSize; ++j )
            {
                // __m256 valueA = _mm256_load_ps( &currentCenter[ j * 8 ] );
                // __m256 valueB = _mm256_load_ps( &currentSample[ j * 8 ] );
                // __m256 add = _mm256_add_ps( valueA, valueB );
                // _mm256_store_ps( &currentCenter[ j * 8 ], add );
                currentCenter[ j ] += currentSample[ j ];
            }
            ++currentCenter[ sampleSize ];
            currentSample += sampleSize;
        }

        currentCenter = state->centers;
        for ( int i = 0; i < state->clusterCount; ++i )
        {
            for ( int j = 0; j < sampleSize; ++j )
            {
                currentCenter[ j ] /= currentCenter[ sampleSize ];
            }
            currentCenter += centerSize;
        }

        currentSample = data;
        for ( int i = 0; i < sampleCount; ++i )
        {
            sampleGroups[ i ] = PredictSample( state, currentSample, sampleSize );
            currentSample += sampleSize;
        }
    }
    // printf( "Cluster centers:\n" );
    // for ( int i = 0; i < state->clusterCount; ++i )
    // {
    //     float32 *center = &state->centers[ i * ( sampleSize + 1 ) ];
    //     for ( int j = 0; j < sampleSize; ++j )
    //     {
    //         printf( "%f ", center[ j ] );
    //     }
    //     printf( "\n" );
    // }
    // printf( "\n" );
}

PYTHON_EXPORT
int *Predict( KMeans_State *state, float32 *data, int sampleCount, int sampleSize )
{
    int *result = ( int * ) malloc( sampleCount * sizeof( int ) );

    for ( int i = 0; i < sampleCount; ++i )
    {
        result[ i ] = PredictSample( state, data, sampleSize );
        data += sampleSize;
    }
    return result;
}

PYTHON_EXPORT
void KMeansCleanup( KMeans_State *state )
{
    free( state->centers );
    free( state );
}
