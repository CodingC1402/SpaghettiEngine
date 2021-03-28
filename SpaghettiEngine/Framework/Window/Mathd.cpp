#include "Mathd.h"

double Mathd::Clamp( double value, double max, double min )
{
    if ( value > max )
        value = max;
    else if ( value < min )
        value = min;

    return value;
}

double Mathd::RoundToPrecision( double number, int precision )
{
    precision = (int)Clamp( precision, 0, 7 );
    long long value = static_cast<int>(number * pow( 10, precision ));
    number = value / (double)(pow( 10, precision ));
    return number;
}
