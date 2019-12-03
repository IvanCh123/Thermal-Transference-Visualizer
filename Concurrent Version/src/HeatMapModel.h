#ifndef HEATMAPMODEL_H
#define HEATMAPMODEL_H

#include <QThread>

class FileHandler;
class ColorHandler;
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
    ColorHandler * colorHandler = nullptr;

public:
    explicit HeatMapModel(QObject* parent = nullptr);
    void run() override;
    ~HeatMapModel() override;

    /**
     * @brief Calls a method in FileHandler that goes through the .csv file and parses it
     * into a matrix of doubles.
    */
    void fillTemperatureMatrix(const QString& fileDirectory);

    /**
     * @brief Creates and starts the workers.
    */
    void simulateHeatExchange();

    /**
     * @brief Goes through all the workers that we've created and makes them exit their events queue, and then
     * deletes them
    */
    void stoptWorkers();

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
    void setEpsilon(double epsilon);

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

