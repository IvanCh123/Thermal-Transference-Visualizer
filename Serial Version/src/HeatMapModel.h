#ifndef HEATMAPMODEL_H
#define HEATMAPMODEL_H

#include <QObject>

class FileHandler;
class ColorHandler;

#define NUMBER_OF_NEIGHBORS 4

class HeatMapModel: public QObject
{
    Q_DISABLE_COPY(HeatMapModel)

private:
    double maximumTemperature = 0.0;
    double minimumTemperature = 0.0;
    double epsilonVariation = 0.0;
    bool isStabilized = true;

    std::vector< std::vector <double> > currentTemperatureMatrix;
    std::vector< std::vector <double> > previousTemperatureMatrix;

    FileHandler * fileHandler = nullptr;
    ColorHandler * colorHandler = nullptr;

public:
    explicit HeatMapModel(QObject* parent = nullptr);

    void fillTemperatureMatrix(const QString& fileDirectory);

    /**
      * @brief Returns the number of rows of the matrix.
      * @return The number of rows of the matrix.
      */
    size_t getNumberOfRows() const;

    /**
      * @brief Returns the number of columns of the matrix.
      * @return The number of columns of the matrix.
      */
    size_t getNumberOfColumns() const;

    /**
      * @brief Calculate the maximum and minimum temperatures located in the matrix and return them through pointers.
      */
    void setMaxAndMinTemperature();

    /**
      * @brief Update the temperature of each cell by calculating the average of its four neighbors.
      */
    void updateTemperatureMatrix();

    /**
      * @brief Set the previous temperature matrix as the current one, in order to calculate the new values of the current one.
      */
    void updatePreviousTemperatureMatrix();

    /**
      * @brief Returns true if the temperature matrix reached the equilibrium state and false if it didn't.
      * @return The equilibrium state of the temperature matrix
      */
    bool equilibriumStateReached() const;

    /**
      * @brief Determines whether a specified position in the matrix is at the edges.
      * @param row Desired row.
      * @param column Desired column.
      * @return True if the specified position is at the edge.
      */
    bool isBorder(const size_t row, const size_t col) const;

    /**
      * @brief Obtains the temperature in the specific position of the matrix and returns its color in RGB format.
      * @param row Desired row.
      * @param column Desired column.
      * @return An RGB color corresponding to the specified temperature.
      */
    QColor getRGBColor(const size_t &row, const size_t &column) const;

    /**
      * @brief Gets the epsilon from epsilonLineEdit
      * @param epsilonVariation Epsilon value written on epsilonLineEdit.
      */
    void getEpsilon(double epsilonVariation);

    /**
      * @brief Cleans both matrix, so they don't store old values when starting a new simulation.
      */
    void restartMatrix();

private:
    /**
      * @brief Calculate the new temperature of the specified position as the average of the right, left, top and bottom temperature.
      * @param row  Desired row.
      * @param column Desired column.
      * @return The average of the neighboring temperatures.
      */
    double getNewTemperature(const size_t &row, const size_t &column) const;
};

#endif // HEATMAPMODEL_H
