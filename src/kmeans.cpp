#include <random>
#include <math.h>
#include "utils.h"

struct KMeans_State
{
    int clusterCount;
    int randomState;
    int maxIterations;
    float32 *centers;
};

PYTHON_EXPORT
void *KMeansInit( int clusterCount, int randomState, int maxIterations )
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

    for ( int i = 0; i < sampleSize; ++i )
    {
        result += ( a[ i ] - b[ i ] ) * ( a[ i ] - b[ i ] );
    }
    return result;
    // return sqrtf( result );
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
void KMeansFit( KMeans_State *state, float32 *data, int sampleCount, int sampleSize )
{
    int centerSize = sampleSize + 1;
    state->centers = ( float32 * ) malloc( state->clusterCount * centerSize * sizeof( float32 ) );

    int *sampleGroups = ( int * ) malloc( sampleCount * sizeof( int ) );
    std::mt19937 mt( state->randomState );
    std::uniform_int_distribution< int > dist( 0, sampleCount - 1 );

    float32 *tempCenter = state->centers;
    for ( int i = 0; i < state->clusterCount; ++i )
    {
        int index = dist( mt );
        for ( int j = 0; j < sampleSize; ++j )
        {
            tempCenter[ j ] = data[ index * sampleSize + j ];
        }
        tempCenter[ sampleSize ] = 0;
        tempCenter += centerSize;
    }

    int currentIteration = 0;
    while ( currentIteration++ < state->maxIterations )
    {
        float32 *currentSample = data;

        bool earlyStop = true;
        for ( int i = 0; i < sampleCount; ++i )
        {
            int oldGroup = sampleGroups[ i ];
            sampleGroups[ i ] = PredictSample( state, currentSample, sampleSize );
            currentSample += sampleSize;
            if ( oldGroup != sampleGroups[ i ] )
            {
                earlyStop = false;
            }
        }

        if ( earlyStop )
        {
            break;
        }

        float32 *currentCenter;
        currentSample = data;
        memset( state->centers, 0, state->clusterCount * centerSize * sizeof( float32 ) );
        for ( int i = 0; i < sampleCount; ++i )
        {
            currentCenter = &state->centers[ centerSize * sampleGroups[ i ] ];
            for ( int j = 0; j < sampleSize; ++j )
            {
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
    }
    printf( "Finished after %d iterations\n", currentIteration );
    free( sampleGroups );
}

PYTHON_EXPORT
int *KMeansPredict( KMeans_State *state, float32 *data, int sampleCount, int sampleSize )
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
