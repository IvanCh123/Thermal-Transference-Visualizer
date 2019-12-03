#ifndef HEATMAPWORKER_H
#define HEATMAPWORKER_H

#include <QThread>

#define NUMBER_OF_NEIGHBORS 4

class HeatMapWorker: public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY(HeatMapWorker)

private:
    int workerId = -1;
    int workerCount = -1;
    double epsilon = 0.0;

    std::vector< std::vector<double> > * previousTemperatureMatrix =  nullptr;
    std::vector< std::vector<double> > * currentTemperatureMatrix = nullptr;

public:
    explicit HeatMapWorker(int workerId, int workerCount, double epsilon, std::vector< std::vector<double> > * previousTemperatureMatrix, std::vector< std::vector<double> > * currentTemperatureMatrix);
    void run() override;

private:
    /**
    * @brief Calculates the start row of each worker
    * @param Amount of rows in the matrix
    * @param Amount of workers
    * @param Id of each worker
    * @return start row
    */
    size_t calculateStart(const size_t& rowCount, const int& workerCount, const int& workerId) const;
    /**
    * @brief Calculates the finish row of each worker
    * @param Amount of rows in the matrix
    * @param Amount of workers
    * @param Id of each worker
    * @return finish row
    */
    size_t calculateFinish(const size_t& rowCount, const int& workerCount, const int& workerId) const;
    /**
    * @brief Calculates the mean for each cell
    * @param Row where the value is at
    * @param Column where the value is at
    * @return mean for each cell
    */
    double getNewTemperature(const size_t &row, const size_t &column) const;

    /**
    * @brief Sees if the the value is in the border
    * @param Row where the value is at
    * @param Column where the value is atkers
    * @return true if the cell passed is in the border, false if it isn't
    */
    bool isBorder(const size_t& row, const size_t& column) const;

signals:
    /**
    * @brief emits a signal to HeatMapModel each time a worker finishes its rows.
    */
    void temperatureUpdated(bool equilibriumState);

public slots:
    /**
    * @brief Recieves a signal from HeatMapModel when a generation has passed
    */
    void updateTemperatures();

};

#endif // HEATMAPWORKER_H
