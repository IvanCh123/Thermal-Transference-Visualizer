#include <iostream>
#include <QDir>
#include <QTextStream>
#include <QVector>
#include <QFile>

#include "HeatMapModel.h"
#include "HeatMapTester.h"

#define FLOAT_EQUALS(x,y)\
        (abs(x-y) < outputRangeDifference)

HeatMapTester::HeatMapTester(int &argc, char **argv)
    : QCoreApplication(argc, argv)
{
    this->heatMapModel = new HeatMapModel(this);
}

HeatMapTester::~HeatMapTester()
{
    delete this->heatMapModel;
}

int HeatMapTester::printHelp()
{
    std::cout << "Usage: HeatMapTester <TEST DIRECTORY>\n";
    return EXIT_FAILURE;
}

int HeatMapTester::run()
{
    if ( this->arguments().count() <= 1 )
        return printHelp();

    for ( int index = 1; index < this->arguments().count(); ++index )
        this->testDirectory( this->arguments()[index]);

    return EXIT_SUCCESS;
}


int HeatMapTester::testDirectory(const QString &testDirectoryPath)
{
    QDir dir(testDirectoryPath);
    std::cout << "CURRENT TEST DIRECTORY: " << testDirectoryPath.toStdString() << std::endl;
    if ( not dir.exists() )
    {
        std::cerr << "error: HeatMapTester: Could not open directory" << qPrintable(testDirectoryPath) << std::endl;
        return EXIT_FAILURE;
    }
    dir.setFilter(QDir::Files);

    this->testFiles = dir.entryInfoList();

    this->connect( this->heatMapModel, &HeatMapModel::simulationDone, this, &HeatMapTester::verifyOutput );

    for(int inputFileIndex = 0; inputFileIndex < testFiles.size(); ++inputFileIndex)
    {
        if(testFiles[inputFileIndex].baseName().startsWith("input"))
        {
            if(!outputMatrix.empty())
                this->outputMatrix.clear();
            this->heatMapModel->restartMatrix();

            QStringList testCaseInfo = getTestCaseInfo(testFiles[inputFileIndex]);

            this->inputFileName = "input"+testCaseInfo.at(0)+"-"+testCaseInfo.at(1)+".csv";
            this->outputFileName = "output"+testCaseInfo.at(0)+"-"+testCaseInfo.at(1)+".csv";

            this->runTestCase( getTestCaseInfo(testFiles[inputFileIndex]).at(1).toDouble(), testFiles[inputFileIndex].filePath() );
        }
    }
    return EXIT_SUCCESS;
}

int HeatMapTester::runTestCase(const double &epsilon, const QString &inputCsvFilePath)
{
    this->heatMapModel->setEpsilon(epsilon);
    this->heatMapModel->fillTemperatureMatrix(inputCsvFilePath);
    this->heatMapModel->setMaxAndMinTemperature();

    this->heatMapModel->run();
    return EXIT_SUCCESS;
}

void HeatMapTester::verifyOutput()
{
    this->heatMapModel->exit();
    this->heatMapModel->deleteLater();

    for(int index = 0; index < this->testFiles.size(); ++index)
    {
        if( testFiles[index].fileName() == this->outputFileName )
        {
            std::cout << "\n-------------------------------------------------" << std::endl;
            std::cout << "Testing: " << qPrintable( this->inputFileName )<< " with " << qPrintable( this->outputFileName ) <<"..." << std::endl;
            this->loadOutput(this->testFiles[index].filePath());
            this->compareContents(this->testFiles[index].filePath());
            std::cout << "-------------------------------------------------\n";
        }
    }
}

void HeatMapTester::compareContents(const QString &outputCsvFilename)
{
    double outputRangeDifference = this->detectPrecisionDifference(outputCsvFilename);
    for(size_t row = 1; row < this->outputMatrix.size()-1; ++row)
    {
        for(size_t column = 1; column < this->outputMatrix[0].size()-1; ++column)
        {
            if( ! FLOAT_EQUALS(this->heatMapModel->getValue(row,column), this->outputMatrix[row][column]) )
            {
               std::cerr << "error: HeatMapTester: Test case failed " << " at [" << row <<"]["<< column << "]"<<std::endl;
               return;
            }
        }
    }
}

QStringList HeatMapTester::getTestCaseInfo(const QFileInfo &fileInfo)
{
    return fileInfo.fileName().split("input").at(1).split(".csv").at(0).split("-");
}

int HeatMapTester::loadOutput(const QString &outputCsvFilename)
{
    if( !outputCsvFilename.isEmpty() )
    {
        QFile file(outputCsvFilename);

        if( file.open(QFile::ReadOnly | QFile::Text) )
        {
            QTextStream fileContent(&file);
            QStringList rowElements = {};
            QString row = "";
            while( !fileContent.atEnd() )
            {
                row = fileContent.readLine();
                rowElements = row.split(',');

                std::vector<double> newRow = {};
                for(int currentElement  = 0; currentElement < rowElements.length(); ++currentElement)
                {
                    newRow.push_back(rowElements.at(currentElement).toDouble());
                }
                this->outputMatrix.push_back( newRow );
            }
        }
        file.close();
    }
    return EXIT_SUCCESS;
}



double HeatMapTester::detectPrecisionDifference(const QString &outputCsvFilename)
{
    // Open the file
        QFile file(outputCsvFilename);
        if ( ! file.open(QIODevice::ReadOnly | QIODevice::Text) )
            return -1.0;

        // Manages text file encoding
        QTextStream textStream( &file );

        // The minimum difference, calculated counting the number of decimal digits
        double diff = 1.0;

        // Read a char at time
        QChar ch = 0;
        bool dotFound = false;
        while ( ! (textStream >> ch).atEnd() )
        {
            // Check only first value, if a new one starts, stop here
            if ( ch == ',' )
                break;

            // If we found the decimal separator
            if ( ch == '.' )
                dotFound = true;

            // If we found a digit and we are in the decimal part, adjust our difference
            if ( dotFound && ch.isDigit() )
                diff /= 10.0;
        }

        // QFile destructor will close the file
        return diff;
}
