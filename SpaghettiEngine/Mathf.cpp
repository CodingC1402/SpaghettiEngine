#include "Mathf.h"

float Mathf::Clamp( float value, float max, float min )
{
    if ( value > max )
        value = max;
    else if ( value < min )
        value = min;

    return value;
}

float Mathf::RoundToPrecision( float number, int precision )
{
    precision = (int)Clamp( precision, 0, 7 );
    long long value = static_cast<int>(number * pow( 10, precision ));
    number = value / (float)(pow( 10, precision ));
    return number;
}
