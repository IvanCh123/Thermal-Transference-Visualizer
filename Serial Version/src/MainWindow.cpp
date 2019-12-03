#include <QDragEnterEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QMimeData>
#include <QPixmap>
#include <QTimer>

#include "HeatMapModel.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->heatMapModel = new HeatMapModel(this);
    this->timer = new QTimer(this);
    this->setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QStringList fileTypes;
    fileTypes << "csv";

    QString fileDirectory;
    QList<QUrl> filePath;
    QList<QUrl>:: Iterator iterator;
    filePath = event->mimeData()->urls();

    for(iterator = filePath.begin(); iterator != filePath.end(); ++iterator)
    {
        fileDirectory = iterator->path();
        QFileInfo droppedFile(fileDirectory);

        if(fileTypes.contains(droppedFile.suffix().trimmed(), Qt::CaseInsensitive))
        {
            this->heatMapModel->fillTemperatureMatrix(fileDirectory);
            this->heatMapModel->setMaxAndMinTemperature();
            this->paintMatrix();
            this->ui->openFileButton->setDisabled(true);
            this->ui->statusBar->showMessage( "Rows: " + QString::number(this->heatMapModel->getNumberOfRows()) + " Columns: " + QString::number(this->heatMapModel->getNumberOfColumns()) );
        }
        else
        {
            this->ui->statusBar->showMessage("Invalid file format. Please try again");
        }
    }
}

void MainWindow::on_epsilonLineEdit_textEdited(const QString& argument)
{
    if( !this->ui->openFileButton->isEnabled() )
    {
        bool ok(false);
        if(  (argument.trimmed().length() > 0) && argument.trimmed().toDouble(&ok) )
        {
            this->ui->simulateButton->setEnabled( true );
        }
        else
        {
            this->ui->simulateButton->setDisabled( true );
        }
    }
}

void MainWindow::on_openFileButton_clicked()
{
    this->heatMapModel->fillTemperatureMatrix(QFileDialog::getOpenFileName(this,"File Explorer"," ", "CSV Files (*.csv)"));
    this->heatMapModel->setMaxAndMinTemperature();
    this->paintMatrix();

    this->ui->restartProjectButton->setEnabled(true);
    this->ui->epsilonLineEdit->setEnabled(true);
    this->ui->refreshRatioLineEdit->setEnabled(true);

    this->ui->openFileButton->setDisabled(true);

    this->ui->statusBar->showMessage( "Rows: " + QString::number(this->heatMapModel->getNumberOfRows()) + " Columns: " + QString::number(this->heatMapModel->getNumberOfColumns()) );
}

void MainWindow::on_simulateButton_clicked()
{
    this->ui->stopButton->setEnabled(true);

    this->ui->simulateButton->setDisabled(true);
    this->ui->restartProjectButton->setDisabled(true);
    this->ui->epsilonLineEdit->setDisabled(true);
    this->ui->refreshRatioLineEdit->setDisabled(true);

    this->heatMapModel->getEpsilon( this->ui->epsilonLineEdit->text().toDouble() );

    bool ok(false);
    int refreshRatio = 0;
    if( this->ui->refreshRatioLineEdit->text().trimmed().length() > 0  && this->ui->refreshRatioLineEdit->text().toDouble(&ok) )
    {
        refreshRatio = this->ui->refreshRatioLineEdit->text().toDouble() * 1000;
    }
    else // Default value for the refresh ratio.
    {
        refreshRatio = 1000;
    }

    this->ui->statusBar->showMessage("Stabilizing the temperature...");
    connect(timer, SIGNAL(timeout()), this, SLOT( startSimulation() ) );
    timer->start( refreshRatio );
}

void MainWindow::on_stopButton_clicked()
{
    this->timer->stop();

    this->ui->restartProjectButton->setEnabled(true);
    this->ui->simulateButton->setEnabled(true);
    this->ui->openFileButton->setEnabled(true);

    this->ui->stopButton->setDisabled(true);

    this->ui->statusBar->showMessage("Simulation stopped.");
}

void MainWindow::on_restartProjectButton_clicked()
{   
    this->ui->openFileButton->setEnabled(true);

    this->ui->simulateButton->setDisabled(true);
    this->ui->restartProjectButton->setDisabled(true);
    this->ui->stopButton->setDisabled(true);

    this->ui->epsilonLineEdit->setDisabled(true);
    this->ui->refreshRatioLineEdit->setDisabled(true);

    this->ui->simulationLabel->clear();
    this->ui->epsilonLineEdit->clear();
    this->ui->refreshRatioLineEdit->clear();
    this->ui->statusBar->clearMessage();

    this->heatMapModel->restartMatrix();
}

void MainWindow::startSimulation()
{
    this->heatMapModel->updateTemperatureMatrix();
    if( this->heatMapModel->equilibriumStateReached() )
    {
        this->timer->stop();
        this->ui->restartProjectButton->setEnabled(true);
        this->ui->stopButton->setDisabled(true);
        this->ui->statusBar->showMessage("Equilibrium state reached.");
    }
    this->heatMapModel->updatePreviousTemperatureMatrix();
    this->paintMatrix();

}

void MainWindow::paintMatrix()
{
    QImage heatMapImage(this->heatMapModel->getNumberOfColumns(), this->heatMapModel->getNumberOfRows(), QImage::Format_RGB32);

    for(size_t row = 0; row < this->heatMapModel->getNumberOfRows(); ++row)
    {
        for(size_t column = 0; column < this->heatMapModel->getNumberOfColumns(); ++column)
        {
            heatMapImage.setPixelColor( column, row, this->heatMapModel->getRGBColor(row, column) );
        }
    }

    QPixmap heatPixelMap( this->heatMapModel->getNumberOfRows(), this->heatMapModel->getNumberOfColumns() );
    this->ui->simulationLabel->setScaledContents(true);
    heatPixelMap.convertFromImage(heatMapImage);
    this->ui->simulationLabel->setPixmap(heatPixelMap.scaled(this->ui->simulationLabel->width(), this->ui->simulationLabel->height(), Qt::KeepAspectRatio) );
}



