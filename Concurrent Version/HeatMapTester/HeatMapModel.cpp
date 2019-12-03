#include "FileHandler.h"
#include "HeatMapModel.h"
#include "HeatMapWorker.h"

HeatMapModel::HeatMapModel(QObject* parent)
    : QThread ()
{
    this->fileHandler = new FileHandler(parent);
    this->previousTemperatureMatrix = new  std::vector< std::vector<double> > ();
    this->currentTemperatureMatrix = new  std::vector< std::vector<double> > ();
}

HeatMapModel::~HeatMapModel()
{
    delete this->previousTemperatureMatrix;
    delete this->currentTemperatureMatrix;
    delete this->fileHandler;
}

void HeatMapModel::run()
{
    this->simulateHeatExchange();
    this->exec();
}

void HeatMapModel::fillTemperatureMatrix(const QString &fileDirectory)
{
    if( !this->previousTemperatureMatrix->empty() )
        this->previousTemperatureMatrix->clear();
    this->fileHandler->processFile(fileDirectory,*this->previousTemperatureMatrix);
    *this->currentTemperatureMatrix = *previousTemperatureMatrix;
}

size_t HeatMapModel::getNumberOfRows() const
{
     return this->currentTemperatureMatrix->size();
}

size_t HeatMapModel::getNumberOfColumns() const
{
     return (*this->currentTemperatureMatrix)[0].size();
}

void HeatMapModel::setMaxAndMinTemperature()
{
    for( size_t row = 0;  row < this->getNumberOfRows(); ++row )
    {
        for( size_t column = 0; column < this->getNumberOfColumns(); ++column )
        {
            if( (*this->currentTemperatureMatrix)[row][column] > this->maximumTemperature )
                this->maximumTemperature = (*this->currentTemperatureMatrix)[row][column];
            if( (*this->currentTemperatureMatrix)[row][column] < this->minimumTemperature )
                this->minimumTemperature = (*this->currentTemperatureMatrix)[row][column];
         }
     }
}

void HeatMapModel::setEpsilon(double epsilon)
{
    this->epsilon = epsilon;
}

void HeatMapModel::restartMatrix()
{
    this->previousTemperatureMatrix->clear();
    this->currentTemperatureMatrix->clear();
}

void HeatMapModel::simulateHeatExchange()
{
    this->workers.clear();
    this->finishedWorkerCount =  0;
    this->equilibriumState = true;

    int workerCount = qMin( QThread::idealThreadCount(), static_cast<int>(this->getNumberOfRows()) );

    for( int workerId = 0; workerId < workerCount; ++workerId )
    {
        HeatMapWorker* worker = new HeatMapWorker{workerId, workerCount, this->epsilon, this->previousTemperatureMatrix, this->currentTemperatureMatrix};
        this->workers.push_back(worker);
        this->connect( worker, &HeatMapWorker::temperatureUpdated, this, &HeatMapModel::temperatureUpdateDone );
        this->connect( this, &HeatMapModel::updateMatrix, worker, &HeatMapWorker::updateTemperatures );
        this->workers[workerId]->start();
    }
     emit updateMatrix();
}

void HeatMapModel::temperatureUpdateDone(bool equilibriumState)
{

    if(!equilibriumState)
        this->equilibriumState = equilibriumState;

    if( ++this->finishedWorkerCount == this->workers.size()-1 )
    {
        std::vector< std::vector<double> >* temp = this->previousTemperatureMatrix;
        this->previousTemperatureMatrix = this->currentTemperatureMatrix;
        this->currentTemperatureMatrix = temp;

        if( this->getEquilibriumState() )
        {
            emit simulationDone();
            for ( HeatMapWorker* worker : this->workers )
            {
                worker->exit();
                worker->deleteLater();
            }
        }
        else
        {
            this->equilibriumState = true;
            this->finishedWorkerCount = 0;
            emit updateMatrix();
        }
    }
}

void HeatMapModel::interruptWorkers()
{
    Q_ASSERT(this->workers.size() > 0);

    for ( HeatMapWorker* worker : this->workers )
    {
        worker->exit();
        worker->deleteLater();
    }
}

bool HeatMapModel::getEquilibriumState() const
{
    return this->equilibriumState;
}

double HeatMapModel::getValue(const size_t &row, const size_t &column) const
{
    return (*this->currentTemperatureMatrix)[row][column];
}
