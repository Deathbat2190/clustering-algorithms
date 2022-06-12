#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "utils.h"

struct Hierarchical_State
{
    int sampleCount;
    int sampleSize;
    int clusterCount;
    int *labels;
};

inline float32 Distance( float32 *data, int aIndex, int bIndex, int sampleCount, int sampleSize )
{
    float32 result = 0;

    float32 *a = data + aIndex;
    float32 *b = data + bIndex;
    for ( int i = 0; i < sampleSize; ++i )
    {
        result += ( a[ i * sampleCount ] - b[ i * sampleCount ] ) * ( a[ i * sampleCount ] - b[ i * sampleCount ] );
    }
    return sqrtf( result );
}

#define CLUSTER_INDEX   0
#define CLUSTER_COUNT   1
#define CLUSTER_MEMBERS 2

float32 CalculateDistance( Hierarchical_State *state, float32 *data, int *clusterA, int clusterACount, int *clusterB, int clusterBCount )
{
    int sampleSize = state->sampleSize;
    int sampleCount = state->sampleCount;
    float32 result = 0.0f;

    for ( int i = 0; i < clusterACount; ++i )
    {
        for ( int j = 0; j < clusterBCount; ++j )
        {
            result += Distance( data, clusterA[ CLUSTER_MEMBERS + i ], clusterB[ CLUSTER_MEMBERS + j ], sampleCount, sampleSize );
        }
    }

    return result / ( clusterACount * clusterBCount );
}

PYTHON_EXPORT
void *HierarchicalInit( int clusterCount )
{
    Hierarchical_State *state = ( Hierarchical_State * ) malloc( sizeof( Hierarchical_State ) );
    state->clusterCount = clusterCount;
    return ( void * ) state;
}

