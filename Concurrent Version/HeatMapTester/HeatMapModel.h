#ifndef HEATMAPMODEL_H
#define HEATMAPMODEL_H

#include <QThread>

class FileHandler;
class HeatMapWorker;

class HeatMapModel: public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY(HeatMapModel)

private:
    double epsilon = 0.0;
    double maximumTemperature = 0.0;
    double minimumTemperature = 0.0;

    bool equilibriumState = true;
    std::vector< std::vector<double> > * currentTemperatureMatrix = nullptr;
    std::vector< std::vector<double> > * previousTemperatureMatrix = nullptr;

    int finishedWorkerCount = 0;
    std::vector< HeatMapWorker* > workers;

    FileHandler * fileHandler = nullptr;

public:
    explicit HeatMapModel(QObject* parent = nullptr);
    void run() override;
    ~HeatMapModel() override;

    void fillTemperatureMatrix(const QString& fileDirectory);
    void simulateHeatExchange();
    void interruptWorkers();

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
      * @brief Returns true if the temperature matrix reached the equilibrium state and false if it didn't.
      * @return The equilibrium state of the temperature matrix
      */
    bool getEquilibriumState() const;

    /**
      * @brief Gets the epsilon from epsilonLineEdit
      * @param epsilonVariation Epsilon value written on epsilonLineEdit.
      */
    void setEpsilon(double epsilon);

    /**
    * @brief Clears both matrix
    */
    void restartMatrix();

    /**
      * @brief Returns value at said position
    */
    double getValue(const size_t &row, const size_t &column) const;

signals:
    /**
    * @brief emits a signal to MainWindow when the simulation has finished
    */
    void simulationDone();

    /**
    * @brief emits a signal to HeatMapWorker when a generation has passed
    */
    void updateMatrix();

private slots:
    /**
      * @brief Recieves a signal from HeatMapWorker when a worker finishes its rows
    */
    void temperatureUpdateDone(bool equilibriumState);
};

#endif // HEATMAPMODEL_H

