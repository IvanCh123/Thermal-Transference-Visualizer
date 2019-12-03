#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui { class MainWindow; }

class HeatMapModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)

private:
    Ui::MainWindow * ui =  nullptr;
    HeatMapModel * heatMapModel = nullptr;
    QTimer *timer = nullptr;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /**
      * @brief Paints the current state of the temperature matrix.
      */
    void paintMatrix();

private slots:
    /**
      * @brief Allows the user to select a .csv file from the file browser, which contains floating point values.
      */
    void on_openFileButton_clicked();
    /**
      * @brief Allows the user to select a new .csv file from the file browser to start a new simulation.
      */
    void on_restartProjectButton_clicked();
    /**
      * @brief Allows the simulation button to be enabled if the user has introduced a .csv file and a valid epsilon.
      * @param argument Text written on epsilonLineEdit.
      */
    void on_epsilonLineEdit_textEdited(const QString& argument);
    /**
      * @brief Starts the heat exchange simulation.
      */
    void on_simulateButton_clicked();
    /**
      * @brief Stops the heat exchange simulation and enables the Restart and OpenFile button.
      */
    void on_stopButton_clicked();
    /**
      * @brief Simulates and shows the temperature exchange between the matrix cells.
      * until reaching the state of thermal equilibrium.
      */
    void startSimulation();



protected:
    /**
     * @brief Detects the file entering the window while dragged.
     * @param event Event of dragging in the file.
     */
    virtual void dragEnterEvent(QDragEnterEvent *event);
    /**
     * @brief Detects the file leaving the window while dragged.
     * @param event Event of dragging out the file.
     */
    virtual void dragLeaveEvent(QDragLeaveEvent *event);
    /**
     * @brief Detects when the user is moving a file over the window.
     * @param event Event of moving over the window the file.
     */
    virtual void dragMoveEvent(QDragMoveEvent *event);
    /**
     * @brief Detects when a file is dropped in the window.
     * @param event Event of dropping in the file.
     */
    virtual void dropEvent(QDropEvent *event);
};

#endif // MAINWINDOW_H
