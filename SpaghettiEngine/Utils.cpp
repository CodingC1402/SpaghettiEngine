#include "Utils.h"

int Utils::GetSize( char *arr )
{
    return sizeof(*arr) / sizeof(arr[0]);
}

int Utils::GetSize( int *arr )
{
    return sizeof( *arr ) / sizeof( arr[0] );
}
