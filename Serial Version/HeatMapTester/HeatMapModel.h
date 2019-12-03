#ifndef HEATMAPMODEL_H
#define HEATMAPMODEL_H

#include <QObject>

class FileHandler;

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

public:
    /**
     * @brief HeatMapModel constructor.
     * @param parent HeatMapModel parent.
     */
    explicit HeatMapModel(QObject* parent = nullptr);

    /**
     * @brief Read from a CSV file the temperatures that are assigned to the temperature matrix.
     * @param filePath The path where the CSV file is located.
     */
    void fillTemperatureMatrix(const QString& filePath);

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
      * @brief Gets the epsilon from epsilonLineEdit
      * @param epsilonVariation Epsilon value written on epsilonLineEdit.
      */
    void setEpsilon(double epsilonVariation);

    /**
      * @brief Cleans both matrix, so they don't store old values when starting a new simulation.
      */
    void restartMatrix();

    /**
     * @brief Obtains the value located in specified cell of the temperature matrix.
     * @param row Desired row.
     * @param column Desired column.
     * @return The value located in temperatureMatrix[row][column].
     */
    double getValue(const size_t &row, const size_t &column) const;


private:
    /**
      * @brief Calculate the new temperature of the specified position as the average of the right, left, top and bottom temperature.
      * @param row  Desired row.
      * @param column Desired column.
      * @return The average of the neighboring temperatures.
      */
    double getNewTemperature(const size_t &row, const size_t &column) const;

    /**
      * @brief Determines whether a specified position in the matrix is at the edges.
      * @param row Desired row.
      * @param column Desired column.
      * @return True if the specified position is at the edge.
      */
    bool isBorder(const size_t row, const size_t col) const;
};

#endif // HEATMAPMODEL_H
