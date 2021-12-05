#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "utils.h"
#include <math.h>

#define PYTHON_EXPORT extern "C" __declspec( dllexport )

global_variable float32 xAxisRange;
global_variable float32 pixelsPerUnit;
global_variable float32 coordinateSystemCenterX; // in pixels
global_variable float32 coordinateSystemCenterY; // in pixels
global_variable Color clustersColors[] = { RED, BLUE, GREEN, YELLOW, PURPLE, MAGENTA, ORANGE,
                                           LIME, DARKBLUE, DARKGREEN, DARKPURPLE, DARKBROWN };

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

inline void ConvertPointToPixels( float32 *x, float32 *y )
{
    *x *= pixelsPerUnit;
    *y *= pixelsPerUnit;
    *x += coordinateSystemCenterX;
    *y = coordinateSystemCenterY - *y;
}

inline void ConvertPixelsToPoint( float32 *x, float32 *y )
{
    *x -= coordinateSystemCenterX;
    *y = coordinateSystemCenterY - *y;
    *x /= pixelsPerUnit;
    *y /= pixelsPerUnit;
}

inline void ConvertUnitsToPixels( float32 *value )
{
    *value = *value * pixelsPerUnit;
}

inline void ConvertPixelsToUnits( float32 *value )
{
    *value = *value / pixelsPerUnit;
}

inline void DrawLineInCoordinateSystem( float32 startX, float32 startY, float32 endX, float32 endY, Color color )
{
    ConvertPointToPixels( &startX, &startY );
    ConvertPointToPixels( &endX, &endY );
    DrawLine( startX, startY, endX, endY, color );
}

inline void DrawCircleInCoordinateSystem( float32 centerX, float32 centerY, float32 radius, Color color )
{
    ConvertPointToPixels( &centerX, &centerY );
    ConvertUnitsToPixels( &radius );
    DrawCircle( centerX, centerY, radius, color );
}

void DrawCoordinateSystem( Color color )
{
    float32 zero = 0.0f;
    float32 xAxisStartX = 0.0f;
    float32 xAxisEndX = ( float32 ) GetScreenWidth();
    ConvertPixelsToPoint( &xAxisStartX, &zero );
    ConvertPixelsToPoint( &xAxisEndX, &zero );
    DrawLineInCoordinateSystem( xAxisStartX, 0, xAxisEndX, 0, color );

    float32 yAxisStartY = 0.0f;
    float32 yAxisEndY = ( float32 ) GetScreenHeight();
    ConvertPixelsToPoint( &zero, &yAxisStartY );
    ConvertPixelsToPoint( &zero, &yAxisEndY );
    DrawLineInCoordinateSystem( 0, yAxisStartY, 0, yAxisEndY, color );

    for ( int x = xAxisStartX; x < xAxisEndX; ++x )
    {
        DrawLineInCoordinateSystem( x, -0.125, x, 0.125, color );
    }

    for ( int y = yAxisStartY; y > yAxisEndY; --y )
    {
        DrawLineInCoordinateSystem( -0.125, y, 0.125, y, color );
    }
}

void DrawData( float32 *data, int size, int *clusters )
{
    for ( int row = 0; row < size; ++row )
    {
        float32 x = data[ 0 ];
        float32 y = data[ 1 ];
        data += 2;
        DrawCircleInCoordinateSystem( x, y, 0.05f, clustersColors[ clusters[ row ] ] );
    }
}

void DrawMousePosition()
{
    float32 floatMouseX = ( float32 ) GetMouseX();
    float32 floatMouseY = ( float32 ) GetMouseY();
    ConvertPixelsToPoint( &floatMouseX, &floatMouseY );

    DrawLineInCoordinateSystem( floatMouseX, floatMouseY, floatMouseX, 0, LIGHTGRAY );
    DrawLineInCoordinateSystem( floatMouseX, floatMouseY, 0, floatMouseY, LIGHTGRAY );

    float32 radius = 5.0f; // in pixels
    ConvertPixelsToUnits( &radius );
    DrawCircleInCoordinateSystem( floatMouseX, floatMouseY, radius, BLUE );

    char coords[ 64 ];
    sprintf( coords, "x: %f\ny: %f", floatMouseX, floatMouseY );
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();
    int textWidth = MeasureText( coords, 20 );
    int textX = mouseX + 30;
    if ( textX > GetScreenWidth() - textWidth ) { textX = mouseX - 30 - textWidth; }
    int textY = mouseY + 30;
    if ( textY > GetScreenHeight() - 50 ) { textY = mouseY - 30 - 50; }

    DrawText( coords, textX, textY, 20, WHITE );
}

void ZoomCoordinateSystem( float32 zoomDelta )
{
    xAxisRange -= GetMouseWheelMove();
    if ( xAxisRange < 1 ) { xAxisRange = 1; }
    if ( xAxisRange > 135 ) { xAxisRange = 135; }
    float32 mouseX = ( float32 ) GetMouseX();
    float32 mouseY = ( float32 ) GetMouseY();
    ConvertPixelsToPoint( &mouseX, &mouseY );

    pixelsPerUnit = ( float32 ) GetScreenWidth() / xAxisRange;

    float32 newMouseX = ( float32 ) GetMouseX();
    float32 newMouseY = ( float32 ) GetMouseY();
    ConvertPixelsToPoint( &newMouseX, &newMouseY );

    float32 deltaX = newMouseX - mouseX;
    float32 deltaY = newMouseY - mouseY;
    ConvertUnitsToPixels( &deltaX );
    ConvertUnitsToPixels( &deltaY );
    coordinateSystemCenterX += deltaX;
    coordinateSystemCenterY -= deltaY;
}

PYTHON_EXPORT
void visualize( float32 *data, int size, int *clusters )
{
    InitWindow( 1920, 1920, "Praca Magisterska" );
    SetTargetFPS( 60 );

    coordinateSystemCenterX = 0.5f * ( float32 ) GetScreenWidth();
    coordinateSystemCenterY = 0.5f * ( float32 ) GetScreenHeight();

    xAxisRange = 20.0f;
    pixelsPerUnit = ( float32 ) GetScreenWidth() / xAxisRange;

    int mouseDragStartX;
    int mouseDragStartY;
    while ( !WindowShouldClose() )
    {
        if ( GetMouseWheelMove() != 0.0f )
        {
            ZoomCoordinateSystem( GetMouseWheelMove() );
        }

        if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) )
        {
            mouseDragStartX = GetMouseX();
            mouseDragStartY = GetMouseY();
        }

        if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) )
        {
            int deltaX = GetMouseX() - mouseDragStartX;
            coordinateSystemCenterX += deltaX;
            mouseDragStartX = GetMouseX();
            int deltaY = GetMouseY() - mouseDragStartY;
            coordinateSystemCenterY += deltaY;
            mouseDragStartY = GetMouseY();
        }

        BeginDrawing();
        {
            ClearBackground( DARKGRAY );
            DrawFPS( 5, 5 );
            DrawCoordinateSystem( WHITE );
            DrawData( data, size, clusters );
            DrawMousePosition();
        }
        EndDrawing();
    }
}
