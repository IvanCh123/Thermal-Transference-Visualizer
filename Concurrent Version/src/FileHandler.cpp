#include <QFile>
#include <QVector>
#include <QTextStream>

#include "FileHandler.h"

FileHandler::FileHandler(QObject* parent):  QObject(parent)
{}

void FileHandler::processFile(const QString& filePath, std::vector< std::vector <double> > &targetMatrix) const
{
    if( !filePath.isEmpty() )
    {
        QFile file(filePath);

        if( file.open(QFile::ReadOnly | QFile::Text) )
        {
            QTextStream fileContent(&file);
            QStringList rowElements = {};
            QString row = "";
            while( !fileContent.atEnd() )
            {
                row = fileContent.readLine();
                rowElements = row.split(',');

                targetMatrix.push_back( this->getNewRow(rowElements) );
            }
        }
        file.close();

    }
}

std::vector<double> FileHandler::getNewRow(const QStringList &rowElements) const
{
    std::vector<double> newRow = {};
    for(int currentElement  = 0; currentElement < rowElements.length(); ++currentElement)
    {
        newRow.push_back(rowElements.at(currentElement).toDouble());
    }
    return newRow;
}
