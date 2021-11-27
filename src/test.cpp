#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "utils.h"

#define PYTHON_EXPORT extern "C" __declspec( dllexport )

PYTHON_EXPORT
void test( int *array, int rows, int cols )
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

PYTHON_EXPORT
int *createIntArray( int rows, int cols )
{
    int *result = ( int * ) malloc( sizeof( int ) * rows * cols );
    printf( "Allocated at: %lld\n", ( long long ) result );
    return result;
}

PYTHON_EXPORT
void freeIntArray( int *array )
{
    printf( "Free from: %lld\n", ( long long ) array );
    free( array );
}

void DrawCoordinateSystem( int width, int height, Color color )
{
    DrawLine( 0, height / 2, width, height / 2, color );
    DrawLine( width / 2, 0, width / 2, height, color );

    int tickSize = width / 20;

    for ( int x = width / 2; x < width; x += tickSize )
    {
        DrawLine( x, height / 2 - 15, x, height / 2 + 15, color );
        DrawLine( width - x, height / 2 - 15, width - x, height / 2 + 15, color );
    }

    for ( int y = height / 2; y < height; y += tickSize )
    {
        DrawLine( width / 2 - 15, y, width / 2 + 15, y, color );
        DrawLine( width / 2 - 15, height - y, width / 2 + 15, height - y, color );
    }
}

global_variable float32 pixelsPerUnit;

inline void ConvertPointToPixels( float32 *x, float32 *y )
{
    *x *= pixelsPerUnit;
    *y *= pixelsPerUnit;
    *x += ( float32 ) GetScreenWidth() / 2.0f;
    *y = ( float32 ) GetScreenHeight() / 2.0f - *y;
}

inline void ConvertPixelsToPoint( float32 *x, float32 *y )
{
    *x -= ( float32 ) GetScreenWidth() / 2.0f;
    *y = ( float32 ) GetScreenHeight() / 2.0f - *y;
    *x /= pixelsPerUnit;
    *y /= pixelsPerUnit;
}

void DrawData( float32 *data, int size )
{
    for ( int row = 0; row < size; ++row )
    {
        float32 x = data[ 0 ];
        float32 y = data[ 1 ];
        data += 2;
        ConvertPointToPixels( &x, &y );
        DrawCircle( x, y, 2, RED );
    }
}

PYTHON_EXPORT
void visualize( float32 *data, int size )
{
    InitWindow( 1920, 1920, "Praca Magisterska" );
    SetTargetFPS( 60 );

    float32 maxX = 10.0f;
    pixelsPerUnit = ( float32 ) GetScreenWidth() / ( maxX * 2 );

    printf( "Pixels per unit: %f", pixelsPerUnit );
    while ( !WindowShouldClose() )
    {
        BeginDrawing();

        ClearBackground( DARKGRAY );
        DrawCoordinateSystem( GetScreenWidth(), GetScreenHeight(), WHITE );
        DrawData( data, size );

        int mouseX = GetMouseX();
        int mouseY = GetMouseY();
        DrawCircle( mouseX, mouseY, 5, BLUE );
        DrawLine( mouseX, mouseY, mouseX, GetScreenHeight() / 2, LIGHTGRAY );
        DrawLine( mouseX, mouseY, GetScreenWidth() / 2, mouseY, LIGHTGRAY );
        char coords[ 64 ];
        float32 floatMouseX = ( float32 ) GetMouseX();
        float32 floatMouseY = ( float32 ) GetMouseY();

        ConvertPixelsToPoint( &floatMouseX, &floatMouseY );
        sprintf( coords, "x: %f\ny: %f", floatMouseX, floatMouseY );
        int textWidth = MeasureText( coords, 20 );
        int textX = mouseX + 30;
        if ( textX > GetScreenWidth() - textWidth ) { textX = mouseX - 30 - textWidth; }
        int textY = mouseY + 30;
        if ( textY > GetScreenHeight() - 50 ) { textY = mouseY - 30 - 50; }

        DrawText( coords, textX, textY, 20, WHITE );
        // float32 x = 1.0f;
        // float32 y = 1.0f;
        // ConvertPointToPixels( &x, &y );
        // DrawCircle( x, y, 5, BLUE );

        EndDrawing();
    }
}
