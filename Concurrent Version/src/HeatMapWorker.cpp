#include "HeatMapWorker.h"

HeatMapWorker::HeatMapWorker(int workerId, int workerCount, double epsilon, std::vector<std::vector<double> > * previousTemperatureMatrix, std::vector<std::vector<double> > * currentTemperatureMatrix):
   QThread ()
  , workerId(workerId)
  , workerCount(workerCount)
  , epsilon(epsilon)
  , previousTemperatureMatrix(previousTemperatureMatrix)
  , currentTemperatureMatrix(currentTemperatureMatrix)
{}

void HeatMapWorker::run()
{
    this->exec();
}

void HeatMapWorker::updateTemperatures()
{
    std::vector< std::vector<double> >* temp = this->previousTemperatureMatrix;
    this->previousTemperatureMatrix = this->currentTemperatureMatrix;
    this->currentTemperatureMatrix = temp;

    size_t startRow = this->calculateStart(this->previousTemperatureMatrix->size(), this->workerCount, this->workerId);
    size_t finishRow =  this->calculateFinish(this->previousTemperatureMatrix->size(), this->workerCount, this->workerId);
    bool equilibriumState = true;

    for( size_t row = startRow; row < finishRow ; ++row )
    {
        for( size_t column = 0; column < (*this->currentTemperatureMatrix)[row].size(); ++column )
        {
            if( !this->isInterruptionRequested() )
            {
                if( !this->isBorder(row,column) )
                    (*this->currentTemperatureMatrix)[row][column] = this->getNewTemperature(row,column);

                    if( abs(  (*this->currentTemperatureMatrix)[row][column] - (*this->previousTemperatureMatrix)[row][column] ) > this->epsilon )
                        equilibriumState = false;
            }
        }
    }
    emit temperatureUpdated(equilibriumState);
}

size_t HeatMapWorker::calculateStart(const size_t& rowCount, const int& workerCount, const int& workerId) const
{
    const size_t equitative = workerId * ((rowCount) / workerCount);
    const size_t overload = qMin(static_cast<size_t>(workerId), (rowCount) % workerCount);
    return equitative + overload;
}

size_t HeatMapWorker::calculateFinish(const size_t &rowCount, const int &workerCount, const int &workerId) const
{
    return calculateStart(rowCount, workerCount, workerId + 1);
}


double HeatMapWorker::getNewTemperature(const size_t &row, const size_t &column)const
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
        sum += (*this->previousTemperatureMatrix)[ row + rowMove[currentNeighbour] ] [ column + columnMove[currentNeighbour] ];
    }

    return (sum / NUMBER_OF_NEIGHBORS); // The new temperature will be the average of the neighboring temperatures.
}


bool HeatMapWorker::isBorder(const size_t &row, const size_t &column) const
{
    return (row == 0 || row == (*this->previousTemperatureMatrix).size()-1 || column == 0 || column == (*this->previousTemperatureMatrix)[row].size()-1);
}
