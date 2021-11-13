#include "stdio.h"

extern "C"
{
    __declspec( dllexport ) void test( int *array, int rows, int cols )
    {
        for ( int row = 0; row < rows; ++row )
        {
            for ( int col = 0; col < cols; ++col )
            {
                printf( " %d", array[ cols * row + col ] );
            }
            printf( "\n" );
        }
    }
}
