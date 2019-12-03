#include "ColorHandler.h"

// Color gradient: (0)Blue, (1)cyan, (2)green, (3)yellow, (4)red.
ColorHandler::ColorHandler(): colors{ RGBColor(0,0,255,0.0), RGBColor(0,255,255,1.0), RGBColor(0,255,0,2.0), RGBColor(255,255,0,3.0), RGBColor(255,0,0,4.0) }
{}

QColor ColorHandler::getRGBColor(const double& minimumTemperature, const double& maximumTemperature, double temperature) const
{
    // The temperature is normalized to the range of [0,NUMBER_OF_COLORS-1], in this case, [0,4].
    temperature = this->getNormalizedTemperature(maximumTemperature,minimumTemperature,temperature);
    // The color corresponding to this temperature will be the mixture between two colors in the gradient. This index is associated with the first color in the array.
    int previousColorIndex = 0;
    // The difference in terms of temperature between these two colors.
    double temperatureRangeLength = 0;
    // Fraction between these two colors where this temperature is located.
    double fractionBetween = 0;

    for(int currentColorIndex = 0; currentColorIndex < NUMBER_OF_COLORS; ++currentColorIndex)
    {
        if( temperature <= colors[currentColorIndex].temperature)
        {
            previousColorIndex = ( (currentColorIndex - 1) > 0 ) ? (currentColorIndex - 1) : 0;
            temperatureRangeLength = colors[ previousColorIndex].temperature - colors[currentColorIndex].temperature;
            fractionBetween = (0 <= temperatureRangeLength && temperatureRangeLength <= 1) ? 0 : (temperature - colors[currentColorIndex].temperature)/temperatureRangeLength;
            int red =  static_cast<int>( RED(currentColorIndex) + ( RED(previousColorIndex) - RED(currentColorIndex) ) * fractionBetween );
            int green = static_cast<int>( GREEN(currentColorIndex) + ( GREEN(previousColorIndex) - GREEN(currentColorIndex) ) * fractionBetween );
            int blue = static_cast<int>( BLUE(currentColorIndex) + ( BLUE(previousColorIndex) - BLUE(currentColorIndex) ) * fractionBetween );
            return  QColor(red,green,blue);
        }
    }
}

double ColorHandler::getNormalizedTemperature(const double& maximumTemperature, const double& minimumTemperature, const double& temperature) const
{
    return (temperature - minimumTemperature)/(maximumTemperature - minimumTemperature) * (NUMBER_OF_COLORS - 1);
}
