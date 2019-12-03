#include "HeatMapModel.h"
#include "FileHandler.h"

HeatMapModel::HeatMapModel(QObject* parent)
{
    Q_UNUSED(parent);
}

void HeatMapModel::fillTemperatureMatrix(const QString &filePath)
{
    if(!this->previousTemperatureMatrix.empty())
        this->previousTemperatureMatrix.clear();
    this->fileHandler->processFile(filePath,this->previousTemperatureMatrix);
}

size_t HeatMapModel::getNumberOfRows() const
{
     return this->previousTemperatureMatrix.size();
}

size_t HeatMapModel::getNumberOfColumns() const
{
     return this->previousTemperatureMatrix[0].size();
}

void HeatMapModel::setMaxAndMinTemperature()
{
    this->maximumTemperature = this->minimumTemperature = 0;

    for(size_t row = 0;  row < this->getNumberOfRows(); ++row)
    {
        for(size_t column = 0; column < this->getNumberOfColumns(); ++column)
        {
            if( this->previousTemperatureMatrix[row][column] > this->maximumTemperature )
                this->maximumTemperature = this->previousTemperatureMatrix[row][column];
            if( this->previousTemperatureMatrix[row][column] < this->minimumTemperature )
                this->minimumTemperature = this->previousTemperatureMatrix[row][column];
         }
     }
}
void HeatMapModel:: updateTemperatureMatrix()
{
    this->isStabilized = true;
    double temporalValue;
    std::vector<double> temporalVector = {};
    for( size_t row = 0; row < this->getNumberOfRows(); ++row )
    {
        temporalVector.clear();
        for( size_t column = 0; column < this->getNumberOfColumns(); ++column )
        {
            temporalValue = 0.0;
            if( this->isBorder(row,column) )
            {
                temporalValue = previousTemperatureMatrix[row][column];
            }
            else
            {
                temporalValue = this->getNewTemperature(row,column);

                if( abs(  temporalValue - previousTemperatureMatrix[row][column] ) > this->epsilonVariation )
                    this->isStabilized = false;
            }
            temporalVector.push_back(temporalValue);
        }
        currentTemperatureMatrix.push_back(temporalVector);
    }
}

void HeatMapModel::updatePreviousTemperatureMatrix()
{
    this->previousTemperatureMatrix = this->currentTemperatureMatrix;
    this->currentTemperatureMatrix.clear();
}

double HeatMapModel::getNewTemperature(const size_t &row, const size_t &column) const
{
    int rowMove [] = {0, 0, -1, 1};
    int columnMove [] = {1, -1, 0, 0};

    // Right neighbour: matrix[currentRow + 0][currentColumn + 1]
    // Left  neighbour: matrix[currentRow + 0][currentColumn - 1]
    // Top neighbour: matrix[currentRow - 1][currentColumn + 0]
    // Lower neighbour: matrix[currentRow + 1][currentColumn + 0]

    double sum = 0;  // Sum of the temperatures corresponding to the neighbors.

    for( int currentNeighbour = 0; currentNeighbour < NUMBER_OF_NEIGHBORS; ++currentNeighbour )
    {
        sum += this->previousTemperatureMatrix[ row + rowMove[currentNeighbour] ] [ column + columnMove[currentNeighbour] ];
    }

    return (sum / NUMBER_OF_NEIGHBORS); // The new temperature will be the average of the neighboring temperatures.
}

bool HeatMapModel::equilibriumStateReached() const
{
    return this->isStabilized;
}

bool HeatMapModel::isBorder(const size_t row, const size_t col) const
{
    return (row == 0 || col == 0 || row == this->getNumberOfRows()-1 || col == this->getNumberOfColumns()-1 );
}

void HeatMapModel::setEpsilon(double temperatureVariation)
{
    this->epsilonVariation = temperatureVariation;
}

void HeatMapModel::restartMatrix()
{
    this->previousTemperatureMatrix.clear();
    this->currentTemperatureMatrix.clear();
}

double HeatMapModel::getValue(const size_t &row, const size_t &column) const
{
    return this->currentTemperatureMatrix[row][column];
}
