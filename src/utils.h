#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#define global_variable static
#define local_persist   static
#define internal        static
#define pi32            3.14159265359f

#define PYTHON_EXPORT extern "C" __declspec( dllexport )

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

typedef float float32;
typedef double float64;

#if SLOW
    #define Assert( expression )    \
        if ( !( expression ) )      \
        {                           \
            int *volatile null = 0; \
            *null = 0;              \
        }
#else
    #define Assert( expression )
#endif

#define Kilobytes( value ) ( ( value ) *1024LL )
#define Megabytes( value ) ( Kilobytes( value ) * 1024LL )
#define Gigabytes( value ) ( Megabytes( value ) * 1024LL )
#define Terabytes( value ) ( Gigabytes( value ) * 1024LL )

#define ArrayCount( array ) ( ( int ) ( sizeof( array ) / sizeof( ( array )[ 0 ] ) ) )

#endif
