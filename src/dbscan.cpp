#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <intrin.h>
#include <string.h>
#include "utils.h"

struct DBSCAN_State
{
    float32 eps;
    int minPoints;
    int sampleCount;
    int sampleSize;
    float32 distanceBuffer[ 8 ] = {};
    int *labels;
    __m256 epsSIMD;
};

inline void DistanceSIMD( DBSCAN_State *state, float32 *data, int aIndex, int pointsOffset, int sampleCount, int sampleSize )
{
    __m256 result = _mm256_set1_ps( 0.0f );
    for ( int i = 0; i < sampleSize; ++i )
    {
        __m256 pointA = _mm256_set1_ps( ( data + aIndex )[ i * sampleCount ] );
        __m256 points = _mm256_load_ps( data + pointsOffset + i * sampleCount );
        __m256 subtract = _mm256_sub_ps( pointA, points );
        __m256 square = _mm256_mul_ps( subtract, subtract );
        result = _mm256_add_ps( result, square );
    }

    result = _mm256_cmp_ps( result, state->epsSIMD, _CMP_LE_OS );
    _mm256_store_ps( state->distanceBuffer, result );
}

inline float32 Distance( float32 *data, int aIndex, int bIndex, int sampleCount, int sampleSize )
{
    float32 result = 0;

    float32 *a = data + aIndex;
    float32 *b = data + bIndex;
    for ( int i = 0; i < sampleSize; ++i )
    {
        result += ( a[ i * sampleCount ] - b[ i * sampleCount ] ) * ( a[ i * sampleCount ] - b[ i * sampleCount ] );
    }
    return result;
}

struct Neighbors
{
    int maxNeighbors = 8;
    int *neighbors = 0;
    int neighborsCount = 0;
};

void CalculateNeighbors( DBSCAN_State *state, float32 *data, int pointIndex, Neighbors *result )
{
    int sampleCount = state->sampleCount;
    int sampleSize = state->sampleSize;
    int simdCount = sampleCount / 8;
    int rest = sampleCount % 8;

    if ( !result->neighbors )
    {
        result->neighbors = ( int * ) malloc( sizeof( int ) * result->maxNeighbors );
    }

    for ( int j = 0; j < simdCount; ++j )
    {
        DistanceSIMD( state, data, pointIndex, j * 8, sampleCount, sampleSize );
        for ( int k = 0; k < 8; ++k )
        {
            int index = j * 8 + k;
            if ( state->distanceBuffer[ k ] && index != pointIndex )
            {
                if ( result->neighborsCount == result->maxNeighbors )
                {
                    result->maxNeighbors += 8;
                    result->neighbors = ( int * ) realloc( result->neighbors, sizeof( int ) * result->maxNeighbors );
                }
                result->neighbors[ result->neighborsCount++ ] = index;
            }
        }
    }

    for ( int j = sampleCount - rest; j < sampleCount; ++j )
    {
        float32 distance = Distance( data, pointIndex, j, sampleCount, sampleSize );

        if ( distance <= state->eps * state->eps && j != pointIndex )
        {
            if ( result->neighborsCount == result->maxNeighbors )
            {
                result->maxNeighbors += 8;
                result->neighbors = ( int * ) realloc( result->neighbors, sizeof( int ) * result->maxNeighbors );
            }
            result->neighbors[ result->neighborsCount++ ] = j;
        }
    }
}

PYTHON_EXPORT
void *DBSCANInit( float32 eps, int minPoints )
{
    DBSCAN_State *state = ( DBSCAN_State * ) malloc( sizeof( DBSCAN_State ) );
    state->eps = eps;
    state->epsSIMD = _mm256_set1_ps( eps * eps );
    state->minPoints = minPoints;
    return ( void * ) state;
}

#define NOISE         -1
#define NOT_PROCESSED -2

PYTHON_EXPORT
int *DBSCANFit( DBSCAN_State *state, float32 *data, int sampleCount, int sampleSize )
{
    state->sampleCount = sampleCount;
    state->sampleSize = sampleSize;
    state->labels = ( int * ) malloc( sizeof( int ) * sampleCount );
    for ( int i = 0; i < sampleCount; ++i )
    {
        state->labels[ i ] = NOT_PROCESSED;
    }

    int *labels = state->labels;
    int clusterCount = -1;
    Neighbors neighbors;
    for ( int i = 0; i < sampleCount; ++i )
    {
        if ( labels[ i ] != NOT_PROCESSED ) { continue; }

        neighbors.neighborsCount = 0;
        CalculateNeighbors( state, data, i, &neighbors );

        if ( neighbors.neighborsCount + 1 < state->minPoints )
        {
            labels[ i ] = NOISE;
            continue;
        }

        labels[ i ] = ++clusterCount;

        Neighbors newNeighbors;
        for ( int j = 0; j < neighbors.neighborsCount; ++j )
        {
            int neighborIndex = neighbors.neighbors[ j ];
            if ( labels[ neighborIndex ] == NOISE )
            {
                labels[ neighborIndex ] = clusterCount;
                continue;
            }

            if ( labels[ neighborIndex ] != NOT_PROCESSED )
            {
                continue;
            }

            labels[ neighborIndex ] = clusterCount;
            newNeighbors.neighborsCount = 0;
            CalculateNeighbors( state, data, neighborIndex, &newNeighbors );
            if ( newNeighbors.neighborsCount + 1 >= state->minPoints )
            {
                neighbors.neighbors = ( int * ) realloc( neighbors.neighbors, sizeof( int ) * ( neighbors.neighborsCount + newNeighbors.neighborsCount ) );
                memcpy( neighbors.neighbors + neighbors.neighborsCount, newNeighbors.neighbors, sizeof( int ) * newNeighbors.neighborsCount );
                neighbors.neighborsCount += newNeighbors.neighborsCount;
            }
        }
        free( newNeighbors.neighbors );
    }
    free( neighbors.neighbors );
    return labels;
}

PYTHON_EXPORT
void DBSCANCleanup( DBSCAN_State *state )
{
    free( state->labels );
    free( state );
}
