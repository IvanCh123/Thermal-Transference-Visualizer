#include <QDragEnterEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QMimeData>
#include <QPixmap>
#include <QTimer>
#include <QTime>

#include "HeatMapModel.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);
    this->heatMapModel = new HeatMapModel(this);
    this->timer = new QTimer(this);
    this->timeElapsed = new QTime();
    this->setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->heatMapModel;
    delete this->timer;
    delete this->timeElapsed;
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
            this->ui->epsilonLineEdit->clear();
            this->ui->epsilonLineEdit->setEnabled(true);
            this->ui->refreshRatioLineEdit->setEnabled(true);
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

    this->ui->epsilonLineEdit->clear();
    this->ui->epsilonLineEdit->setEnabled(true);
    this->ui->refreshRatioLineEdit->setEnabled(true);

    this->ui->openFileButton->setDisabled(true);

    this->ui->statusBar->showMessage( "Rows: " + QString::number(this->heatMapModel->getNumberOfRows()) + " Columns: " + QString::number(this->heatMapModel->getNumberOfColumns()) );
}

void MainWindow::on_simulateButton_clicked()
{
    this->ui->openFileButton->setDisabled(true);
    this->ui->stopButton->setEnabled(true);
    this->ui->simulateButton->setDisabled(true);
    this->ui->epsilonLineEdit->setDisabled(true);
    this->ui->refreshRatioLineEdit->setDisabled(true);

    this->heatMapModel->setEpsilon( this->ui->epsilonLineEdit->text().toDouble() );

    bool ok(false);
    int refreshRatio = 0;
    if( this->ui->refreshRatioLineEdit->text().trimmed().length() > 0  && this->ui->refreshRatioLineEdit->text().toDouble(&ok) )
    {
        refreshRatio = this->ui->refreshRatioLineEdit->text().toDouble() * 1000;
    }
    else // Default value for the refresh ratio
    {
        refreshRatio = 1000;
    }

    this->ui->statusBar->showMessage("Stabilizing...");

    this->connect( this->heatMapModel, &HeatMapModel::simulationDone, this, &MainWindow::simulation_finished );
    this->connect( this->timer, &QTimer::timeout, this, &MainWindow::update_interface );

    this->timeElapsed->start();

    timer->start( refreshRatio );
    this->heatMapModel->start();
}

void MainWindow::on_stopButton_clicked()
{
    this->timer->stop();
    this->heatMapModel->stoptWorkers();

    this->ui->openFileButton->setEnabled(true);
    this->ui->stopButton->setDisabled(true);

    QString simDuration = QString::number(this->timeElapsed->elapsed()/1000.0);
    this->ui->statusBar->showMessage("Simulation stopped after "+ simDuration +" seconds");

    this->heatMapModel->exit();
}

void MainWindow::simulation_finished()
{
    this->timer->stop();
    this->paintMatrix();
    this->ui->stopButton->setDisabled(true);
    this->ui->openFileButton->setEnabled(true);

    QString simDuration = QString::number(this->timeElapsed->elapsed()/1000.0);
    this->ui->statusBar->showMessage("Equilibrium state reached after "+ simDuration +" seconds");
    this->heatMapModel->exit();
}

void MainWindow::update_interface()
{
    this->paintMatrix();
}

void MainWindow::paintMatrix()
{
    QImage heatMapImage(this->heatMapModel->getNumberOfColumns(), this->heatMapModel->getNumberOfRows(), QImage::Format_RGB32);

    for( size_t row = 0; row < this->heatMapModel->getNumberOfRows(); ++row )
    {
        for( size_t column = 0; column < this->heatMapModel->getNumberOfColumns(); ++column )
        {
            heatMapImage.setPixelColor( column, row, this->heatMapModel->getRGBColor(row, column) );
        }
    }
    QPixmap heatPixelMap( this->heatMapModel->getNumberOfRows(), this->heatMapModel->getNumberOfColumns() );
    this->ui->simulationLabel->setScaledContents(true);
    heatPixelMap.convertFromImage(heatMapImage);
    this->ui->simulationLabel->setPixmap(heatPixelMap.scaled(this->ui->simulationLabel->width(), this->ui->simulationLabel->height(), Qt::KeepAspectRatio) );
}
