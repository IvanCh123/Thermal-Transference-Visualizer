#ifndef COLORHANDLER_H
#define COLORHANDLER_H

#define NUMBER_OF_COLORS 5
#define RED(colorIndex) colors[colorIndex].red
#define GREEN(colorIndex) colors[colorIndex].green
#define BLUE(colorIndex) colors[colorIndex].blue

#include <QColor>

class ColorHandler
{
public:
    ColorHandler();

    /**
        * @brief Use the given temperature to calculate a color according to the established color gradient.
        * The cold temperatures will be blue, the warm ones in greens, and the hot ones in red. The temperatures that are
        * somewhere between cold and warm will be cyan, and between warm and hot will be yellow.
        * @param minimumTemperature Minimum temperature in temperature matrix.
        * @param maximumTemperature Maximum temperature in temperature matrix.
        * @param currentTemperature Current temperature to be represent as RGB Color.
        * @return The RBG Color to represent the given temperature.
        */
    QColor getRGBColor(const double& minimumTemperature, const double& maximumTemperature, double temperature) const;

private:
    /**
        * @brief Normalizes the given temperature in the range of [0, NUMBER_OF_COLORS], according to maximum and minimum temperatures.
        * @param maximumTemperature Maximum temperature in temperature matrix.
        * @param minimumTemperature Minimum temperature in temperature matrix.
        * @param currentTemperature Temperature to normalize.
        * @return The normalized version of the current temperature.
        */
    double getNormalizedTemperature(const double& maximumTemperature, const double& minimumTemperature, const double& temperature) const;

private:
    struct RGBColor
    {
        int red;
        int green;
        int blue;
        double temperature;

        RGBColor(int red, int green, int blue, double temperature)
        : red{red}, green {green}, blue{blue}, temperature{temperature}{}
    };

    RGBColor colors[NUMBER_OF_COLORS];
};

#endif // COLORHANDLER_H