PYTHON_EXPORT
int *HierarchicalFit( Hierarchical_State *state, float32 *data, int sampleCount, int sampleSize )
{
    state->sampleCount = sampleCount;
    state->sampleSize = sampleSize;
    state->labels = ( int * ) malloc( sizeof( int ) * sampleCount );

    int rowCount = sampleCount + 2;
    int *clusters = ( int * ) calloc( sampleCount * rowCount, sizeof( int ) );

    for ( int i = 0; i < sampleCount; ++i )
    {
        clusters[ i * rowCount + CLUSTER_INDEX ] = i;
        clusters[ i * rowCount + CLUSTER_COUNT ] = 1;
        clusters[ i * rowCount + CLUSTER_MEMBERS ] = i;
    }
    float32 *distances = ( float32 * ) calloc( sampleCount * sampleCount, sizeof( float32 ) );

    int currentClusterCount = sampleCount;
    int *clusterToMergeA = 0;
    int clusterToMergeAIndex = 0;
    int clusterToMergeACount = 0;
    int *clusterToMergeB = 0;
    int clusterToMergeBRowIndex = 0;
    int clusterToMergeBCount = 0;
    float32 minDistance = FLT_MAX;

    for ( int i = 0; i < currentClusterCount; ++i )
    {
        int *clusterA = &clusters[ i * rowCount ];
        int clusterACount = clusterA[ CLUSTER_COUNT ];
        int clusterAIndex = clusterA[ CLUSTER_INDEX ];
        for ( int j = i + 1; j < currentClusterCount; ++j )
        {
            int *clusterB = &clusters[ j * rowCount ];
            int clusterBCount = clusterB[ CLUSTER_COUNT ];
            int clusterBIndex = clusterB[ CLUSTER_INDEX ];
            float32 distance = CalculateDistance( state, data, clusterA, clusterACount, clusterB, clusterBCount );
            distances[ clusterBIndex + clusterAIndex * sampleCount ] = distance;
            distances[ clusterAIndex + clusterBIndex * sampleCount ] = distance;
            if ( distance < minDistance )
            {
                minDistance = distance;
                clusterToMergeA = clusterA;
                clusterToMergeAIndex = clusterAIndex;
                clusterToMergeACount = clusterACount;
                clusterToMergeB = clusterB;
                clusterToMergeBRowIndex = j;
                clusterToMergeBCount = clusterBCount;
            }
        }
    }

    memcpy( clusterToMergeA + clusterToMergeACount + CLUSTER_MEMBERS,
            clusterToMergeB + CLUSTER_MEMBERS,
            sizeof( int ) * clusterToMergeBCount );
    clusterToMergeA[ CLUSTER_COUNT ] += clusterToMergeBCount;
    clusterToMergeACount = clusterToMergeA[ CLUSTER_COUNT ];

    currentClusterCount -= 1;
    memcpy( clusterToMergeB, clusterToMergeB + rowCount,
            sizeof( int ) * ( currentClusterCount - clusterToMergeBRowIndex ) * rowCount );

    while ( currentClusterCount > state->clusterCount )
    {
        for ( int i = 0; i < currentClusterCount; ++i )
        {
            int *cluster = &clusters[ i * rowCount ];
            int clusterCount = cluster[ CLUSTER_COUNT ];
            int clusterIndex = cluster[ CLUSTER_INDEX ];

            if ( clusterIndex == clusterToMergeAIndex ) { continue; }

            float32 distance = CalculateDistance( state, data, clusterToMergeA, clusterToMergeACount, cluster, clusterCount );
            distances[ clusterIndex + clusterToMergeAIndex * sampleCount ] = distance;
            distances[ clusterToMergeAIndex + clusterIndex * sampleCount ] = distance;
        }

        clusterToMergeA = 0;
        clusterToMergeAIndex = 0;
        clusterToMergeACount = 0;
        clusterToMergeB = 0;
        clusterToMergeBRowIndex = 0;
        clusterToMergeBCount = 0;
        minDistance = FLT_MAX;

        for ( int i = 0; i < currentClusterCount; ++i )
        {
            int *clusterA = &clusters[ i * rowCount ];
            int clusterACount = clusterA[ CLUSTER_COUNT ];
            int clusterAIndex = clusterA[ CLUSTER_INDEX ];
            for ( int j = i + 1; j < currentClusterCount; ++j )
            {
                int *clusterB = &clusters[ j * rowCount ];
                int clusterBCount = clusterB[ CLUSTER_COUNT ];
                int clusterBIndex = clusterB[ CLUSTER_INDEX ];

                float32 distance = distances[ clusterBIndex + clusterAIndex * sampleCount ];
                // float32 distance = distances[ clusterAIndex + clusterBIndex * sampleCount ]; //SLOW
                if ( distance < minDistance )
                {
                    minDistance = distance;
                    clusterToMergeA = clusterA;
                    clusterToMergeAIndex = clusterAIndex;
                    clusterToMergeACount = clusterACount;
                    clusterToMergeB = clusterB;
                    clusterToMergeBRowIndex = j;
                    clusterToMergeBCount = clusterBCount;
                }
            }
        }

        // merge the clusters
        memcpy( clusterToMergeA + clusterToMergeACount + CLUSTER_MEMBERS,
                clusterToMergeB + CLUSTER_MEMBERS,
                sizeof( int ) * clusterToMergeBCount );
        clusterToMergeA[ CLUSTER_COUNT ] += clusterToMergeBCount;
        clusterToMergeACount = clusterToMergeA[ CLUSTER_COUNT ];

        currentClusterCount -= 1;
        memcpy( clusterToMergeB, clusterToMergeB + rowCount,
                sizeof( int ) * ( currentClusterCount - clusterToMergeBRowIndex ) * rowCount );
    }

    for ( int i = 0; i < currentClusterCount; ++i )
    {
        int *cluster = &clusters[ i * rowCount ];
        int count = cluster[ CLUSTER_COUNT ];

        for ( int j = 0; j < count; ++j )
        {
            state->labels[ cluster[ CLUSTER_MEMBERS + j ] ] = i;
        }
    }

    return state->labels;
}

PYTHON_EXPORT
void HierarchicalCleanup( Hierarchical_State *state )
{
    free( state->labels );
    free( state );
}
